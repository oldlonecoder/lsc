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

string  String::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";
string  String::__nullstr__               = "";

std::ostream &operator<<(std::ostream &in, const String &_s)
{
    in << _s();
    return in;
}

String::String()
{
    // _D has been instanciated here as well...
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
}

String::String(const char *a_str)
{
    //SOut << __PRETTY_FUNCTION__ << ":[\"" << a_str << "\"]:\n";
    _d = a_str;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
}

String::String(std::string &&a_str)
{
    std::swap(_d, a_str);
    a_str.clear();
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
}

String::String(const string &a_str)
{
    //ffnl << a_str << "\n";
    _d = a_str;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
}

String::String(const String &Str)
{
    //ffnl << Str._str << "\n";
    _d      = Str._d;
    _cursor = Str._cursor;
}

String::String(String &&Str) noexcept
{
    //ffnl << Str._str << "\n";
    std::swap(_d, Str._d);
    _cursor = std::move(Str._cursor);
}

String::~String()
{
    //ffnl << _str << "\n";
    _d.clear();
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
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
    _cursor       = std::move(a_str._cursor);
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
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = --_d.end();
    
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
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.end();
    
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
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.end();
    
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
    if(_cursor.pos == _cursor.stop)
        return false;
    
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

string String::word::operator()()
{
    string _s;
    if(start == E)
        _s.insert(_s.begin(), start, E + 1);
    else
        _s.insert(_s.begin(), start, E + 1);
    
    return _s;
}

std::string String::word::operator*()
{
    string _s;
    if(start == E)
        _s.insert(_s.begin(), start, E + 1);
    else
        _s.insert(_s.begin(), start, E + 1);
    return _s;
}

[[maybe_unused]] std::string String::word::mark() const
{
    String                        str;
    std::string::const_iterator cstart = start - position;
    
    int                         l  = 1;
    std::string::const_iterator cc = cstart;
    // localiser le debut de la ligne;
    while(*cc && (cc > cstart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= cstart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != SE) && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    String tstr;
    
    tstr << str << '\n';
    for(int x = 1; x < col; x++)
        tstr << ' ';
    tstr << '^';
    return tstr();
}

void String::word::operator++()
{
    
}

void String::word::operator++(int)
{
}

std::string String::word::location()
{
    String str = "(%d,%d)";
    str << line << col;
    return str();
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

/*!
    * @brief break/split/tokenize,etc... the content of this String into pieces.
    * @param wcollection  OUTPUT reference to the 'Words array' containter, filled by this method.
    * @param a_delimiters Separators in the form of a string of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as they appear
    * @return number of "Words/tokens" contained into the wcollection.
    * @notice : After several years of experience and experimentations, offset have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c a_delimiters.
    */
std::size_t String::words(String::word::list_t &wcollection, const std::string &a_delimiters, bool keep_as_word) const
{
    
    String::s_p_s Crs = String::s_p_s(_d);
    if(_d.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.reset(_d);
    std::string token_separators = a_delimiters.empty() ? String::_default_token_separators : a_delimiters;
    if(!Crs.skip())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    word w;
    Crs >> w;
    
    while(!Crs.end())
    {
        //if (!wcollection.empty());
        std::string::const_iterator cc = Crs.pos;
        if(token_separators.find(*Crs.pos) != string::npos)
        {
            cc = Crs.pos;
            if(cc > w.start)
            {
                --cc;
                wcollection.push_back({w.start, cc, Crs.stop, w.line, w.col, w.position});
                
                Crs >> w;
                cc = Crs.pos;
            }
            
            // '//' as one token instead of having two consecutive '/'
            if((*Crs.pos == '/') && (*(Crs.pos + 1) == '/'))
                ++Crs;
            
            if(keep_as_word)
            {
                wcollection.push_back({w.start, Crs.pos, Crs.stop, w.line, w.col, w.position});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.end())
                Crs >> w;
            else
            {
                return wcollection.size();
            }
            
        }
        else if((*Crs.pos == '\'') || (*Crs.pos == '"'))
        { // Quoted litteral string...
            Crs >> w;
            if(keep_as_word)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the Word coords anyway.
                wcollection.push_back({w.start, w.start, Crs.stop, w.line, w.col, w.position});
            }
            
            string::const_iterator p = scan_to(w.start + (keep_as_word ? 0 : 1), *Crs.pos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs.pos < p)
                ++Crs; // compute white spaces!!!
            
            if(keep_as_word)
            {
                // then push the litteral that is inside the quotes.
                wcollection.push_back({w.start + 1, p - 1, Crs.stop, w.line, w.col, w.position});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                wcollection.push_back({w.start, p, Crs.stop, w.line, w.col, w.position});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the Word.
                wcollection.push_back({w.start, Crs.pos, Crs.stop, w.line, w.col, w.position});
            }
            if(++Crs)
                Crs >> w;
            else
                return wcollection.size();
            
        }
        else
        {
            cc = Crs.pos;
            ++cc;
            if(cc == Crs.stop)
            {
                ++Crs.pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.start < cc)
                {
                    wcollection.push_back({w.start, cc - 1, Crs.stop, w.line, w.col, w.position});
                    ++Crs;
                }
                
                if(Crs.skip())
                {
                    Crs >> w;
                    continue;
                }
                return wcollection.size();
            }
            if(!Crs.end())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs.pos > w.start)
        wcollection.push_back({w.start, Crs.pos - 1, Crs.stop, w.line, w.col, w.position});
    
    return wcollection.size();
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
    String               text = std::move(func_desc);
    String::word::list_t w;
    
    std::size_t count = text.words(w);
    for(auto    s: w)
    {
        std::cout << s();
    }
    std::cout << '\n';
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

String::s_p_s::s_p_s(const std::string &Str)
{
    start = pos = Str.cbegin();
    stop  = Str.cend();
}

bool String::s_p_s::skip()
{
    if(end())
        return false;
    
    while(isspace(*pos))
    {
        switch(*pos)
        {
            case 10:
            {
                if((++pos) >= stop)
                    return false;
                if(*pos == 13)
                    ++pos;
                ++line;
                col = 1;
            }
                break;
            case 13:
            {
                if((++pos) >= stop)
                    return false;
                if(*pos == 10)
                    ++pos;
                ++line;
                col = 1;
            }
                break;
            case '\t':++pos;
                ++col;
                break;
            default:++pos;
                ++col;
                break;
        }
    }
    return pos < stop;
}

bool String::s_p_s::end()
{
    return pos >= stop;
}

bool String::s_p_s::operator++()
{
    if(pos >= stop)
        return false;
    ++pos;
    ++col;
    if(pos >= stop)
        return false;
    return skip();
}

bool String::s_p_s::operator++(int)
{
    if(pos >= stop)
        return false;
    ++pos;
    ++col;
    if(pos >= stop)
        return false;
    return skip();
}

String::s_p_s &String::s_p_s::operator>>(String::word &w)
{
    w.start    = pos;
    w.line     = line;
    w.col      = col;
    w.position = index = (uint64_t) (pos - start);
    return *this;
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