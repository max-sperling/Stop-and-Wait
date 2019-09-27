#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "ITrans.hpp"

enum TransType
{
    P2P
};

class TransFactory
{
public:
    static ITransPtr create(TransType type);
};