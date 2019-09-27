#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "IConf.hpp"

enum ConfType
{
    Json
};

class ConfFactory
{
public:
    static IConfPtr create(ConfType type);
};