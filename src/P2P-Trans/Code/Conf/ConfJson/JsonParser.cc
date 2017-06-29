/************************/
/* Author: Max Sperling */
/************************/

#include "JsonParser.hh"

#include <exception>

using namespace std;

JsonParser::JsonParser()
{
    filePtr = new fstream();
    resetParser();
}

JsonParser::~JsonParser()
{
    resetParser();
    delete filePtr;
}

void JsonParser::parseFile(string file)
{
    resetParser();

    filePtr->open(file);
    if (!filePtr->is_open()) throw exception();

    char c;
    while (!filePtr->eof())
    {
        filePtr->get(c);
        changeState(c);
    }
    filePtr->close();
}

void JsonParser::getValStr(string name, string &value)
{
    auto iter = dataPtr.find(name);
    if (iter == dataPtr.end()) throw exception();
    if (iter->second->type != Data::Types::text) throw exception();
    value = iter->second->value;
}

void JsonParser::getValInt(string name, unsigned int &value)
{
    auto iter = dataPtr.find(name);
    if (iter == dataPtr.end()) throw exception();
    if (iter->second->type != Data::Types::number) throw exception();
    value = stoi(iter->second->value);
}

void JsonParser::resetParser()
{
    for (auto it = dataPtr.begin(); it != dataPtr.end(); ++it)
    {
        delete it->second;
    }
    dataPtr.clear();

    state = States::start;
    tmpName = "";
    tmpValue = "";
}

void JsonParser::changeState(char input)
{
    switch (input)
    {
    case '\r':
    case '\n':
    case '\t':
    case ' ':
    case '{':
    case ':':
        break;
    case '"':
        switch (state)
        {
        case States::start:
            state = States::name;
            break;
        case States::equals:
            state = States::text;
            break;
        case States::name:
            state = States::equals;
            break;
        case States::text:
            Data* pData = new Data();
            pData->type = Data::Types::text;
            pData->value = tmpValue;
            dataPtr.insert(pair<string, Data*>(tmpName, pData));
            tmpName = "";
            tmpValue = "";
            state = States::start;
            break;
        }
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        switch (state)
        {
        case States::name:
            tmpName += input;
            break;
        case  States::equals:
            state = States::number;
        case States::text:
        case States::number:
            tmpValue += input;
            break;
        }
        break;
    case '}':
    case ',':
        switch (state)
        {
        case States::number:
            Data* pData = new Data();
            pData->type = Data::Types::number;
            pData->value = tmpValue;
            dataPtr.insert(pair<string, Data*>(tmpName, pData));
            tmpName = "";
            tmpValue = "";
            state = States::start;
        }
        break;
    default:
        switch (state)
        {
        case States::name:
            tmpName += input;
            break;
        case States::text:
            tmpValue += input;
            break;
        }
        break;
    }
}
