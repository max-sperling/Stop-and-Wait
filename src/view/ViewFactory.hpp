#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "IView.hpp"

enum ViewType
{
    Qt
};

class ViewFactory
{
public:
    static IViewPtr create(ViewType type);
};