/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hh"

#include "Server.hh"
#include "Packet.hh"
#include <QDir>

using namespace std;

// ***** Public ************************************************************************************
Income::Income(IViewPtr viewPtr, qintptr socketDescriptor)
{
    this->viewPtr = viewPtr;
    this->socketDescriptor = socketDescriptor;

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Income::run()
{
    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()), Qt::QueuedConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    viewPtr->logIt("Server: Connection started: " + to_string(socketDescriptor));

    exec();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Income::onGetTCPStream()
{
    auto byteArrayToInt = [](std::array<unsigned char, 4> value)
        {
            return (value[0] << 24)
                + ((value[1] & 0xFF) << 16)
                + ((value[2] & 0xFF) << 8)
                + (value[3] & 0xFF);
        };

    while(socket->bytesAvailable())
    {
        QByteArray buffer = socket->read(sizeof(int));
        std::array<unsigned char, 4> value;
        value[0] = buffer[0];
        value[1] = buffer[1];
        value[2] = buffer[2];
        value[3] = buffer[3];
        //(buffer.begin(), buffer.end());
        unsigned int size = byteArrayToInt(value);

        buffer = socket->read(sizeof(char));
        unsigned int type = buffer.toUInt();

        buffer = socket->read(size-sizeof(char));
        string data = buffer.toStdString();

        switch(type)
        {
        case Packet::Meta:
            {
                viewPtr->logIt("Server: Receiving started");
                
                QString dir = "./Files/";
                QDir().mkdir(dir);
                file.setFileName(dir + QString::fromStdString(data));

                if (!file.open(QFile::WriteOnly | QFile::Truncate))
                {
                    viewPtr->logIt("Server: Can't open file");
                    return;
                }
            }
            break;
        case Packet::Content:
            {
                if(file.write(QByteArray::fromStdString(data)) == -1)
                {
                    viewPtr->logIt("Server: Can't write data");
                    return;
                }
            }
            break;
        default:
            viewPtr->logIt("Server: Unknown packet type");
            break;
        }
    }
}

void Income::onDisconnected()
{
    viewPtr->logIt("Server: Receiving finished");
    file.close();

    viewPtr->logIt("Server: Connection closed: " + to_string(socketDescriptor));
    socket->deleteLater();

    exit(0);
}
// *************************************************************************************************
