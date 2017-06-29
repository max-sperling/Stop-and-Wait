/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hh"

#include "Server.hh"

// ***** Public ************************************************************************************
Income::Income(qintptr Id, QObject *parent) : QThread(parent)
{
    socketDescriptor = Id;
    server = (Server *)parent;

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Income::run()
{
    tcpSocket = new QTcpSocket();

    if(!tcpSocket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(tcpSocket->error());
        return;
    }

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onGetTCPStream()), Qt::DirectConnection);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qDebug() << "Connection started: " << socketDescriptor;

    exec();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Income::onGetTCPStream()
{
    QString content = tcpSocket->readAll();

    if(content == "register")
    {
        qDebug() << "adding Host: " << tcpSocket->peerAddress().toString();
        emit addHost(tcpSocket->peerAddress());
        tcpSocket->write("registered");
    }
    else if(content == "unregister")
    {
        qDebug() << "removing Host: " << tcpSocket->peerAddress().toString();
        emit remHost(tcpSocket->peerAddress());
    }
    else if(content == "request")
    {
        QString hostsStr = "";
        QList<QHostAddress>::iterator i;
        QList<QHostAddress> *hostList = server->getHosts();
        for(i=hostList->begin(); i!=hostList->end(); ++i)
        {
            if(i+1 == hostList->end())
                hostsStr += i->toString();
            else
                hostsStr += i->toString() + ",";
        }
        qDebug() << "sending Hostlist: " << hostsStr;
        tcpSocket->write(hostsStr.toLatin1());
    }
    else qDebug() << "unknown Packet";

    tcpSocket->disconnectFromHost();
}

void Income::onDisconnected()
{
    qDebug() << "Connection closed: " << socketDescriptor;
    tcpSocket->deleteLater();
    exit(0);
}
// *************************************************************************************************
