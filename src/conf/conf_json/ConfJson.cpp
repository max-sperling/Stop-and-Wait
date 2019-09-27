/************************/
/* Author: Max Sperling */
/************************/

#include "ConfJson.hpp"

#include <fstream>
#include "JsonParser.hpp"

using namespace std;

ConfJson::ConfJson()
{

}

ConfJson::~ConfJson()
{
    
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
