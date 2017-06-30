#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <memory>
#include <string>
#include <deque>

class IView;
typedef std::shared_ptr<IView> IViewPtr;
class IViewListener;

class IView
{
public:
    static IViewPtr create();
    virtual ~IView(){}

    virtual bool init() = 0;
    virtual void logIt(std::string str) = 0;
    virtual bool attach(IViewListener *lis) = 0;
    virtual bool detach(IViewListener *lis) = 0;
};

class IViewListener
{
public:
    virtual void onClickedSendFile(std::string str) = 0;
};
