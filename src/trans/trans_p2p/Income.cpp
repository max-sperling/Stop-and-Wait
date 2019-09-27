/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hpp"

#include <QDir>
#include "Packet.hpp"

using namespace std;

// ***** Public ************************************************************************************
Income::Income(IViewPtr viewPtr, qintptr socketId)
{
    m_viewPtr = viewPtr;
    m_socketId = socketId;
    m_fileName = "";
    m_logIdent = "[Server][" + to_string(m_socketId) + "]";
    m_blockSize = 0;
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
        m_viewPtr->logIt(m_logIdent + " Can't setup socket");
        return;
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()), Qt::QueuedConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    m_viewPtr->logIt(m_logIdent + " Connected");

    exec();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Income::onGetTCPStream()
{
    if(m_blockSize == 0)
    {
        if(m_socket->bytesAvailable() < sizeof(int))
            return;

        QByteArray buffer = m_socket->read(sizeof(int));
        std::array<char, 4> size = {
            buffer[0], buffer[1], buffer[2], buffer[3]};
        m_blockSize = Packet::byteArrayToInt(size);
    }

    if(m_socket->bytesAvailable() < m_blockSize)
        return;

    m_blockData = m_socket->read(m_blockSize);
    string data = m_blockData.toStdString();
    Packet packet(m_blockSize, data);

    switch(packet.getType())
    {
    case Packet::Meta:
        {
            if(!m_fileName.empty())
            {
                m_viewPtr->logIt(m_logIdent + " Receiving already started");
                return;
            }

            string folder = "./Files/";
            m_fileName = folder + packet.getData();
            m_viewPtr->logIt(m_logIdent + " Started receiving: " + m_fileName);

            QDir().mkdir(QString::fromStdString(folder));
            m_file.setFileName(QString::fromStdString(m_fileName));

            if (!m_file.open(QFile::WriteOnly | QFile::Truncate))
            {
                m_viewPtr->logIt(m_logIdent + " Can't open file");
                return;
            }
        }
        break;
    case Packet::Content:
        {
            if(m_file.write(QByteArray::fromStdString(packet.getData())) == -1)
            {
                m_viewPtr->logIt(m_logIdent + " Can't write data");
                return;
            }
        }
        break;
    default:
        m_viewPtr->logIt(m_logIdent + " Unknown packet type");
        break;
    }

    m_blockSize = 0;
    m_blockData.clear();
}

void Income::onDisconnected()
{
    m_viewPtr->logIt(m_logIdent + " Finished receiving: " + m_fileName);
    m_file.close();

    m_viewPtr->logIt(m_logIdent + " Disconnected");
    m_socket->deleteLater();

    exit(0);
}
// *************************************************************************************************
