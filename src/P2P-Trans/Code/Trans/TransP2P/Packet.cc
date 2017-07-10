/************************/
/* Author: Max Sperling */
/************************/

#include "Packet.hh"

using namespace std;

array<char, 4> Packet::intToBytes(unsigned int value)
{
    array<char, 4> result;
    result.at(0) = (value >> 24) & 0xFF;
    result.at(1) = (value >> 16) & 0xFF;
    result.at(2) = (value >>  8) & 0xFF;
    result.at(3) = (value      ) & 0xFF;
    return result;
}

unsigned int Packet::byteArrayToInt(array<char, 4> value)
{
    unsigned int result = 0;
    result +=  (value[0] << 24);
    result += ((value[1] & 0xFF) << 16);
    result += ((value[2] & 0xFF) << 8);
    result +=  (value[3] & 0xFF);
    return result;
}

Packet::Packet(Type type, string data)
{
    m_type = type;
    m_data = data;
    m_size = s_lenType + data.length();
}

Packet::Packet(unsigned int size, string raw)
{
    m_size = size;
    m_type = (Type)stoi(raw.substr(0, s_lenType));
    m_data = raw.substr(s_lenType, raw.length()-s_lenType);
}

string Packet::getRaw()
{
    array<char, s_lenSize> size = intToBytes(m_size);
    return string(begin(size), end(size)) + to_string(m_type) + m_data;
}

Packet::Type Packet::getType()
{
    return m_type;
}

string Packet::getData()
{
    return m_data;
}