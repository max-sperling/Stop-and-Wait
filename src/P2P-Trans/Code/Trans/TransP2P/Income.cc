/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hh"

#include "Server.hh"
#include "Packet.hh"

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

    connect(socket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()), Qt::DirectConnection);
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
        unsigned int size = buffer.toUInt();

        buffer = socket->read(sizeof(char));
        unsigned int type = buffer.toUInt();

        buffer = socket->read(size-sizeof(char));
        string data = buffer.toStdString();

        switch(type)
        {
        case Packet::Meta:
            {
                viewPtr->logIt("Server: Receiving started");
                
                file.setFileName(QString::fromStdString(data));

                if (!file.open(QIODevice::WriteOnly))
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
    viewPtr->logIt("Server: Receiving finished");
}

void Income::onDisconnected()
{
    viewPtr->logIt("Server: Connection closed: " + to_string(socketDescriptor));
    file.close();
    socket->deleteLater();
    exit(0);
}
// *************************************************************************************************
