/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hh"

#include <QDir>
#include "Packet.hh"

using namespace std;

// ***** Public ************************************************************************************
Income::Income(IViewPtr viewPtr, qintptr socketId)
{
    m_viewPtr = viewPtr;
    m_socketId = socketId;
}

Income::~Income()
{

}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Income::run()
{
    m_socket = new QTcpSocket();

    if(!m_socket->setSocketDescriptor(m_socketId))
    {
        m_viewPtr->logIt("Server: Can't set socket descriptor");
        return;
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()), Qt::QueuedConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    m_viewPtr->logIt("Server: Connection started: " + to_string(m_socketId));

    exec();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Income::onGetTCPStream()
{
    while(m_socket->bytesAvailable())
    {
        while(m_socket->bytesAvailable() < sizeof(int)){}
        QByteArray buffer = m_socket->read(sizeof(int));
        std::array<char, 4> value = {
            buffer[0], buffer[1], buffer[2], buffer[3]};
        unsigned int size = Packet::byteArrayToInt(value);

        while(m_socket->bytesAvailable() < size){}
        string data = m_socket->read(size).toStdString();
        Packet packet(size, data);

        switch(packet.getType())
        {
        case Packet::Meta:
            {
                m_viewPtr->logIt("Server: Receiving started");

                QString dir = "./Files/";
                QDir().mkdir(dir);
                m_file.setFileName(dir + QString::fromStdString(packet.getData()));

                if (!m_file.open(QFile::WriteOnly | QFile::Truncate))
                {
                    m_viewPtr->logIt("Server: Can't open file");
                    return;
                }
            }
            break;
        case Packet::Content:
            {
                if(m_file.write(QByteArray::fromStdString(packet.getData())) == -1)
                {
                    m_viewPtr->logIt("Server: Can't write data");
                    return;
                }
            }
            break;
        default:
            m_viewPtr->logIt("Server: Unknown packet type");
            break;
        }
    }
}

void Income::onDisconnected()
{
    m_viewPtr->logIt("Server: Receiving finished");
    m_file.close();

    m_viewPtr->logIt("Server: Connection closed: " + to_string(m_socketId));
    m_socket->deleteLater();

    exit(0);
}
// *************************************************************************************************
