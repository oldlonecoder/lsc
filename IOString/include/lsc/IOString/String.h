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
using std::string;

class IOSTR_LIB String
{
    static std::string     _msNull;
    std::string            _mStr;///< Private/Encapsulated std::string instance.
    std::string::size_type _arg_position = 0; // Initialize Argument index position...
    uint8_t                _mPrecision   = 4;
    bool                   _mBacColor = false;
    //char* __buff = nullptr;
    friend class winbuffer;
    
    /*!
     * @brief Format specifier in order:
     *
     * <ul>
     *     @li Justify           { '<', '>', '^' }
     *     @li Box-formatting    { '[]' }
     *         <ul>
     *             @li 'h:integer' : Box height ( or number of lines including borders ) defaults to 0 (zero - no height)
     *             @li 'w:integer' : Box width
     *             @li '^'         : Top Justify inner content
     *             @li '_'         : Bottom Justify inner content
     *             @li '-'         : Vertical Center Justify inner content
     *             @li '|'         : Horizontally centered inner content
     *             @li '='         : Line-Justify width inner center
     *             @li '<'         : justify to Left inner content
     *             @li '>'         : Justify to right inner content
     *             @li 'f'         : Draw the box frame ( as of this version, only single line UTF-8 box chars )
     *             @li ';'         : Mandatory separators between params (and args)
     *         </ul>
     *
     *     @li Width/Length      { integer }
     *     @li Decimal precision { .integer }
     *     @li Type modifiers:
     *     <ul>
     *         @li l : Long (32bits) modifier
     *         @li d : Base 10 signed integer ( ld : long base 10 32 bits integer modifier )
     *         @li f : floating point
     *         @li b : binary representation of byte(s) value.
     *         @li s : string
     *         @li c : ascii character
     *     </ul>
     *     @li Suplementary encodings:
     *     <ul>
     *         @li fg: Character Color Name / or #0xRGB
     *         @li bg: Background Color Name
     *         @li ...
     *     </ul>
     * </ul>
     * @param aStr
     * @return boolean false : no format applied, true: format was applied successfully.
    */
    struct IOSTR_LIB Format
    {
        string &mStr; ///< Mandatory constructor immediate assign
        string mBuf; ///< Working internal temp buffer
        
        char              mJustifyCode  = 0;///<  { none,  <, ^, > }.
        int               mWidth        = 0; ///< Padding Width.
        char              mFilling      = 0; ///< Filling char. (or numerical value)
        int               mPaddingWidth = 0; ///< Padding Group Length/Width ( such as "08b" )
        char              mGroupSep     = 0x32; ///< Padding group separator character. (default is space ' ').
        string::size_type mBegin        = 0, mEnd = 0;
        string::size_type mLength       = 0;
        std::size_t       mPosition     = std::string::npos;
        string::iterator  mCursor;
        Format() = delete;
        ~Format();
        explicit Format(string &aStr);
        bool Init();
        
        template<typename T> bool Input(const T &aArg)
        {
            if(!Init())
                return false;
            //...
            return true;
        }
        
        // Exploring/Trying to setup the Format params data:
        char Justify(string::iterator &it);     ///< Justify (left:'<', right:'>, center:'^')
        int Width(string::iterator &it);        ///< Minimum Buffer length/Width
        int LeadingZero(string::iterator &it);  ///< Padding with leading Zero's
        ///----------------------------------------------------------------------------------------
        
    };

private:
    
    // %[flags][width][.precision][Length]specifier
    struct format_t
    {
        uint8_t     F        = 0; // Flag ( - + . # 0 ) => if s='s' then '-' => justify right; '+' => justify left; '^' => justify center.
        uint8_t     W        = 0; // Width ( Length ) Value
        uint8_t     R        = 0; // Custom flag set if this format requires floating point spec.
        uint8_t     P        = 6; // Precision (Same as  default).
        uint8_t     L        = 0; // Length modifier ( linenum,ll,h,hh )
        std::size_t len      = 0; // Format Length.
        std::size_t position = 0; // Arg index position within _mStr.
        char        S        = 0; // Effective characeter code specifier such as 'd'; 's'; 'f'; 'l'; 'p'...
        const char  *C       = nullptr;
        
        format_t(std::string &Str) : C(Str.c_str())
        {}
        
        std::string operator()();
        
    };
    
    using lambda_fn_t = std::function<std::string(const String::format_t &Fmt)>;
public:
    
    using List = std::vector<std::string>;
    using Iterator = List::iterator;
    using CIterator = List::const_iterator;

public:
    
    String();
    String(const String &Str);
    String(String &&Str) noexcept;
    String(const std::string &aStr);
    String(std::string &&aStr);
    String(const char *aStr);
    
    ~String();
    
    String &operator=(const String &) = default;
    String &operator=(std::string &&aStr);
    String &operator=(String &&aStr) noexcept;
    String &operator=(const char *aStr);
    String &operator+=(const String &aStr);
    String &operator+=(const std::string &aStr);
    String &operator+=(char c);
    String &operator+(const String &aStr);
    String &operator+(const std::string &aStr);
    String &operator+(char c);
    
    template<typename T>
    String &operator+=(const T &a)
    {
        std::ostringstream os;
        os.precision(_mPrecision);
        os << a;
        _mStr.append(os.str());
        return *this;
    }
    
    template<typename T>
    String &operator+(const T &a)
    {
        std::ostringstream os;
        os.precision(_mPrecision);
        os << a;
        _mStr.append(os.str());
        return *this;
    }
    
    bool operator==(const String &aStr) const;
    
    bool empty() const
    {
        return _mStr.empty();
    }
    
    operator bool() const
    {
        return !_mStr.empty();
    }
    
    operator std::string()
    {
        return _mStr;
    }
    
    std::string &str()
    {
        return _mStr;
    }
    
    std::string operator()() const
    {
        return _mStr;
    }
    
    // -- on peut maintenant commencer nos routines de manipulations et de traitements....
    static String::List ToList(int argc, char **argv);
    String &operator<<(const String &aStr);
    String &operator<<(const char *aStr);
    String &operator<<(const std::string &aStr);
    String &operator<<(char c);
    String &operator<<(Color::Type c);
    bool SkipWS(std::string::iterator &pos);
    static bool SkipWS(const char *pos);
    String &operator>>(std::string &_arg);
    static std::string MakeStr(const char *B, const char *E);
    
    template<typename T>
    String &operator>>(T &_arg)
    {
        
        std::istringstream in(
            _mStr); //  When String was derived from std::string ... Oops!  std::istringstream in(*this);
        if constexpr (
            std::is_same<float, T>::value || std::is_same<double, T>::value ||
            std::is_same<long double, T>::value ||
            std::is_same<int, T>::value || std::is_same<unsigned int, T>::value ||
            std::is_same<int8_t, T>::value || std::is_same<uint8_t, T>::value ||
            std::is_same<int16_t, T>::value || std::is_same<uint16_t, T>::value ||
            std::is_same<int32_t, T>::value || std::is_same<uint32_t, T>::value ||
            std::is_same<int64_t, T>::value || std::is_same<uint64_t, T>::value
            )
        {
            in.precision(_mPrecision);
            _arg = 0;
            in.precision(_mPrecision);
            in >> _arg;
            return *this;
        }
        T R;
        in >> R;
        _arg = R;
        return *this;
    }
    
    char *Duplicate() const
    {
        return strdup(_mStr.c_str());
    }
    
    const char *c_str()
    {
        return _mStr.c_str();
    }
    
    //    struct TEACC_CORE_DLL rect_xy
    //    {
    //        int x = -1;
    //        int y = -1;
    //    };
    //
    //
    
    void Clear();
    
    //virtual const std::string& tea_id() { return "String";}
    
    static std::string datetime(const std::string &str_fmt);
    
    template<typename t>
    String &operator=(const t &_a)
    {
        std::ostringstream os;
        os.precision(_mPrecision);
        os << _a;
        Clear();
        _mStr = os.str();
        return *this;
    }
    
    std::string Extract(const std::string &first_lhs, const std::string &first_rhs);
    
    [[nodiscard]] std::string::const_iterator ScanTo(std::string::const_iterator start, char c) const;
    
    const char *ScanTo(const char *start, char c) const;
    
    //bool test();
    String &Octal(uint64_t __arg)
    {
        
        std::ostringstream os;
        os << std::oct << __arg;
        if(scan_arg() == std::string::npos)
        {
            _mStr.append(os.str());
            return *this;
        }
        
        return format<std::string>(os.str());
    }
    
    [[nodiscard]] size_t Length() const
    {
        return _mStr.size();
    }
    
    char &operator[](size_t p)
    {
        return _mStr[p];
    }
    
    /*!
     * @brief Replace on the argument position into this string with the argument content, or behave as += operator.
     * @tparam T
     * @param aArg Template argument value
     * @return  reference to this.
     */
    template<typename T>
    String &Arg(T aArg)
    {
        Format(_mStr).Input(aArg);
        //            if (!Fmt.LocateArg()) return *this;
        //
        //            std::ostringstream os;
        //            os << aArg;
        //            _mStr.append(os.str());
        return *this;
    }
    
    int filter(const String::List &a_exp);
    
    template<typename T>
    std::string expand(const T &cnt)
    {
        String ss;
        
        int      x = cnt.size();
        for(auto item: cnt)
        {
            ss << item;
            if(x-- > 1)
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
    
    template<typename T>
    String &format(const T &_argv);
    
    /// <summary>
    /// Prepare removing std::sprintf calls.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="fmt_data"></param>
    /// <param name="a_arg"></param>
    /// <returns>Reference to self</returns>
    template<typename T>
    String &input_arg(const String::format_t &fmt_data, const T &a_arg)
    {
        std::ostringstream os;
        os << a_arg;
        _mStr.erase(fmt_data.position, fmt_data.len);
        _mStr.insert(fmt_data.position, os.str());//  += os.str();
        return *this;
    }
    
    template<typename T>
    String &operator<<(const T &Argument)
    {
        //Format(_mStr).Input(Argument);
        //return *this;
        
        if (scan_arg() == std::string::npos) {
            std::ostringstream os;
            os << Argument;
            _mStr += os.str();
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
    
    template<typename T> String &hexadecimal(T &v)
    {
        std::stringstream is(_mStr.c_str() + 2);
        //std::cerr << " this:'" << _D.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }
    
    static std::string type_of(std::string &&func_desc);
    
    /*!
     * @brief "Stringify Bytes into binary representation.
     * @tparam T  Argument of type T
     * @param __arg Argument value.
     * @param padd Apdding value (number of padding 0's to fille the numberic base size in digits)
     * @param f  number_t of contiguous bits to group.
     * @return std::string
     */
    template<typename T>
    static std::string to_binary(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int     nbytes = sizeof(T);
        
        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t *) &__arg, nbytes);
        std::string stream = "";
        int         s      = 0;
        //bool discard = false;
        for(int     x      = 1; x <= nbytes; x++)
        {
            seq = tableau[nbytes - x];
            if((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for(int y = 7; y >= 0; y--)
            { // est-ce que le bit #y est � 1 ?
                if(s >= f)
                {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if(b & seq)
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
    std::string::iterator begin()
    { return _mStr.begin(); }
    
    std::string::iterator end()
    { return _mStr.end(); }
    // --------------------------
private:
    std::string::size_type scan_arg();
    
    void process_arg(String::lambda_fn_t Fn);
    
    void put_arg(const std::string &aStr);
    
};

template<typename T>
String &String::format(const T &_argv)
{
    format_t fmt = {_mStr};
    char     buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";
    
    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _mStr.begin() + _arg_position;
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
    while((n != _mStr.end()) && isdigit(*n))
        ++n;
    l = n; // save  ahead 'cursor position'
    --n;
    if(n >= c)
    {
        int t = 0;
        while(n >= c)
            // interpret format width value:
            fmt.W = fmt.W + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
    }
    else
        fmt.W = 0; // no width
    c = l; // update effective cursor position
    
    // check floating point format: m,n,l:=>  read positions
    if(*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while((n != _mStr.end()) && isdigit(*n))
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
        fmt.R = fmt.P; // no floating point format
    
    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    // format prefix ends;
    // Now the format type mnemonic:
    //  - remove the format string.
    //  - insert 'formatted/interpreted' value substring at the _arg_position:
    //  - reset _arg_position
    switch(*c)
    {
        case 'b':
        {
            if constexpr (!std::is_same<T, const std::string &>::value)
            {
                std::string BinaryStr;
                bool        pad = fmt.F == '0';
                BinaryStr       = String::to_binary<T>(_argv, pad,
                    fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits to 128 ...
                
                //std::sprintf(buf, "%s", BinaryStr.c_str());
                fmt.len = (c + 1) - beg;  //save format substring's Length
                _mStr.erase(_arg_position, fmt.len);
                _mStr.insert(_arg_position, BinaryStr.c_str(), BinaryStr.length());
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
        default:break;
    }
    
    fmt.len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_mStr, _arg_position, fmt.len);
    //std::cout << "ff(_mStr, _arg_position, fmt.len): '" << ff << "'\n";
    // -- Clang-tidy:error: cannot pass object of non-trivial type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string &>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=const std::string&]:'" << buf << "'\n";
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=std::string]:'" << buf << "'\n";
    }
    else
    {
        std::sprintf(buf, ff.c_str(), _argv);
        //std::cout << "\nbuffer[argv=T (const T&)]:'" << buf << "'\n";
    }
    _mStr.erase(_arg_position, fmt.len);
    _mStr.insert(_arg_position, buf, std::strlen(buf));
    _arg_position = 0;
    return *this;
};

}


//#endif //VXIO_FRAMEWORK_IOSTR_H
