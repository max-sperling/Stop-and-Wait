#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QString>
#include <QHostAddress>
#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    bool init(unsigned int port);
    QList<QHostAddress> *getHosts();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<QHostAddress> *hostList;

private slots:
    void addHost(QHostAddress ip);
    void remHost(QHostAddress ip);
};
