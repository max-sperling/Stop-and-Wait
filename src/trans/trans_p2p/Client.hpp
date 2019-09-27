#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QObject>
#include <QString>
#include <string>
#include "../../view/IView.hpp"

class Client : public QObject
{
    Q_OBJECT

public:
    Client(IViewPtr viewPtr);
    ~Client();

    bool init(std::string addr, unsigned int port);
    bool sendFile(std::string fileName);

private:
    IViewPtr m_viewPtr;
    std::string m_addr;
    unsigned int m_port;
};
