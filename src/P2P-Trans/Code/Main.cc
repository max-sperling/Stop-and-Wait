/************************/
/* Author: Max Sperling */
/************************/

#include "./View/IView.hh"
#include "./Conf/IConf.hh"
#include "./Trans/ITrans.hh"

using namespace std;

int main(int argc, char *argv[])
{
    IViewPtr viewPtr = IView::create();
    IConfPtr confPtr = IConf::create();
    ITransPtr transPtr = ITrans::create();

    if(!transPtr->init(viewPtr, confPtr)) return 1;
    if(!transPtr->exec(argc, argv)) return 2;

    return 0;
}
