//
// Created by oldlonecoder on 6/19/22.
//

#include "lsc/iostr/CString.h"

namespace Lsc
{

[[maybe_unused]] string_view     CString::_msDefaultTokenSeparators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";



CString::CString(const string &aString):_mData(aString.c_str()){}
CString::CString(string_view&& aString):_mData(aString){}

CString::~CString()
{
}


#pragma region ProcessingTokens
CString::SPS::SPS(string_view &Str)
{
    mStart = mPos = Str.begin();
    mStop  = Str.end();
}

bool CString::SPS::Skip()
{
    if(End())
        return false;
    
    while(isspace(*mPos))
    {
        switch(*mPos)
        {
            case 10:
            {
                if((++mPos) >= mStop)
                    return false;
                if(*mPos == 13)
                    ++mPos;
                ++mLine;
                mCol = 1;
            }
                break;
            case 13:
            {
                if((++mPos) >= mStop)
                    return false;
                if(*mPos == 10)
                    ++mPos;
                ++mLine;
                mCol = 1;
            }
                break;
            case '\t':++mPos;
                ++mCol;
                break;
            default:++mPos;
                ++mCol;
                break;
        }
    }
    return mPos < mStop;
}

bool CString::SPS::End()
{
    return mPos >= mStop;
}

bool CString::SPS::operator++()
{
    if(mPos >= mStop)
        return false;
    ++mPos;
    ++mCol;
    if(mPos >= mStop)
        return false;
    return Skip();
}

bool CString::SPS::operator++(int)
{
    if(mPos >= mStop)
        return false;
    ++mPos;
    ++mCol;
    if(mPos >= mStop)
        return false;
    return Skip();
}

CString::SPS &CString::SPS::operator>>(CString::Word &w)
{
    w.mStart    = mPos;
    w.mLine     = mLine;
    w.mCol      = mCol;
    w.mPosition = mIndex = (uint64_t) (mPos - mStart);
    return *this;
}
string_view::iterator CString::SPS::ScanTo(string_view::iterator start, char c) const
{
    string_view::iterator p = start;
    ++p;
    while((p != mStop) && (*p != c))
        ++p;
    return p;
}

/*!
* @brief break/split/tokenize,etc... the content of this CString into pieces.
* @param Collection  OUTPUT reference to the 'Words array' containter, filled by this method.
* @param a_delimiters Separators in the form of a string of ascii-8 characters.
* @param KeepAsWord if true (or non-zero), the Separators will be put into the collection as they appear
* @return number of "Words/tokens" contained into the Collection.
* @notice : After several years of experience and experimentations, offset have determined that
* white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c a_delimiters.
*/
std::size_t CString::Words(CString::Word::List &Collection, string_view aDelimiters, bool KeepAsWord)
{
    
    //CString::SPS Crs = CString::SPS(_mData);
    if(_mData.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    _mCursor.Reset(_mData);
    string_view token_separators = aDelimiters.empty() ? CString::_msDefaultTokenSeparators : aDelimiters;
    if(!_mCursor.Skip())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    Word w;
    _mCursor >> w;
    
    while(!_mCursor.End())
    {
        //if (!Collection.empty());
        auto cc = _mCursor.mPos;
        if(token_separators.find(*_mCursor.mPos) != string::npos)
        {
            cc = _mCursor.mPos;
            if(cc > w.mStart)
            {
                --cc;
                Collection.push_back({w.mStart, cc, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
                
                _mCursor >> w;
                cc = _mCursor.mPos;
            }
            
            // '//' as one token instead of having two consecutive '/'
            if((*_mCursor.mPos == '/') && (*(_mCursor.mPos + 1) == '/'))
                ++_mCursor;
            
            if(KeepAsWord)
            {
                Collection.push_back({w.mStart, _mCursor.mPos, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
            }
            ++_mCursor;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!_mCursor.End())
                _mCursor >> w;
            else
            {
                return Collection.size();
            }
            
        }
        else if((*_mCursor.mPos == '\'') || (*_mCursor.mPos == '"'))
        { // Quoted litteral string...
            _mCursor >> w;
            if(KeepAsWord)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the Word coords anyway.
                Collection.push_back({w.mStart, w.mStart, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
            }
            
            auto p = _mCursor.ScanTo(w.mStart + (KeepAsWord ? 0 : 1), *_mCursor.mPos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(_mCursor.mPos < p)
                ++_mCursor; // compute white spaces!!!
            
            if(KeepAsWord)
            {
                // then push the litteral that is inside the quotes.
                Collection.push_back({w.mStart + 1, p - 1, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
                //++_Cursor; // _Cursor now on the closing quote
                _mCursor >> w; // Litteral is done, update w.
                Collection.push_back({w.mStart, p, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the Word.
                Collection.push_back({w.mStart, _mCursor.mPos, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
            }
            if(++_mCursor)
                _mCursor >> w;
            else
                return Collection.size();
            
        }
        else
        {
            cc = _mCursor.mPos;
            ++cc;
            if(cc == _mCursor.mStop)
            {
                ++_mCursor.mPos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.mStart < cc)
                {
                    Collection.push_back({w.mStart, cc - 1, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
                    ++_mCursor;
                }
                
                if(_mCursor.Skip())
                {
                    _mCursor >> w;
                    continue;
                }
                return Collection.size();
            }
            if(!_mCursor.End())
                ++_mCursor; // advance offset to the next separator/white space.
        }
    }
    if(_mCursor.mPos > w.mStart)
        Collection.push_back({w.mStart, _mCursor.mPos - 1, _mCursor.mStop, w.mLine, w.mCol, w.mPosition});
    
    return Collection.size();
}


#pragma endregion ProcessingTokens

string_view CString::Word::operator()()
{
    return string_view{&(*mStart), static_cast<size_t>(mE-mStart)};
}

string_view CString::Word::operator*()
{
    return string_view{&(*mStart), static_cast<size_t>(mE-mStart)};
}
std::string CString::Word::Mark() const
{
    std::string Str;
    auto  CStart = mStart - mPosition;
    int                         l  = 1;
    auto cc = CStart;
    // localiser le debut de la ligne;
    while(*cc && (cc > CStart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= CStart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != mSE) && (*cc != '\n') && (*cc != '\r'))
            Str += *cc++;
    }
    
    Str += '\n'; // Must not assume newline platform. Todo: call newline (format) platform dependant generator
    for(int x = 1; x < mCol; x++)
        Str += ' ';
    Str += '^';
    return Str;
}


void CString::Word::operator++()
{

}
void CString::Word::operator++(int)
{

}


string CString::Word::Location() const
{
    std::ostringstream in;
    in << "(" << mLine << ',' << mCol << ')';
    return in.str();
}
} // Lsc