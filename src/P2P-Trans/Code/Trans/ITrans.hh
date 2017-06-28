#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <memory>
#include "../View/IView.hh"
#include "../Conf/IConf.hh"

class ITrans;
typedef std::shared_ptr<ITrans> ITransPtr;

class ITrans
{
public:
    static ITransPtr create();
    virtual ~ITrans(){}

    virtual bool init(IViewPtr viewPtr, IConfPtr confPtr) = 0;
    virtual bool exec(int argc, char *argv[]) = 0;
};
