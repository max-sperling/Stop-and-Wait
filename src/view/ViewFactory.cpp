/************************/
/* Author: Max Sperling */
/************************/

#include "ViewFactory.hpp"
#include "view_qt/ViewQt.hpp"

using namespace std;

IViewPtr ViewFactory::create(ViewType type)
{
    IViewPtr viewPtr;
    switch(type)
    {
    case ViewType::Qt:
        viewPtr = make_shared<ViewQt>();
        break;
    }
    return viewPtr;
}