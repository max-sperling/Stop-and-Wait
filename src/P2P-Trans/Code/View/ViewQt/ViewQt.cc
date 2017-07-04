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
    m_listeners = new vector<IViewListener*>();
}

ViewQt::~ViewQt()
{
    delete m_lstLog;
    delete m_lytWin;
}

bool ViewQt::start(ITransPtr transPtr)
{
    m_transPtr = transPtr;

    m_widWin = new QWidget();
    m_lytWin = new QGridLayout();
    m_lstLog = new QListWidget();
    m_btnSend = new QPushButton("Send");

    connect(m_btnSend, SIGNAL(pressed()), this, SLOT(onClickedSend()));

    m_lytWin->addWidget(m_lstLog);
    m_lytWin->addWidget(m_btnSend);
    m_widWin->setLayout(m_lytWin);
    m_widWin->show();

    return true;
}

void ViewQt::logIt(string str)
{
    std::cout << str << std::endl;
    m_lstLog->addItem(QString::fromStdString(str));
}

bool ViewQt::attach(IViewListener *lis)
{
    auto iter = std::find(m_listeners->begin(), m_listeners->end(), lis);
    if (iter != m_listeners->end()) return false;

    m_listeners->push_back(lis);
    return true;
}

bool ViewQt::detach(IViewListener *lis)
{
    auto iter = std::find(m_listeners->begin(), m_listeners->end(), lis);
    if (iter == m_listeners->end()) return false;

    m_listeners->erase(iter);
    return true;
}

void ViewQt::onClickedSend()
{
    QString fileName = QFileDialog::getOpenFileName(m_widWin, "Send", QDir::homePath());

    if(fileName == "") return;

    for(IViewListener *lis : *m_listeners)
    {
        lis->onClickedSend(fileName.toStdString());
    }
}
