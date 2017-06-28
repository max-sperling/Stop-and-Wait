/************************/
/* Author: Max Sperling */
/************************/

#include "DataFile.hh"

#include <fstream>

using namespace std;

IDataPtr IData::create()
{
    return IDataPtr(new DataFile());
}

bool DataFile::init(int argc, char *argv[])
{
    if(argc != 2) return false;
    this->confFile = argv[1];
    return true;
}

bool DataFile::readConf(Addr &server, Addr &client)
{
    ifstream ifs(confFile);
    if(ifs.fail()) return false;
    string str = {istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};
    ifs.close();
    //JSON-Parser
    return true;
}

bool DataFile::readPacket(string name, deque<char> &bin)
{

}

bool DataFile::writePacket(string name, deque<char> &bin)
{

}
