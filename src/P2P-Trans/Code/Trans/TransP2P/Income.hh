#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>
#include <QFile>
#include "../../View/IView.hh"

class Server;

class Income : public QThread
{
    Q_OBJECT

public:
    Income(IViewPtr viewPtr, qintptr socketDescriptor);

protected:
    void run();

private:
    IViewPtr viewPtr;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QFile file;

signals:
    void error(QTcpSocket::SocketError socketError);
    void addHost(QHostAddress ip);
    void remHost(QHostAddress ip);

private slots:
    void onGetTCPStream();
    void onDisconnected();
};
