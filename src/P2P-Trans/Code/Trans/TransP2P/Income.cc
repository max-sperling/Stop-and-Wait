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
    while(socket->bytesAvailable())
    {
        QByteArray buffer = socket->read(sizeof(int));
        std::array<char, 4> value = {
            buffer[0], buffer[1], buffer[2], buffer[3]};        
        unsigned int size = Packet::byteArrayToInt(value);

        Packet packet(size, (socket->read(size).toStdString()));

        switch(packet.getType())
        {
        case Packet::Meta:
            {
                viewPtr->logIt("Server: Receiving started");
                
                QString dir = "./Files/";
                QDir().mkdir(dir);
                file.setFileName(dir + QString::fromStdString(packet.getData()));

                if (!file.open(QFile::WriteOnly | QFile::Truncate))
                {
                    viewPtr->logIt("Server: Can't open file");
                    return;
                }
            }
            break;
        case Packet::Content:
            {
                if(file.write(QByteArray::fromStdString(packet.getData())) == -1)
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
