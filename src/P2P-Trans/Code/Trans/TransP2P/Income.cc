/************************/
/* Author: Max Sperling */
/************************/

#include "Income.hh"

#include <QFile>
#include "Server.hh"

// ***** Public ************************************************************************************
Income::Income(qintptr socketDescriptor)
{
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

    qDebug() << "Connection started: " << socketDescriptor;

    exec();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Income::onGetTCPStream()
{
    QByteArray q = socket->readAll();

    QFile file;
    file.setFileName("tmp.txt");


    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << "Can't open file for written";
        return;
    }
    file.write(q);

    file.close();
        //socket->disconnectFromHost();
}

void Income::onDisconnected()
{
    qDebug() << "Connection closed: " << socketDescriptor;
    socket->deleteLater();
    exit(0);
}
// *************************************************************************************************
