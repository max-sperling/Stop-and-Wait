#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <string>
#include <sstream>
#include <iomanip>
#include <array>

class Packet
{
public:
    enum Type
    {
        Meta, Content
    };

    static std::array<unsigned char, 4> intToBytes(int value)
    {
        std::array<unsigned char, 4> result;
        result.at(0) = (value >> 24) & 0xFF;
        result.at(1) = (value >> 16) & 0xFF;
        result.at(2) = (value >>  8) & 0xFF;
        result.at(3) = (value      ) & 0xFF;
        return result;
    }

    static Packet *create(std::string data);

    virtual std::string getData() = 0;
    virtual Type getType() = 0;

private:
    unsigned int size;
    Type type;
};

class MetaPacket : public Packet
{
public:
    MetaPacket(std::string baseName)
    {
        this->baseName = baseName;
    }

    // --- Packet ----------
    virtual std::string getData()
    {
        std::array<unsigned char, 4> len = intToBytes(1+baseName.length());
        std::string data = std::string(std::begin(len), std::end(len)) + std::to_string(Packet::Meta) + baseName;
        return data;
    }
    virtual Type getType()
    {
        return Packet::Meta;
    }
    // ---------------------

private:
    std::string baseName;
};

class ContentPacket : public Packet
{
public:
    ContentPacket(std::string content)
    {
        this->content = content;
    }

    // --- Packet ----------
    virtual std::string getData()
    {
        std::array<unsigned char, 4> len = intToBytes(1+content.length());
        std::string data = std::string(std::begin(len), std::end(len)) + std::to_string(Packet::Content) + content;
        return data;
    }
    virtual Type getType()
    {
        return Packet::Content;
    }
    // ---------------------

private:
    std::string content;
};