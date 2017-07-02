#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <string>
#include <sstream>
#include <iomanip>

class Packet
{
public:
    enum Type
    {
        Meta, Content
    };

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
        std::stringstream ss;
        ss << std::setw(sizeof(int)) << std::setfill('0') << 1+baseName.length();
        std::string data = ss.str() + std::to_string(Packet::Meta) + baseName;
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
        std::stringstream ss;
        ss << std::setw(sizeof(int)) << std::setfill('0') << 1+content.length();
        std::string data = ss.str() + std::to_string(Packet::Content) + content;
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