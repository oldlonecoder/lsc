//
// Created by oldlonecoder on 21-11-20.
//

#include <lsc/IOString/String.h>
#ifdef _WIN32
    #include <Winsock2.h> 
#else
    #include <arpa/inet.h>
#endif

namespace Lsc
{
using std::string;

string  String::__nullstr__               = "";

std::ostream &operator<<(std::ostream &in, const String &_s)
{
    in << _s();
    return in;
}

String::String()
{
}

String::String(const char *a_str)
{
    //SOut << __PRETTY_FUNCTION__ << ":[\"" << a_str << "\"]:\n";
    _d = a_str;
}

String::String(std::string &&a_str)
{
    std::swap(_d, a_str);
    a_str.clear();
}

String::String(const string &a_str)
{
    //ffnl << a_str << "\n";
    _d = a_str;
}

String::String(const String &Str)
{
    //ffnl << Str._str << "\n";
    _d      = Str._d;
}

String::String(String &&Str) noexcept
{
    //ffnl << Str._str << "\n";
    std::swap(_d, Str._d);
}

String::~String()
{
    //ffnl << _str << "\n";
    _d.clear();
}



//String& String::operator=(const String& a_str)
//{
//    //ffnl << a_str._str << "\n";
//    _D = a_str._D;
//    _ArgPosition = a_str._ArgPosition;
//    _Precision = a_str._Precision;
//    return *this;
//}

String &String::operator=(String &&a_str) noexcept
{
    //ffnl << a_str._str << "\n";
    std::swap(_d, a_str._d);
    _arg_position = 0;
    return *this;
}

bool String::operator==(const String &a_str) const
{
    return _d == a_str._d;
}

String &String::operator=(string &&a_str)
{
    //ffnl << a_str << "\n";
    swap(_d, a_str);
    _arg_position = 0;
    return *this;
}

String &String::operator<<(char c)
{
    if(scan_arg() == std::string::npos)
    {
        _d += c;
        return *this;
    }
    return format<char>(c);
}

String &String::operator=(const char *a_str)
{
    if(a_str)
        _d = a_str;
    else
        _d        = "";
    _arg_position = 0;
    return *this;
}

String &String::operator<<(const string &a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        _d += a_str;
        return *this;
    }
    return format<const std::string &>(a_str);
}

String::list_t String::to_stdlist(int argc, char **argv)
{
    list_t  args;
    for(int x = 0; x < argc; x++)
    {
        args.push_back(argv[x]);
        
    }
    return args;
}

String &String::operator<<(const String &a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        std::ostringstream os;
        os << a_str();
        _d += os.str();
        return *this;
    }
    return format<const std::string &>(a_str());
}

String &String::operator<<(const char *a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a_str;
        _d += os.str();
        return *this;
    }
    return format<const char *>(a_str);
}

String &String::operator+=(const String &a_atr)
{
    _d += a_atr._d;
    return *this;
}
String &String::operator+=(const string &a_atr)
{
    _d += a_atr;
    return *this;
}

String &String::operator+=(char c)
{
    _d += c;
    return *this;
}

String &String::operator+(char c)
{
    _d += c;
    return *this;
}

String &String::operator+(const String &a_atr)
{
    _d += a_atr._d;
    return *this;
}
String &String::operator+(const string &a_atr)
{
    _d += a_atr;
    return *this;
}

String &String::operator>>(string &arg_)
{
    arg_ = _d;
    return *this;
}

std::string String::make_str(const char *B, const char *E)
{
    std::string Str;
    const char *C = B;
    if((!B) || (!E) || (!*B) || (!*E))
        return Str;
    while(C <= E)
        Str += *C++;
    
    return Str;
}

void String::clear()
{
    _d.clear();
    _arg_position = (string::size_type) 0;
}

std::string String::datetime(const std::string &str_fmt)
{
    time_t rawtime;
    struct tm *timeinfo;
    char tmb[180];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    std::string _s = tmb;
    return tmb;
}

string::size_type String::scan_arg()
{
    _arg_position = _d.find('%', 0);
    return _arg_position;
}

void String::put_arg(const string &aStr)
{
    if(scan_arg() == std::string::npos)
    {
        _d.append(aStr);
        return;
    }
    
    format<const string &>(aStr);
}

// Ce qui fait royalement chier avec les iterateurs des stl, m'est que depuis linenum'iterateur, comment on accede � son conteneur ???????
bool String::skip_ws(string::iterator &pos)
{
    if(pos == _d.end()) // aucun moyen de savoir si linenum'it�rateur en est un de notre conteneur "_str" !!
        return false;
    while(isspace(*pos))
        ++pos;
    return true;
}

// Ce qui fait royalement chier avec les iterateurs des stl, m'est que depuis linenum'iterateur, comment on accede � son conteneur ???????
bool String::skip_ws(const char *pos)
{
    if(!pos)
        return false;
    while(pos && (*pos && isspace(*pos)))
        ++pos;
    return true;
}


string::const_iterator String::scan_to(string::const_iterator start, char c) const
{
    string::const_iterator p = start;
    ++p;
    while((p != _d.end()) && (*p != c))
        ++p;
    return p;
}

const char *String::scan_to(const char *start, char c) const
{
    const char *p = start;
    if(!p)
        return nullptr;
    ++p;
    while(p && (*p != c))
        ++p;
    return p;
}


int String::filter(const String::list_t &a_exp)
{
    if(!a_exp.size())
        return 0;
    auto i = a_exp.cbegin();
    
    // array::front is the init of the match at the beginning of this string.
    // if ...front() is Empty, then there is no required match at the beginning of this string
    // so proceed to the next substring to find.
    // if the last is not Empty, then match the last substring.
    std::string::size_type pos = 0;
    if(!(*i).empty())
    {
        pos = _d.find((*i));
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
            pos = _d.find((*i), pos);
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
        pos            = _d.find((*end), pos);
        if(pos != std::string::npos)
        {
            if((pos + sz) != _d.size())
                return false;
        }
    }
    return true;
}

std::string String::SizeF(uint64_t sz)
{
    float fsize       = (float) sz;
    String  us          = "";
    int   currentunit = 0;
    String  units[]     = {"B", "kb", "mb", "gb", "tb"};
    int   unitssize   = sizeof(units) / sizeof(units[0]);
    
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
std::string String::extract_surrounded(const std::string &first_lhs, const std::string &first_rhs)
{
    std::size_t lhs_pos = _d.find(first_lhs);
    if(lhs_pos == std::string::npos)
        return "";
    std::size_t rhs_pos = _d.find(first_rhs);
    if(rhs_pos == std::string::npos)
        return "";
    
    return _d.substr(lhs_pos, rhs_pos - lhs_pos);
}

/*!
 * @brief Try to extract the "template" type from the \c func_desc that normally shall be given by __PRETTY_FUNCTION__ at the call Location.
 *
 * template parameter assumed to be "T" ...for now.
 *
 * @param func_desc
 *
 * @note This function has no control of the proper format and (function) prototype data.
 * @return the resulting type text
 */
std::string String::type_of(string &&func_desc)// , const std::string& _T) -> _T is the class to search.
{
//    String               text = std::move(func_desc);
//    String::word::list_t w;
//
//    std::size_t count = text.words(w);
//    for(auto    s: w)
//    {
//        std::cout << s();
//    }
//    std::cout << '\n';
    return "c to implement, please\n";
}

void String::process_arg(String::lambda_fn_t Fn)
{
    format_t fmt = {_d};
    
    std::string::iterator c = _d.begin() + _arg_position;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :
    
    switch(*c)
    {
        case '-':
        case '+':
        case '#':
        case '0':fmt.F = *c++;
            break;
        default:
            //++m;
            break;
    }
    
    n = c;
    // %[width]:
    while((n != _d.end()) && isdigit(*n))
        ++n;
    l = n;
    --n;
    if(n >= c)
    {
        int t = 0;
        while(n >= c)
            fmt.W = fmt.W + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
    }
    else
        fmt.W = 0;
    c = l;
    
    if(*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while((n != _d.end()) && isdigit(*n))
            ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R     = 0;
        while(n >= c)
            fmt.R = fmt.R + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
        c = l;
    }
    else
        fmt.R = fmt.P;
    
    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    switch(*c)
    {
        case 'b':fmt.S = *c++;
            break;
        case 'd': // Decimale ou entier
        case 'i':fmt.S = *c++;
            break;
        case 'x':
        case 'X':fmt.S = *c++;
            break;
        case 'f':
        case 'F':
        case 'g':
        case 'G':fmt.S = *c++;
            break;
        case 's':fmt.S = *c++;
    }
    
    fmt.len = c - beg;
    //std::string ff(_D, _ArgPosition, fmt.distance);
    _d.erase(_arg_position, fmt.len);
    _d.insert(_arg_position, Fn(fmt));
    _arg_position = 0;
}

std::string String::format_t::operator()()
{
    std::ostringstream str;
    str << "\nFlag    :" << F << "\n";
    str << "Width     :" << (int) W << "\n";
    str << "Precision :" << (int) P << "\n";
    str << "delta     :" << (int) len << "\n";
    str << "spec      :" << (char) S << "\n";
    return str.str();
}


String &String::operator<<(Color::Type c)
{
    if(scan_arg() == std::string::npos)
    {
        //std::cerr << __FUNCTION__ << ":\n";
        _d += Color::Ansi(c);
        return *this;
    }
    //@todo handle color output format!
    return format<std::string>(Color::Ansi(c));
}


}