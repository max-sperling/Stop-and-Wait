/************************/
/* Author: Max Sperling */
/************************/

#include "ViewQt.hh"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <QFileDialog>
#include "../../Trans/ITrans.hh"

using namespace std;

IViewPtr IView::create()
{
    return IViewPtr(new ViewQt());
}

ViewQt::ViewQt()
{
    listeners = new vector<IViewListener*>();
}

ViewQt::~ViewQt()
{
    delete lstLog;
    delete lytWin;
}

bool ViewQt::start(ITransPtr transPtr)
{
    this->transPtr = transPtr;

    widWin = new QWidget();
    lytWin = new QGridLayout();
    lstLog = new QListWidget();
    btnSend = new QPushButton("Send");

    connect(btnSend, SIGNAL(pressed()), this, SLOT(onClickedSend()));

    lytWin->addWidget(lstLog);
    lytWin->addWidget(btnSend);
    widWin->setLayout(lytWin);
    widWin->show();

    return true;
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

void ViewQt::onClickedSend()
{
    QString fileName = QFileDialog::getOpenFileName(widWin, "Send", QDir::homePath());

    if(fileName == "") return;

    for(IViewListener *lis : *listeners)
    {
        lis->onClickedSend(fileName.toStdString());
    }
}
