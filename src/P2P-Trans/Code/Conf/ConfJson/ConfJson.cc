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

bool ConfJson::read(string &addr, unsigned int &port)
{
    JsonParser parser;

    try {
        parser.parseFile("config.json");
        parser.getValStr("Addr", addr);
        parser.getValInt("Port", port);
    }
    catch(...) {
        return false;
    }

    return true;
}
