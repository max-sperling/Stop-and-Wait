/************************/
/* Author: Max Sperling */
/************************/

#include "Server.hh"

#include <QDebug>
#include "Income.hh"

// ***** Public ************************************************************************************
Server::Server(IViewPtr viewPtr)
{
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
    Income *income = new Income(viewPtr, socketDescriptor);
    income->start();
}
// *************************************************************************************************
