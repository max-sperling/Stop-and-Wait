/************************/
/* Author: Max Sperling */
/************************/

#include "ConfFile.hh"

#include <fstream>
#include "JsonParser.hh"

using namespace std;

IConfPtr IConf::create()
{
    return IConfPtr(new ConfFile());
}

bool ConfFile::init(int argc, char *argv[])
{
    if(argc != 2) return false;
    this->confFile = argv[1];
    return true;
}

bool ConfFile::read(string &remoteIp, unsigned int &remoteServerPort,
    unsigned int &localServerPort, unsigned int &localClientPort)
{
    JsonParser parser;

    try {
        parser.parseFile(confFile);
        parser.getValStr("remoteIp", remoteIp);
        parser.getValInt("remoteServerPort", remoteServerPort);
        parser.getValInt("localServerPort", localServerPort);
        parser.getValInt("localClientPort", localClientPort);
    }
    catch(...) {
        return false;
    }

    return true;
}
