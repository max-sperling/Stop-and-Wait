#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "IViewFwd.hpp"

#include <string>
#include <deque>
#include "../trans/ITransFwd.hpp"

class IView
{
public:
    virtual ~IView(){}

    virtual bool start() = 0;
    virtual void logIt(std::string str) = 0;
    virtual bool attach(IViewListener *lis) = 0;
    virtual bool detach(IViewListener *lis) = 0;
};

class IViewListener
{
public:
    virtual void onClickedSend(std::string str) = 0;
};
