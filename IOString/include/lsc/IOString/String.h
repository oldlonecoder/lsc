//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_IOSTR_H
//#define VXIO_FRAMEWORK_IOSTR_H
#pragma once


#include <lsc/IOString/dlexport.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>

#include <lsc/IOString/Encoding/Color.h>


namespace Lsc
{


class IOSTR_LIB String
{
    static std::string __nullstr__;
    std::string _d;///< Private/Encapsulated std::string instance.
    std::string::size_type _arg_position = 0; // Initialize Argument index position...
    static std::string _default_token_separators;
    uint8_t _decimal_precision = 4;
    //char* __buff = nullptr;
    friend class winbuffer;
private:
    // %[flags][width][.precision][Length]specifier
    struct format_t
    {
        uint8_t F = 0; // Flag ( - + . # 0 ) => if s='s' then '-' => justify right; '+' => justify left; '^' => justify center.
        uint8_t W = 0; // Width ( Length )
        uint8_t R = 0; // Custom flag set if this format requires floating point spec.
        uint8_t P = 6; // Precision (Same as  default).
        uint8_t L = 0; // Length modifier ( linenum,ll,h,hh )
        std::size_t len = 0; // Format Length.
        std::size_t position = 0; // arg index position within _d.
        char S = 0; // Effective characeter code specifier such as 'd'; 's'; 'f'; 'l'; 'p'...
        const char* C = nullptr;

        format_t(std::string& Str) : C(Str.c_str())
        {}
        std::string operator()();

    };
    using lambda_fn_t = std::function<std::string(const String::format_t& Fmt)>;
public:

    using list_t = std::vector<std::string>;
    using Iterator = list_t::iterator;
    using CIterator = list_t::const_iterator;

public:
    struct IOSTR_LIB word
    {
        std::string::const_iterator start;
        std::string::const_iterator E;
        std::string::const_iterator SE;

        std::string operator()();
        std::string operator*();

        using list_t = std::vector<String::word>;
        using iterator_t = list_t::iterator;
        [[maybe_unused]] std::string mark() const;

        int line = 1;
        int col = 1;
        std::size_t position = 0;
        void operator++();
        void operator++(int);

        std::string location();
    };

private:
    struct s_p_s
    {
        std::string::const_iterator start;
        std::string::const_iterator pos;
        std::string::const_iterator stop; /// ...

        int         line = 1;
        int         col = 1;
        uint64_t    index = 0;

        s_p_s() = default;
        ~s_p_s() = default;

        s_p_s(const std::string& Str);
        bool skip();
        bool end();
        bool operator++();
        bool operator++(int);
        void reset(const std::string& _str)
        {
            pos = start = _str.cbegin();
            line = col = 1;
            index = 0;
            stop = _str.cend();
        }
        s_p_s& operator>>(String::word& w);
        //BCE& operator = (const Word& w);

    } _cursor;

public:

    String();
    String(const String& Str);
    String(String&& Str) noexcept;
    String(const std::string& aStr);
    String(std::string&& aStr);
    String(const char* aStr);

    ~String();

    String& operator=(const String&) = default;

    String& operator=(std::string && aStr);
    String& operator=(String && aStr) noexcept;
    String& operator=(const char* aStr);

    String& operator+=(const String & aStr);
    String& operator+=(const std::string & aStr);
    String& operator+=(char c);

    String& operator+(const String & aStr);
    String& operator+(const std::string & aStr);
    String& operator+(char c);

    template<typename T> String& operator+=(const T & a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a;
        _d.append(os.str());
        return *this;
    }

    template<typename T> String& operator+(const T & a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a;
        _d.append(os.str());
        return *this;
    }

    bool operator==(const String & aStr) const;

    bool empty() const
    {
        return _d.empty();
    }
    operator bool() const
    {
        return !_d.empty();
    }
    operator std::string()
    {
        return _d;
    }
    std::string& str()
    {
        return _d;
    }
    std::string operator()() const
    {
        return _d;
    }

    // -- on peut maintenant commencer nos routines de manipulations et de traitements....
    static String::list_t to_stdlist(int argc, char** argv);
    String& operator<<(const String & aStr);
    String& operator<<(const char* aStr);
    String& operator<<(const std::string & aStr);
    String& operator<<(char c);
    String& operator<<(Color::Type c);
    

    bool skip_ws(std::string::iterator & pos);
    static bool skip_ws(const char* pos);
    String& operator>>(std::string & _arg);
    static std::string make_str(const char* B, const char* E);
    template<typename T> String& operator>>(T & _arg)
    {

/*           if constexpr (std::is_same<T, uint64_t&>::value || std::is_same<T, uint16_t&>::value || std::is_same<T, uint32_t&>::value || std::is_same<T, uint64_t&>::value || std::is_same<T, uint8_t&>::value || std::is_same<T, uint16_t&>::value || std::is_same<T, uint32_t&>::value || std::is_same<T, uint64_t&>::value)
        {
            std::string::size_type pos;
            if (((pos = _D.find("0x")) != std::string::npos) || ((pos = _D.find("0X")) != std::string::npos))
            {
                pos += 2;
                std::istringstream i(_D.c_str() + pos);
                i >> std::hex >> _arg;
                return *this;
            }
        }*/
        std::istringstream in(_d); //  When String was derived from std::string ... Oops!  std::istringstream in(*this);
        if constexpr (
            std::is_same<float,T>::value || std::is_same<double, T>::value || std::is_same<long double, T>::value ||
            std::is_same<int, T>::value || std::is_same<unsigned int, T>::value ||
            std::is_same<int8_t, T>::value || std::is_same<uint8_t, T>::value ||
            std::is_same<int16_t, T>::value || std::is_same<uint16_t, T>::value ||
            std::is_same<int32_t, T>::value || std::is_same<uint32_t, T>::value ||
            std::is_same<int64_t, T>::value || std::is_same<uint64_t, T>::value
            )
        {
            in.precision(_decimal_precision);
            _arg = 0;
        }
        T R;
        in >> R;
        _arg = R;
        return *this;
    }

    char* duplicate() const
    {
        return strdup(_d.c_str());
    }


    const char* c_str()
    {
        return _d.c_str();
    }

//    struct TEACC_CORE_DLL rect_xy
//    {
//        int x = -1;
//        int y = -1;
//    };
//
//

    void clear();

    //virtual const std::string& tea_id() { return "String";}

    static std::string datetime(const std::string & str_fmt);

    template<typename t> String& operator=(const t & _a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << _a;
        clear();
        _d = os.str();
        return *this;
    }
    std::string extract_surrounded(const std::string & first_lhs, const std::string & first_rhs);
    [[nodiscard]] std::string::const_iterator scan_to(std::string::const_iterator start, char c) const;
    const char* scan_to(const char* start, char c) const;

    //bool test();
    String& octal(uint64_t __arg)
    {

        std::ostringstream os;
        os << std::oct << __arg;
        if (scan_arg() == std::string::npos)
        {
            _d.append(os.str());
            return *this;
        }

        return format<std::string >(os.str());
    }

    [[nodiscard]] size_t length() const
    {
        return _d.size();
    }
    char& operator[](size_t p)
    {
        return _d[p];
    }
    //bool empty() { return _str.Empty(); }

    static std::string default_token_separators()
    {
        return String::_default_token_separators;
    }
    static std::string token_separators()
    {
        return String::_default_token_separators;
    }
    std::size_t words(String::word::list_t & wcollection, const std::string & a_delimiters = "", bool keep_as_word = true) const;

    template<typename T> String& arg(T _arg)
    {
        if (scan_arg() == std::string::npos)
        {
            std::ostringstream os;
            os << std::oct << _arg;
            _d.append(os.str());
            return *this;
        }

        return format<T >(_arg);
    }
    int filter(const String::list_t & a_exp);

    template<typename T> std::string expand(const T & cnt)
    {
        String ss;

        int x = cnt.size();
        for (auto item : cnt)
        {
            ss << item;
            if (x-- > 1)
                ss << ',';
        }
        return ss();
    }

    static std::string upper_case(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::toupper(c);
        });

        return s;
    }
    static std::string lower_case(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return s;
    }

    static std::string SizeF(uint64_t sz);

    template<typename T> String& format(const T & _argv);
    /// <summary>
    /// Prepare removing std::sprintf calls.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="fmt_data"></param>
    /// <param name="a_arg"></param>
    /// <returns>Reference to self</returns>
    template<typename T> String& input_arg(const String::format_t& fmt_data, const T& a_arg)
    {
        std::ostringstream os;
        os << a_arg;
        _d.erase(fmt_data.position, fmt_data.len);
        _d.insert(fmt_data.position, os.str());//  += os.str();
        return *this;
    }

    template<typename T> String& operator<<(const T & Argument)
    {
        if (scan_arg() == std::string::npos)
        {
            std::ostringstream os;
            os << Argument;
            _d += os.str();
            return *this;
        }

/*
         process_arg([Argument](const String::format_t& Fmt) -> std::string {
            if (Fmt.S == 'b')
            {
                bool pad = Fmt.F == '0';
                ///@todo Still have to resolve T here...
                if constexpr ((!std::is_same<T, const std::string&>::value) && (!std::is_same<T, const char*>::value))
                    return String::to_binary<T>(Argument, pad, Fmt.W <= 128 ? Fmt.W : 128);
            }
            if constexpr ((std::is_same<T, const std::string&>::value) || (std::is_same<T, const char*>::value))
                return Argument;

            std::ostringstream os;
            os << Argument;
            return os.str();
        });
*/
        return format(Argument);
    }

    template<typename T> String& hexadecimal(T & v)
    {
        std::stringstream is(_d.c_str() + 2);
        //std::cerr << " this:'" << _D.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }

    static std::string type_of(std::string && func_desc);


    /*!
     * @brief "Stringify Bytes into binary representation.
     * @tparam T  Argument of type T
     * @param __arg Argument value.
     * @param padd Apdding value (number of padding 0's to fille the numberic base size in digits)
     * @param f  number_t of contiguous bits to group.
     * @return std::string
     */
    template<typename T> static std::string to_binary(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&__arg, nbytes);
        std::string stream = "";
        int s = 0;
        //bool discard = false;
        for (int x = 1; x <= nbytes; x++)
        {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for (int y = 7; y >= 0; y--)
            { // est-ce que le bit #y est � 1 ?
                if (s >= f)
                {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq)
                    stream += '1';
                else
                    stream += '0';
            }
        }
        /*tableau.Clear();*/
        return stream;
    }
    /*!
        Important: renommer begin et end &agrave; cbegin et cend pour std::string::const_iterator !!
    */
    std::string::iterator begin() { return _d.begin(); }
    std::string::iterator end() { return _d.end(); }
    // --------------------------
private:
    std::string::size_type scan_arg();
    void process_arg(String::lambda_fn_t Fn);

    void put_arg(const std::string & aStr);
    int push_word(word::list_t & strm, word & w, std::string::size_type sz);

};

template<typename T> String& String::format(const T& _argv)
{
    format_t fmt = { _d };
    char buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";

    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _d.begin() + _arg_position;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :

    switch (*c)
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
    while ((n != _d.end()) && isdigit(*n))
        ++n;
    l = n; // save  ahead 'cursor position'
    --n;
    if (n >= c)
    {
        int t = 0;
        while (n >= c)
        // interpret format width value:
            fmt.W = fmt.W + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
    }
    else
        fmt.W = 0; // no width
    c = l; // update effective cursor position

    // check floating point format: m,n,l:=>  read positions
    if (*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while ((n != _d.end()) && isdigit(*n)) ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R = 0;
        while (n >= c)
            fmt.R = fmt.R + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
        c = l;
    }
    else
        fmt.R = fmt.P; // no floating point format

    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    // format prefix ends;
    // Now the format type mnemonic:
    //  - remove the format string.
    //  - insert 'formatted/interpreted' value substring at the _arg_position:
    //  - reset _arg_position
    switch (*c)
    {
        case 'b':
        {
            if constexpr (!std::is_same<T, const std::string&>::value)
            {
                std::string BinaryStr;
                bool pad = fmt.F == '0';
                BinaryStr = String::to_binary<T>(_argv, pad, fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits to 128 ...

                //std::sprintf(buf, "%s", BinaryStr.c_str());
                fmt.len = (c + 1) - beg;  //save format substring's length
                _d.erase(_arg_position, fmt.len);
                _d.insert(_arg_position, BinaryStr.c_str(), BinaryStr.length());
                _arg_position = 0;
                return *this;
            }
            break;
        }

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
        case 's':
        case 'c':fmt.S = *c++;
            break;
        default:
            break;
    }

    fmt.len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_d, _arg_position, fmt.len);
    //std::cout << "ff(_d, _arg_position, fmt.len): '" << ff << "'\n";
    // -- Clang-tidy:error: cannot pass object of non-trivial type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string&>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=const std::string&]:'" << buf << "'\n";
    }
    else
    if constexpr (std::is_same<T, std::string>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=std::string]:'" << buf << "'\n";
    }
    else
    {
        std::sprintf(buf, ff.c_str(), _argv);
        //std::cout << "\nbuffer[argv=T (const T&)]:'" << buf << "'\n";
    }
    _d.erase(_arg_position, fmt.len);
    _d.insert(_arg_position, buf, std::strlen(buf));
    _arg_position = 0;
    return *this;
};


}


//#endif //VXIO_FRAMEWORK_IOSTR_H
