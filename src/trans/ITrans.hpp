#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "ITransFwd.hpp"

#include "../view/IViewFwd.hpp"
#include "../conf/IConf.hpp"

class ITrans
{
public:
    virtual ~ITrans(){}

    virtual bool init(IViewPtr viewPtr, IConfPtr confPtr) = 0;
    virtual bool exec(int argc, char *argv[]) = 0;
};
