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
    auto iter = m_pData.find(name);
    if (iter == m_pData.end()) throw exception();
    if (iter->second->type != data::types::text) throw exception();
    value = iter->second->value;
}

void JsonParser::getValInt(string name, unsigned int &value)
{
    auto iter = m_pData.find(name);
    if (iter == m_pData.end()) throw exception();
    if (iter->second->type != data::types::number) throw exception();
    value = stoi(iter->second->value);
}

void JsonParser::resetParser()
{
    for (auto it = m_pData.begin(); it != m_pData.end(); ++it)
    {
        delete it->second;
    }
    m_pData.clear();

    m_state = States::start;
    m_tmpName = "";
    m_tmpValue = "";
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
        switch (m_state)
        {
        case States::start:
            m_state = States::name;
            break;
        case States::equals:
            m_state = States::text;
            break;
        case States::name:
            m_state = States::equals;
            break;
        case States::text:
            data* pData = new data();
            pData->type = data::types::text;
            pData->value = m_tmpValue;
            m_pData.insert(pair<string, data*>(m_tmpName, pData));
            m_tmpName = "";
            m_tmpValue = "";
            m_state = States::start;
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
        switch (m_state)
        {
        case States::name:
            m_tmpName += input;
            break;
        case  States::equals:
            m_state = States::number;
        case States::text:
        case States::number:
            m_tmpValue += input;
            break;
        }
        break;
    case '}':
    case ',':
        switch (m_state)
        {
        case States::number:
            data* pData = new data();
            pData->type = data::types::number;
            pData->value = m_tmpValue;
            m_pData.insert(pair<string, data*>(m_tmpName, pData));
            m_tmpName = "";
            m_tmpValue = "";
            m_state = States::start;
        }
        break;
    default:
        switch (m_state)
        {
        case States::name:
            m_tmpName += input;
            break;
        case States::text:
            m_tmpValue += input;
            break;
        }
        break;
    }
}
