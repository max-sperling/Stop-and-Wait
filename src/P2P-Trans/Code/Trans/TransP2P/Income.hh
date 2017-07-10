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
    std::string m_fileName;
    std::string m_logIdent;

    unsigned int m_blockSize;
    QByteArray m_blockData;

private slots:
    void onGetTCPStream();
    void onDisconnected();
};
