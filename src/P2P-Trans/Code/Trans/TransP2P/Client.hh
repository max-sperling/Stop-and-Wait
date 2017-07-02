#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>
#include <string>
#include "../../View/IView.hh"

class Client : public QObject
{
    Q_OBJECT

public:
    Client(IViewPtr viewPtr);
    ~Client();
    bool init(std::string addr, unsigned int port);
    bool sendFile(std::string fileName);

private:
    bool connectToServer();

    IViewPtr viewPtr;

    QThread *thread;
    QTcpSocket *socket;
    QString addr;
    quint16 port;

private slots:
    void onStartedThread();
    void onDisconnected();
};
