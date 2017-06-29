#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IConf.hh"

class ConfJson : public IConf
{
public:
    ConfJson(){}
    ~ConfJson(){}

    // --- IData -----------------------------
    virtual bool init(int argc, char *argv[]);
    virtual bool read(std::string &addr, unsigned int &port);
    // ---------------------------------------

private:
    std::string confFile;
};