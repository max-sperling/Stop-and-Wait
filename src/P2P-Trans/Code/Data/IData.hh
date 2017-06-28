#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <memory>
#include <string>
#include <deque>

class IData;
typedef std::shared_ptr<IData> IDataPtr;

class IData
{
public:
    static IDataPtr create();
    virtual ~IData(){}

    struct Addr
    {
        std::string ip;
        unsigned int port;
    };

    virtual bool open(std:string file) = 0;
    virtual bool read(std::deque<char> &bin) = 0;
    virtual bool write(std::deque<char> bin) = 0;
    virtual bool close() = 0;
};
