/************************/
/* Author: Max Sperling */
/************************/

#include "Client.hh"

#include "Outcome.hh"

using namespace std;

// ***** Public ************************************************************************************
Client::Client(IViewPtr viewPtr)
{
    m_viewPtr = viewPtr;
}

Client::~Client()
{

}

bool Client::init(string addr, unsigned int port)
{
    m_addr = addr;
    m_port = port;

    return true;
}

bool Client::sendFile(std::string fileName)
{
    Outcome *outcome = new Outcome(m_viewPtr, m_addr, m_port, fileName);
    connect(outcome, SIGNAL(finished()), outcome, SLOT(deleteLater()));
    //moveToThread(outcome);
    outcome->start();

    return true;
}
// *************************************************************************************************
