/************************/
/* Author: Max Sperling */
/************************/

#include "TransP2P.hh"

#include <QCoreApplication>
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
    QCoreApplication app(argc, argv);

    if(!confPtr->init(argc, argv))
    {
        viewPtr->write("Usage: program <conf-File>");
        return false;
    }

    string addr;
    unsigned int port;

    if(!confPtr->read(addr, port))
    {
        viewPtr->write("Error while reading Config");
        return false;
    }

    serPtr = new Server();
    if(!serPtr->init(port))
    {
        viewPtr->write("Error while init Server");
        return false;
    }

    cliPtr = new Client();
    if(!cliPtr->init(addr, port))
    {
        viewPtr->write("Error while init Client");
        return false;
    }

    app.exec();
    
    // delete cliPtr;
    // delete serPtr;

    return true;
}
