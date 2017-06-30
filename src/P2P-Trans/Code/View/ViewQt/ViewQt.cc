/************************/
/* Author: Max Sperling */
/************************/

#include "ViewQt.hh"

#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

IViewPtr IView::create()
{
    return IViewPtr(new ViewQt());
}

ViewQt::ViewQt()
{
}

ViewQt::~ViewQt()
{
    delete lstLog;
    delete lytWin;
}

bool ViewQt::init()
{
    widWin = new QWidget();
	lytWin = new QGridLayout();
    lstLog = new QListWidget();

    lytWin->addWidget(lstLog);
    widWin->setLayout(lytWin);
    widWin->show();
}

void ViewQt::logIt(string str)
{
    std::cout << str << std::endl;
    lstLog->addItem(QString::fromStdString(str));
}

bool ViewQt::attach(IViewListener *lis)
{
    auto iter = std::find(listeners->begin(), listeners->end(), lis);
    if (iter != listeners->end()) return false;

    listeners->push_back(lis);
    return true;
}

bool ViewQt::detach(IViewListener *lis)
{
    auto iter = std::find(listeners->begin(), listeners->end(), lis);
    if (iter == listeners->end()) return false;

    listeners->erase(iter);
    return true;
}

void ViewQt::clickedSendFile(string file)
{
    for(IViewListener *lis : *listeners)
    {
        lis->onClickedSendFile(file);
    }
}
