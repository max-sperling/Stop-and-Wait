#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "IView.hpp"

enum ViewType
{
    Qute
};

class ViewFactory
{
public:
    static IViewPtr create(ViewType type);
};
