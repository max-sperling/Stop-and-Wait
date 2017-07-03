/************************/
/* Author: Max Sperling */
/************************/

#include "Client.hh"

#include <QFile>
#include <QFileInfo>
#include <iostream>
#include <thread>
#include "Packet.hh"
using namespace std;

// ***** Public ************************************************************************************
Client::Client(IViewPtr viewPtr)
{
    socket = new QTcpSocket();
    this->viewPtr =  viewPtr;
}

Client::~Client()
{
    socket->deleteLater();
}

bool Client::init(string addr, unsigned int port)
{
    this->addr = QString::fromStdString(addr);
    this->port = port;

    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    return true;
}

bool Client::sendFile(std::string fileName)
{
    if(!connectToServer()) return false;

    QFile file(QByteArray::fromStdString(fileName));
    if(!file.open(QIODevice::ReadOnly))
    {
        viewPtr->logIt("Client: Can't open file");
        return false;
    }

    viewPtr->logIt("Client: Sending started");

    // Meta
    QFileInfo fileInfo(file);
    string name = fileInfo.fileName().toStdString();
    Packet metaPacket(Packet::Meta, name);
    socket->write(QByteArray::fromStdString(metaPacket.getRaw()));

    // Content
    QByteArray buffer;
    while(!(buffer = file.read(Packet::fileSizeMax)).isEmpty())
    {
        string content = buffer.toStdString();
        Packet contentPacket(Packet::Content, content);
        socket->write(QByteArray::fromStdString(contentPacket.getRaw()));
    }

    viewPtr->logIt("Client: Sending finished");
    file.close();

    socket->disconnectFromHost();

    return true;
}
// *************************************************************************************************

// ***** Private ***********************************************************************************
bool Client::connectToServer()
{
    if(socket->state() == QTcpSocket::ConnectedState)
        return true;

    socket->connectToHost(addr, port);

    if(!socket->waitForConnected(2000))
    {
        viewPtr->logIt("Client: Can't connect");
        return false;
    }
    viewPtr->logIt("Client: Connected");

    return true;
}
// *************************************************************************************************

// ***** Private Slots *****************************************************************************
void Client::onDisconnected()
{
    viewPtr->logIt("Client: Disconnected");
}
// *************************************************************************************************
