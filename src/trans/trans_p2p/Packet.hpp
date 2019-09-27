#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <string>
#include <array>

class Packet
{
public:
    static const unsigned int s_maxSize = 50000000;
    static const unsigned int s_lenSize = sizeof(int);
    static const unsigned int s_lenType = sizeof(char);

    enum Type {
        Meta, Content
    };

    static std::array<char, 4> intToBytes(unsigned int value);
    static unsigned int byteArrayToInt(std::array<char, 4> value);

    Packet(Type type, std::string data);
    Packet(unsigned int size, std::string raw);

    std::string getRaw();
    Type getType();
    std::string getData();

private:
    unsigned int m_size;
    Type m_type;
    std::string m_data;
};
