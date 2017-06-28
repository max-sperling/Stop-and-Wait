/************************/
/* Author: Max Sperling */
/************************/

#include "./View/IView.hh"
#include "./Data/IData.hh"
#include "./Trans/ITrans.hh"

using namespace std;

int main(int argc, char *argv[])
{
    IViewPtr viewPtr = IView::create();
    IDataPtr dataPtr = IData::create();
    ITransPtr compPtr = ITran::create();

    //if(!compPtr->init(viewPtr, dataPtr)) return 1;
    //if(!compPtr->exec(argc, argv)) return 2;

    return 0;
}
