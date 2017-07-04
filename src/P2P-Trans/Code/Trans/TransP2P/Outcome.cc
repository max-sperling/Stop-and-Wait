/************************/
/* Author: Max Sperling */
/************************/

#include "Outcome.hh"

#include <QFile>
#include <QFileInfo>
#include "Packet.hh"

using namespace std;

// ***** Public ************************************************************************************
Outcome::Outcome(IViewPtr viewPtr, string addr, unsigned int port, string fileName)
{
    m_viewPtr = viewPtr;
    m_addr = addr;
    m_port = port;
    m_fileName = fileName;
}

Outcome::~Outcome()
{

}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Outcome::run()
{
    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if(!connectToServer()) return;
    if(!openFile()) return;
    if(!sendFile()) return;

    m_file.close();
    m_socket->disconnectFromHost();

    exec();
}
// *************************************************************************************************

// ***** Private ***********************************************************************************
bool Outcome::connectToServer()
{
    if(m_socket->state() == QTcpSocket::ConnectedState)
        return true;

    m_socket->connectToHost(QString::fromStdString(m_addr), m_port);

    if(!m_socket->waitForConnected(2000))
    {
        m_viewPtr->logIt("Client: Can't connect");
        return false;
    }
    m_viewPtr->logIt("Client: Connected");

    return true;
}

bool Outcome::openFile()
{
    m_file.setFileName(QByteArray::fromStdString(m_fileName));
    if(!m_file.open(QIODevice::ReadOnly))
    {
        m_viewPtr->logIt("Client: Can't open file");
        return false;
    }

    return true;
}

bool Outcome::sendFile()
{
    m_viewPtr->logIt("Client: Sending started");

    // Meta
    QFileInfo fileInfo(m_file);
    string name = fileInfo.fileName().toStdString();
    Packet metaPacket(Packet::Meta, name);
    m_socket->write(QByteArray::fromStdString(metaPacket.getRaw()));

    // Content
    QByteArray buffer;
    while(!(buffer = m_file.read(Packet::maxSizeByte)).isEmpty())
    {
        string content = buffer.toStdString();
        Packet contentPacket(Packet::Content, content);
        m_socket->write(QByteArray::fromStdString(contentPacket.getRaw()));
    }

    m_viewPtr->logIt("Client: Sending finished");

    return true;
}
// *************************************************************************************************

// ***** Private Slots *****************************************************************************
void Outcome::onDisconnected()
{
    m_viewPtr->logIt("Client: Disconnected");
    m_socket->deleteLater();

    exit(0);
}
// *************************************************************************************************
