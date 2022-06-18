//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

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

class IOSTR_LIB String
{
    string                         _mData;
    static string                  _msNull;
    string::size_type              _mAargPosition     = 0; // Initialize Argument index position...
    [[maybe_unused]] static string _msDefaultTokenSeparators;
    uint8_t                        _mDecimalPrecision = 4;
    
    /*!
     * @brief Format the string
     *
     * @author Serge Lussier (oldlonecoder), lussier.serge@gmail.com
     */
    struct __attribute__ ((visibility ("hidden")))  Format final
    {
        string &mString;
        string            mArgStr;
        string::size_type mPos = string::npos;
        
        Format() = delete;
        
        Format(string &aData) : mString(aData)
        {
            mPos = mString.find('%', 0);
        }
        
        ~Format()
        {
            mArgStr.clear();
        }
        
        explicit operator bool() const
        { return mPos != string::npos; }
    }; // struct String::Format
    
public:
    String() = default;
    String(const char* aStr);
    String(string aStr);
    String(std::string_view aStr);
    String(const String& aStr);
    
    ~String();
    
    string operator()() { return _mData; }
    
};

} // Lsc

