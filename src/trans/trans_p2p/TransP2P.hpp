#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../ITrans.hpp"
#include "../../view/IView.hpp"

class Server;
class Client;

class TransP2P : public ITrans, public IViewListener,
    public std::enable_shared_from_this<ITrans>
{
public:
    TransP2P();
    ~TransP2P();

    // --- ITrans ----------------------------
    virtual bool init(IViewPtr viewPtr, IConfPtr confPtr);
    virtual bool exec(int argc, char *argv[]);
    // ---------------------------------------

     // --- IViewListener --------------------
    virtual void onClickedSend(std::string str);
    // ---------------------------------------

private:
    IViewPtr m_viewPtr;
    IConfPtr m_confPtr;

    Server *m_serPtr;
    Client *m_cliPtr;
};
