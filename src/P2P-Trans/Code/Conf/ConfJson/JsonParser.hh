#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <string>
#include <map>
#include <fstream>

class JsonParser
{
public:
    JsonParser();
    ~JsonParser();

    void parseFile(std::string file);
    void getValStr(std::string name, std::string &value);
    void getValInt(std::string name, unsigned int &value);

private:
    void resetParser();
    void changeState(char input);

    enum States
    {
        start, equals, name, number, text, error
    };

    struct Data
    {
        enum Types{text, number};
        Types type;
        std::string value;
    };

    std::fstream *filePtr;
    States state;
    std::string tmpName;
    std::string tmpValue;
    std::map<std::string, Data*> dataPtr;
};
