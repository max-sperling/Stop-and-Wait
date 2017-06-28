#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../ITrans.hh"

class Server;
class Client;

class TransP2P : public ITrans
{
public:
    TransP2P(){}
    ~TransP2P(){}

    // --- ITrans ----------------------------
    virtual bool init(IViewPtr viewPtr, IConfPtr confPtr);
    virtual bool exec(int argc, char *argv[]);
    // ---------------------------------------

private:
    IViewPtr viewPtr;
    IConfPtr confPtr;

    Server *serPtr;
    Client *cliPtr;
};
