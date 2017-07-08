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
    ~Income();

protected:
    void run();

private:
    IViewPtr m_viewPtr;

    QTcpSocket *m_socket;
    qintptr m_socketId;
    QFile m_file;

    unsigned int m_size;
    QByteArray m_data;

private slots:
    void onGetTCPStream();
    void onDisconnected();
};
