#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IView.hh"

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <vector>

class ViewQt : public QObject, public IView
{
    Q_OBJECT

public:
    ViewQt();
    ~ViewQt();

    // --- IView -----------------------------------
    virtual bool start(ITransPtr transPtr);
    virtual void logIt(std::string str);
    virtual bool attach(IViewListener *lis);
    virtual bool detach(IViewListener *lis);
    // ---------------------------------------------

private:
    ITransPtr transPtr;

    QWidget *widWin;
    QGridLayout *lytWin;
    QListWidget *lstLog;
    QPushButton *btnSend;

    std::vector<IViewListener*> *listeners;

private slots:
    void onClickedSend();
};
