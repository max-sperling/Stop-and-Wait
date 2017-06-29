/************************/
/* Author: Max Sperling */
/************************/

#include "Server.hh"

#include <QDebug>
#include "Income.hh"

// ***** Public ************************************************************************************
Server::Server(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    hostList = new QList<QHostAddress>();
}

bool Server::init(unsigned int port)
{
    if(!this->listen(QHostAddress::Any, port))
        return false;

    return true;
}

QList<QHostAddress> *Server::getHosts()
{
    return hostList;
}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Server::incomingConnection(qintptr socketDescriptor)
{
    Income *income = new Income(socketDescriptor, this);

    connect(income, SIGNAL(addHost(QHostAddress)), this, SLOT(addHost(QHostAddress)));
    connect(income, SIGNAL(remHost(QHostAddress)), this, SLOT(remHost(QHostAddress)));

    income->start();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Server::addHost(QHostAddress ip)
{
    hostList->append(ip);
}

void Server::remHost(QHostAddress ip)
{
    hostList->removeAll(ip);
}
// *************************************************************************************************
