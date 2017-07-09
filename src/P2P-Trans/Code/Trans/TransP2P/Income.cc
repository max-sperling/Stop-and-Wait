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
    m_size = 0;
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
        if(m_size == 0)
        {
            if(m_socket->bytesAvailable() < sizeof(int))
                return;
            QByteArray buffer = m_socket->read(sizeof(int));
            std::array<char, 4> size = {
                buffer[0], buffer[1], buffer[2], buffer[3]};
            m_size = Packet::byteArrayToInt(size);
        }

        if(m_socket->bytesAvailable() < m_size)
            return;

        m_data = m_socket->read(m_size);
        string data = m_data.toStdString();
        Packet packet(m_size, data);

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

        m_size = 0;
        m_data.clear();
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
