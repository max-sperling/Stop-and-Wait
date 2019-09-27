#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IView.hpp"

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
    virtual bool start();
    virtual void logIt(std::string str);
    virtual bool attach(IViewListener *lis);
    virtual bool detach(IViewListener *lis);
    // ---------------------------------------------

private:
    std::weak_ptr<ITrans> m_transPtr;
    std::vector<IViewListener*> *m_listeners;

    QWidget *m_widWin;
    QGridLayout *m_lytWin;
    QListWidget *m_lstLog;
    QPushButton *m_btnSend;

private slots:
    void onClickedSend();
};
