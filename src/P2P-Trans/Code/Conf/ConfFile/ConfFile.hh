#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IConf.hh"

class ConfFile : public IConf
{
public:
    ConfFile(){}
    ~ConfFile(){}

    // --- IData -----------------------------
    virtual bool init(int argc, char *argv[]);
    virtual bool read(std::string &remoteIp, unsigned int &remoteServerPort,
        unsigned int &localServerPort, unsigned int &localClientPort);
    // ---------------------------------------

private:
    std::string confFile;
};
