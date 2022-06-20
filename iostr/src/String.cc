//
// Created by oldlonecoder on 6/18/22.
//

#include <lsc/iostr/String.h>
#include <lsc/iostr/CString.h>

namespace Lsc
{

string                           String::_msNull;

String::~String()
{
    _mData.clear();
}

String::String(const char *aStr) : _mData(aStr)
{}
String::String(string aStr) : _mData(aStr)
{}
String::String(std::string_view aStr) : _mData(aStr)
{}
String::String(const String &aStr) : _mData(aStr._mData)
{}

String &String::operator+=(const String &aStr)
{
    
    return *this;
}
String &String::operator+=(const string &aStr)
{
    return *this;
}
String &String::operator+=(char c)
{
    return *this;
}
String &String::operator+(const String &aStr)
{
    return *this;
}
String &String::operator+(const string &aStr)
{
    return *this;
}
String &String::operator+(char c)
{
    return *this;
}
bool String::operator==(const String &aStr) const
{
    return false;
}
String::List String::StdList(int argc, char **argv)
{
    return Lsc::String::List();
}
String &String::operator<<(const String &aStr)
{
    return *this;
}
String &String::operator<<(const char *aStr)
{
    return *this;
}
String &String::operator<<(const string &aStr)
{
    return *this;
}
String &String::operator<<(char c)
{
    return *this;
}
String &String::operator<<(Color::Type c)
{
    return *this;
}
bool String::SkipWS(string::iterator &pos)
{
    
    return false;
}
bool String::SkipWS(const char *pos)
{
    return false;
}

String &String::operator>>(string &_arg)
{
    _arg = _mData;
    return *this;
}

std::string String::MakeStr(const char *B, const char *E)
{
    std::string Str;
    const char  *C = B;
    if((!B) || (!E) || (!*B) || (!*E))
        return Str;
    while(C <= E)
        Str += *C++;
    
    return Str;
}

void String::Clear()
{
    _mData.clear();
    
}

std::string String::DateTime(const string &str_fmt)
{
    time_t    rawtime;
    struct tm *timeinfo;
    char      tmb[180];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    std::string _s = tmb;
    return tmb;
}

std::string String::ExtractSurrounded(const string &first_lhs, const string &first_rhs)
{
    std::size_t lhs_pos = _mData.find(first_lhs);
    if(lhs_pos == std::string::npos)
        return "";
    std::size_t rhs_pos = _mData.find(first_rhs);
    if(rhs_pos == std::string::npos)
        return "";
    
    return _mData.substr(lhs_pos, rhs_pos - lhs_pos);
}

const char *String::ScanTo(const char *start, char c) const
{
    const char *p = start;
    if(!p)
        return nullptr;
    ++p;
    while(p && (*p != c))
        ++p;
    return p;
}

int String::Filter(const String::List &a_exp)
{
    if(a_exp.empty())
        return 0;
    auto i = a_exp.cbegin();
    
    // array::front is the init of the match at the beginning of this string.
    // if ...front() is Empty, then there is no required match at the beginning of this string
    // so proceed to the next substring to find.
    // if the last is not Empty, then match the last substring.
    std::string::size_type pos = 0;
    if(!(*i).empty())
    {
        pos = _mData.find((*i));
        if(pos != 0) // no match
            return false;
    }
    else;
    ++i;
    auto end = a_exp.cend();
    --end;
    if(!(*i).empty())
    {
        while(i < end)
        {
            if((*i).empty())
            {
                ++i;
                continue;
            }
            pos = _mData.find((*i), pos);
            if(pos != std::string::npos)
            {
                ++pos;
                ++i;
                continue;
            }
            return false;
        }
    }
    if(!(*end).empty())
    {
        std::size_t sz = (*end).size();
        pos            = _mData.find((*end), pos);
        if(pos != std::string::npos)
        {
            if((pos + sz) != _mData.size())
                return false;
        }
    }
    return true;
}

std::string String::SizeF(uint64_t sz)
{
    float  fsize       = (float) sz;
    String us          = "";
    int    currentunit = 0;
    String units[]     = {"B", "kb", "mb", "gb", "tb"};
    int    unitssize   = sizeof(units) / sizeof(units[0]);
    
    while(fsize > 1024.0 && currentunit < unitssize - 1)
    {
        fsize /= 1024.0;
        currentunit++;
    }
    
    us = std::fixed;
    us += std::setprecision(2);
    us += fsize;
    us += " ";
    us += units[currentunit];
    
    return us;
}

string::iterator String::ScanTo(string::iterator aStart, char c)
{
    auto Start = aStart;
    while(Start != _mData.end() && *Start != c)
        ++Start;
    return Start; // Either at @ _mData.end() or on c.
}







} // Lsc
