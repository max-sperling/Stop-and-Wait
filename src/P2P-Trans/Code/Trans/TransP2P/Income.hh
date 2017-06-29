#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>

class Server;

class Income : public QThread
{
    Q_OBJECT

public:
    Income(qintptr Id, QObject *parent = 0);

protected:
    void run();

private:
    Server *server;

    QTcpSocket *tcpSocket;
    qintptr socketDescriptor;

signals:
    void error(QTcpSocket::SocketError socketError);
    void addHost(QHostAddress ip);
    void remHost(QHostAddress ip);

private slots:
    void onGetTCPStream();
    void onDisconnected();
};
