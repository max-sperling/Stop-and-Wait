/************************/
/* Author: Max Sperling */
/************************/

#include "TransP2P.hh"

#include <QApplication>
#include <iostream>
#include "Server.hh"
#include "Client.hh"

using namespace std;

ITransPtr ITrans::create()
{
    return ITransPtr(new TransP2P());
}

bool TransP2P::init(IViewPtr viewPtr, IConfPtr confPtr)
{
    m_viewPtr = viewPtr;
    m_confPtr = confPtr;

    return true;
}

bool TransP2P::exec(int argc, char *argv[])
{
    QApplication app(argc, argv);

    m_viewPtr->start(shared_from_this());
    m_viewPtr->attach(this);

    string addr;
    unsigned int port;

    if(!m_confPtr->read(addr, port))
    {
        m_viewPtr->logIt("Error while reading Config");
        return false;
    }

    m_serPtr = new Server(m_viewPtr);
    if(!m_serPtr->init(port))
    {
        m_viewPtr->logIt("Error while init Server");
        return false;
    }

    m_cliPtr = new Client(m_viewPtr);
    if(!m_cliPtr->init(addr, port))
    {
        m_viewPtr->logIt("Error while init Client");
        return false;
    }

    app.exec();
    
    // delete cliPtr;
    // delete serPtr;

    return true;
}

void TransP2P::onClickedSend(std::string str)
{
    m_cliPtr->sendFile(str);
}
