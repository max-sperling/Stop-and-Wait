#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <QString>
#include <QTcpServer>
#include "../../View/IView.hpp"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(IViewPtr viewPtr);
    bool init(unsigned int port);

protected:
    void incomingConnection(qintptr socketId);

private:
	IViewPtr m_viewPtr;
};
