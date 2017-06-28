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

    std::fstream *filePtr;

    enum States
    {
        start, equals, name, number, text, error
    } m_state;

    struct data
    {
        enum types{text, number};
        types type;
        std::string value;
    };

    std::string m_tmpName;
    std::string m_tmpValue;
    std::map<std::string, data*> m_pData;
};
