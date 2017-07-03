#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <memory>
#include <string>
#include <deque>

class IConf;
typedef std::shared_ptr<IConf> IConfPtr;

class IConf
{
public:
    static IConfPtr create();
    virtual ~IConf(){}

    virtual bool read(std::string &addr, unsigned int &port) = 0;
};
