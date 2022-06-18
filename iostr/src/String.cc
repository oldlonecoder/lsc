//
// Created by oldlonecoder on 6/18/22.
//

#include <lsc/iostr/String.h>

namespace Lsc
{

string                      String::_msNull;
[[maybe_unused]] string     String::_msDefaultTokenSeparators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";

String::~String()
{
    _mData.clear();
}


String::String(const char *aStr):_mData(aStr){}
String::String(string aStr):_mData(aStr){}
String::String(std::string_view aStr):_mData(aStr){}
String::String(const String &aStr):_mData(aStr._mData){}



} // Lsc