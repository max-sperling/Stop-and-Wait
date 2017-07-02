#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "IViewFwd.hh"

#include <string>
#include <deque>
#include "../Trans/ITransFwd.hh"

class IView
{
public:
    static IViewPtr create();
    virtual ~IView(){}

    virtual bool start(ITransPtr transPtr) = 0;
    virtual void logIt(std::string str) = 0;
    virtual bool attach(IViewListener *lis) = 0;
    virtual bool detach(IViewListener *lis) = 0;
};

class IViewListener
{
public:
    virtual void onClickedSend(std::string str) = 0;
};
