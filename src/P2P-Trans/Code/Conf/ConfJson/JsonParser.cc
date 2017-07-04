/************************/
/* Author: Max Sperling */
/************************/

#include "JsonParser.hh"

#include <exception>

using namespace std;

JsonParser::JsonParser()
{
    m_file = new fstream();
    resetParser();
}

JsonParser::~JsonParser()
{
    resetParser();
    delete m_file;
}

void JsonParser::parseFile(string file)
{
    resetParser();

    m_file->open(file);
    if (!m_file->is_open()) throw exception();

    char c;
    while (!m_file->eof())
    {
        m_file->get(c);
        changeState(c);
    }
    m_file->close();
}

void JsonParser::getValStr(string name, string &value)
{
    auto iter = m_data.find(name);
    if (iter == m_data.end()) throw exception();
    if (iter->second->type != Data::Types::text) throw exception();
    value = iter->second->value;
}

void JsonParser::getValInt(string name, unsigned int &value)
{
    auto iter = m_data.find(name);
    if (iter == m_data.end()) throw exception();
    if (iter->second->type != Data::Types::number) throw exception();
    value = stoi(iter->second->value);
}

void JsonParser::resetParser()
{
    for (auto it = m_data.begin(); it != m_data.end(); ++it)
    {
        delete it->second;
    }
    m_data.clear();

    m_state = States::start;
    m_ident = "";
    m_value = "";
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
            Data* pData = new Data();
            pData->type = Data::Types::text;
            pData->value = m_value;
            m_data.insert(pair<string, Data*>(m_ident, pData));
            m_ident = "";
            m_value = "";
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
            m_ident += input;
            break;
        case  States::equals:
            m_state = States::number;
        case States::text:
        case States::number:
            m_value += input;
            break;
        }
        break;
    case '}':
    case ',':
        switch (m_state)
        {
        case States::number:
            Data* pData = new Data();
            pData->type = Data::Types::number;
            pData->value = m_value;
            m_data.insert(pair<string, Data*>(m_ident, pData));
            m_ident = "";
            m_value = "";
            m_state = States::start;
        }
        break;
    default:
        switch (m_state)
        {
        case States::name:
            m_ident += input;
            break;
        case States::text:
            m_value += input;
            break;
        }
        break;
    }
}
