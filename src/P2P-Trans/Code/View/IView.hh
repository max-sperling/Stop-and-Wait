#pragma once
/************************/
/* Author: Max Sperling */
/************************/


class IView;
typedef std::shared_ptr<IView> IViewPtr;

class IView
{
public:
    static IViewPtr create();
    virtual ~IView(){}

    //virtual void write(std::string str) = 0;
};
