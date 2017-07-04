/************************/
/* Author: Max Sperling */
/************************/

#include "Client.hh"

#include <QFile>
#include <QFileInfo>
#include "Packet.hh"

using namespace std;

// ***** Public ************************************************************************************
Client::Client(IViewPtr viewPtr)
{
    m_viewPtr = viewPtr;
    m_socket = new QTcpSocket();
}

Client::~Client()
{
    m_socket->deleteLater();
}

bool Client::init(string addr, unsigned int port)
{
    m_addr = QString::fromStdString(addr);
    m_port = port;

    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    return true;
}

bool Client::sendFile(std::string fileName)
{
    if(!connectToServer()) return false;

    QFile file(QByteArray::fromStdString(fileName));
    if(!file.open(QIODevice::ReadOnly))
    {
        m_viewPtr->logIt("Client: Can't open file");
        return false;
    }

    m_viewPtr->logIt("Client: Sending started");

    // Meta
    QFileInfo fileInfo(file);
    string name = fileInfo.fileName().toStdString();
    Packet metaPacket(Packet::Meta, name);
    m_socket->write(QByteArray::fromStdString(metaPacket.getRaw()));

    // Content
    QByteArray buffer;
    while(!(buffer = file.read(Packet::fileSizeMax)).isEmpty())
    {
        string content = buffer.toStdString();
        Packet contentPacket(Packet::Content, content);
        m_socket->write(QByteArray::fromStdString(contentPacket.getRaw()));
    }

    m_viewPtr->logIt("Client: Sending finished");
    file.close();

    m_socket->disconnectFromHost();

    return true;
}
// *************************************************************************************************

// ***** Private ***********************************************************************************
bool Client::connectToServer()
{
    if(m_socket->state() == QTcpSocket::ConnectedState)
        return true;

    m_socket->connectToHost(m_addr, m_port);

    if(!m_socket->waitForConnected(2000))
    {
        m_viewPtr->logIt("Client: Can't connect");
        return false;
    }
    m_viewPtr->logIt("Client: Connected");

    return true;
}
// *************************************************************************************************

// ***** Private Slots *****************************************************************************
void Client::onDisconnected()
{
    m_viewPtr->logIt("Client: Disconnected");
}
// *************************************************************************************************
