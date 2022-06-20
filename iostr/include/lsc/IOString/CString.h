//
// Created by oldlonecoder on 6/19/22.
//


#pragma once

//#ifndef LSC_CSTRING_H
//#define LSC_CSTRING_H
#include <lsc/iostr/dlexport.h>
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
namespace Lsc
{
using std::string;
using std::string_view;

class IOSTR_LIB CString
{
    string_view        _mData;
    static string_view _msNull;
    [[maybe_unused]] static string_view _msDefaultTokenSeparators;
public:
    struct IOSTR_LIB Word
    {
        string_view::iterator mStart;
        string_view::iterator mE;
        string_view::iterator mSE;
        
        string_view operator()();
        string_view operator*();
        
        using List = std::vector<CString::Word>;
        using Iterator = List::iterator;
        [[maybe_unused]] std::string Mark() const;
        
        int         mLine     = 1;
        int         mCol      = 1;
        std::size_t mPosition = 0;
        void operator++();
        void operator++(int);
        [[nodiscard]] std::string Location() const;
    };
    std::size_t Words(CString::Word::List &Collection, string_view Delimiters = "", bool KeepAsWord = true);
    
    using List = std::vector<string_view>;
    
    explicit CString(const std::string& aString);
    CString() = default;
    explicit CString(string_view&& aString);
    ~CString();
private:
    struct __attribute__ ((visibility ("hidden"))) SPS
    {
        string_view::iterator mStart;
        string_view::iterator mPos;
        string_view::iterator mStop; /// ...
        
        int      mLine  = 1;
        int      mCol   = 1;
        uint64_t mIndex = 0;
        
        SPS() = default;
        ~SPS() = default;
        
        explicit SPS(string_view& aCStr);
        bool Skip();
        bool End();
        bool operator++();
        bool operator++(int);
        void Reset(string_view &_str)
        {
            mPos   = mStart = _str.begin();
            mLine  = mCol   = 1;
            mIndex = 0;
            mStop  = _str.end();
        }
        [[nodiscard]] string_view::iterator ScanTo(string_view::iterator start, char c) const;
        SPS &operator>>(CString::Word &aWord);
        //BCE& operator = (const Word& w);
    } _mCursor;
    static string_view DefaultTokenSeparators()
    {
        return CString::_msDefaultTokenSeparators;
    }
    
    static string_view TokenSeparators()
    {
        return CString::_msDefaultTokenSeparators;
    }
};

} // Lsc

//#endif //LSC_CSTRING_H
