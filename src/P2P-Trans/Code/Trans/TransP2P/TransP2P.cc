/************************/
/* Author: Max Sperling */
/************************/

#include "TransP2P.hh"

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
    if(!confPtr->init(argc, argv))
    {
        viewPtr->write("Usage: program <conf-File>");
        return false;
    }

    string remoteIp;
    unsigned int remoteServerPort;
    unsigned int localServerPort;
    unsigned int localClientPort;

    if(!confPtr->read(remoteIp, remoteServerPort,
        localServerPort, localClientPort))
    {
        viewPtr->write("Error while reading Config");
        return false;
    }

    // tokPtr = new Tokenizer;
    // genPtr = new Generator;

    // deque<Token> token;
    // try{
    //     tokPtr->exec(srcCode, token);
    // }
    // catch(...){
    //     viewPtr->write("Error while Tokenization");
    //     return false;
    // }

    // deque<char> binary;
    // try{
    //     genPtr->exec(token, binary);
    // }catch(CompEx &cex){
    //     viewPtr->write("Error while Generating");
    //     viewPtr->write(cex.getError());
    //     return false;
    // }

    // viewPtr->write(binary);

    // if(!dataPtr->write(binary))
    // {
    //     viewPtr->write("Error while writing File");
    //     return false;
    // }

    // delete tokPtr;
    // delete genPtr;

    return true;
}
