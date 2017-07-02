#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "ITransFwd.hh"

#include "../View/IViewFwd.hh"
#include "../Conf/IConf.hh"

class ITrans
{
public:
    static ITransPtr create();
    virtual ~ITrans(){}

    virtual bool init(IViewPtr viewPtr, IConfPtr confPtr) = 0;
    virtual bool exec(int argc, char *argv[]) = 0;
};
