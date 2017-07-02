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
    this->viewPtr = viewPtr;
    this->confPtr = confPtr;

    return true;
}

bool TransP2P::exec(int argc, char *argv[])
{
    QApplication app(argc, argv);

    viewPtr->start(shared_from_this());
    viewPtr->attach(this);

    if(!confPtr->init(argc, argv))
    {
        viewPtr->logIt("Usage: program <conf-File>");
        return false;
    }

    string addr;
    unsigned int port;

    if(!confPtr->read(addr, port))
    {
        viewPtr->logIt("Error while reading Config");
        return false;
    }

    serPtr = new Server(viewPtr);
    if(!serPtr->init(port))
    {
        viewPtr->logIt("Error while init Server");
        return false;
    }

    cliPtr = new Client(viewPtr);
    if(!cliPtr->init(addr, port))
    {
        viewPtr->logIt("Error while init Client");
        return false;
    }

    app.exec();
    
    // delete cliPtr;
    // delete serPtr;

    return true;
}

void TransP2P::onClickedSend(std::string str)
{
    cliPtr->sendFile(str);
}
