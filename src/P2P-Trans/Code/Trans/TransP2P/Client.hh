#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QThread>
#include <QHostAddress>
#include <QTcpSocket>
#include <string>

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    bool init(std::string addr, unsigned int port);

private:
    bool connectToServer();
    void waitForInput();
    bool sendTCPStream(QByteArray data);

    QThread *thread;
    QTcpSocket *socket;
    QString addr;
    quint16 port;

private slots:
    void onStartedThread();
    void onGetTCPStream();
    void onDisconnected();
};
