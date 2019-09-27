/************************/
/* Author: Max Sperling */
/************************/

#include "TransFactory.hpp"
#include "trans_p2p/TransP2P.hpp"

using namespace std;

ITransPtr TransFactory::create(TransType type)
{
    ITransPtr transPtr;
    switch(type)
    {
    case TransType::P2P:
        transPtr = make_shared<TransP2P>();
        break;
    }
    return transPtr;
}