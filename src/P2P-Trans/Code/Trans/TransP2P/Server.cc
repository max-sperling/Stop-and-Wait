/************************/
/* Author: Max Sperling */
/************************/

#include "Server.hh"

#include <QDebug>
#include "Income.hh"

// ***** Public ************************************************************************************
Server::Server(IViewPtr viewPtr)
{
    //qRegisterMetaType<QHostAddress>("QHostAddress");
    clientList = new QList<QHostAddress>();
    this->viewPtr = viewPtr;
}

bool Server::init(unsigned int port)
{
    if(!this->listen(QHostAddress::Any, port))
        return false;

    return true;
}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Server::incomingConnection(qintptr socketDescriptor)
{
    Income *income = new Income(socketDescriptor);

    connect(income, SIGNAL(addHost(QHostAddress)), this, SLOT(addHost(QHostAddress)));
    connect(income, SIGNAL(remHost(QHostAddress)), this, SLOT(remHost(QHostAddress)));

    income->start();
}
// *************************************************************************************************

// ***** Slots *************************************************************************************
void Server::addHost(QHostAddress ip)
{
    clientList->append(ip);
}

void Server::remHost(QHostAddress ip)
{
    clientList->removeAll(ip);
}
// *************************************************************************************************
