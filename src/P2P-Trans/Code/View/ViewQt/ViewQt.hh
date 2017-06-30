#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IView.hh"

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <vector>

class ViewQt : public IView
{
public:
    ViewQt();
    ~ViewQt();

    // --- IView -----------------------------------
    virtual bool init();
    virtual void logIt(std::string str);
    virtual bool attach(IViewListener *lis);
    virtual bool detach(IViewListener *lis);
    // ---------------------------------------------

private:
    void clickedSendFile(std::string file);

    QWidget *widWin;
    QGridLayout *lytWin;
    QListWidget *lstLog;

    std::vector<IViewListener*> *listeners;
};
