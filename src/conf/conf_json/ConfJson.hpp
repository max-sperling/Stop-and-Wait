#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IConf.hpp"

class ConfJson : public IConf
{
public:
    ConfJson();
    ~ConfJson();

    // --- IData -----------------------------
    virtual bool read(std::string &addr, unsigned int &port);
    // ---------------------------------------
};
