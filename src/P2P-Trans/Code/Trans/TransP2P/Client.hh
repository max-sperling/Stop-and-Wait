#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QString>
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

    IViewPtr m_viewPtr;
    QTcpSocket *m_socket;
    QString m_addr;
    quint16 m_port;

private slots:
    void onDisconnected();
};
