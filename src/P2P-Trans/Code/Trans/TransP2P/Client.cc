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
    thread = new QThread();
    socket = new QTcpSocket();
    this->viewPtr =  viewPtr;
    //moveToThread(thread);
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
    connect(thread, SIGNAL(started()), this, SLOT(onStartedThread()));

    thread->start();

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
    string baseName = fileInfo.fileName().toStdString();
    MetaPacket metaPacket(baseName);
    QByteArray metaData = QByteArray::fromStdString(metaPacket.getData());
    socket->write(metaData);

    // Content
    QByteArray content = file.readAll();
    ContentPacket contentPacket(content.toStdString());
    QByteArray contentData = QByteArray::fromStdString(contentPacket.getData());
    socket->write(contentData);
    // todo: in Loop

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
void Client::onStartedThread()
{
    
}

void Client::onDisconnected()
{
    viewPtr->logIt("Client: Disconnected");
}
// *************************************************************************************************
