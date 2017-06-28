#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include "../IData.hh"

class DataFile : public IData
{
public:
    DataFile(){}
    ~DataFile(){}

    // --- IData -----------------------------
    virtual bool open(std:string file);
    virtual bool read(std::deque<char> &bin);
    virtual bool write(std::deque<char> bin);
    virtual bool close();
    // ---------------------------------------

private:
    std::string dataFile;
};
