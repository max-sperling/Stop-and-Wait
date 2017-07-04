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
    Income(IViewPtr viewPtr, qintptr socketId);

protected:
    void run();

private:
    IViewPtr m_viewPtr;
    QTcpSocket *m_socket;
    qintptr m_socketId;
    QFile m_file;

signals:
    void error(QTcpSocket::SocketError socketError);
    void addHost(QHostAddress ip);
    void remHost(QHostAddress ip);

private slots:
    void onGetTCPStream();
    void onDisconnected();
};
