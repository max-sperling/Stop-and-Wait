/************************/
/* Author: Max Sperling */
/************************/

#include "ConfJson.hh"

#include <fstream>
#include "JsonParser.hh"

using namespace std;

IConfPtr IConf::create()
{
    return IConfPtr(new ConfJson());
}

bool ConfJson::init(int argc, char *argv[])
{
    if(argc != 2) return false;
    this->confFile = argv[1];
    return true;
}

bool ConfJson::read(string &addr, unsigned int &port)
{
    JsonParser parser;

    try {
        parser.parseFile(confFile);
        parser.getValStr("Addr", addr);
        parser.getValInt("Port", port);
    }
    catch(...) {
        return false;
    }

    return true;
}
