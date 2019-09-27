/************************/
/* Author: Max Sperling */
/************************/

#include "./view/ViewFactory.hpp"
#include "./conf/ConfFactory.hpp"
#include "./trans/TransFactory.hpp"

int main(int argc, char *argv[])
{
    IViewPtr viewPtr = ViewFactory::create(ViewType::Qt);
    IConfPtr confPtr = ConfFactory::create(ConfType::Json);
    ITransPtr transPtr = TransFactory::create(TransType::P2P);

    if(!transPtr->init(viewPtr, confPtr)) return 1;
    if(!transPtr->exec(argc, argv)) return 2;

    return 0;
}
