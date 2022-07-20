//
// Created by oldlonecoder on 2022-07-06.
//
#include <Lsc/IOString/String.h>



namespace Lsc
{

// ------------------------------ String::Format ----------------------------------------
String::Format::Format(string &aStr):mStr(aStr)
{
    if(!Init()) return;
    
}

String::Format::~Format()
{
    mBuf.clear();
}


/*!
 * @brief Initialize the format start and end location into the string.
 * @return true if the string has proper format delimiters; false otherwise.
 *
 * @author oldlonecoder (lussier.serge@gmail.com).
 */
bool String::Format::Init()
{
    mBegin = mPosition = mStr.find("\\{",0);
    mEnd = mStr.find('}',mPosition);
    if(mEnd == string::npos) return false; // Missing closing brace.
    mLength = mEnd - mBegin;
    mCursor = mStr.begin() + mPosition;
    
    std::cout << __PRETTY_FUNCTION__ << " Cursor on '" << *mCursor << "'\n    Advancing past \\{ :\n";
    ++mCursor; ++mCursor; // Or mCursor += 2;
    std::cout << "    Now Cursor on '" << *mCursor << "'\n";
    Justify(mCursor);
    return true;
}


char String::Format::Justify(string::iterator &it)
{
    mJustifyCode = ' ';
    if(mStr.find(*mCursor, 0) != std::string::npos)
        mJustifyCode = *mCursor++;
    
    std::cout << __PRETTY_FUNCTION__  << " - Justfy Code: '" << mJustifyCode <<"'\n";
    std::cout << "    Now Cursor on '" << *mCursor << "'\n";
    return mJustifyCode;
}

int String::Format::Width(std::string::iterator &it)
{
    return 0;
}

int String::Format::LeadingZero(std::string::iterator &it)
{
    return 0;
}


//----------------------- String::Format End -----------------------------------------------------------


}