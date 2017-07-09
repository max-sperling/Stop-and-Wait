/************************/
/* Author: Max Sperling */
/************************/

#include "Server.hh"

#include "Income.hh"

// ***** Public ************************************************************************************
Server::Server(IViewPtr viewPtr)
{
    m_viewPtr = viewPtr;
}

bool Server::init(unsigned int port)
{
    if(!listen(QHostAddress::Any, port))
        return false;

    return true;
}
// *************************************************************************************************

// ***** Protected *********************************************************************************
void Server::incomingConnection(qintptr socketId)
{
    Income *income = new Income(m_viewPtr, socketId);
    connect(income, SIGNAL(finished()), income, SLOT(deleteLater()));
    //moveToThread(income);
    income->start();
}
// *************************************************************************************************
