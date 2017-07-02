/************************/
/* Author: Max Sperling */
/************************/

#include "Packet.hh"

using namespace std;

Packet *Packet::create(string data)
{
    switch((Type)stoi(data,0))
    {
    case Meta:
        {
            string baseName = data.substr(1);
            return new MetaPacket(baseName);
        }
        break;
    case Content:
    	{
        	string content = data.substr(1);
        	return new ContentPacket(content);
    	}
        break;
    default:
    	return nullptr;
        break;
    }
}