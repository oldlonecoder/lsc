//
// Created by oldlonecoder on 6/19/22.
//

//#ifndef LSC_CARGUMENTS_H
//#define LSC_CARGUMENTS_H


#pragma once

#include <lsc/IOString/CString.h>

#include <map>

namespace Lsc
{
using std::string_view;
using std::cout;

template<typename T> class CArgumentList
{
    T                  &_mInvokeObject;
    int _mArgc = 0;
    char ** _mArgv = nullptr;
    
    CString::List _mArgs; ///< Unnamed (and no switch) Arguments.
    CString::Token::List _mTokens; ///< List of cmdline components;
public:
    using Switch [[maybe_unused]] = int (T::*)(CString);
    
    
    class IOSTR_LIB Argument
    {
        string _mValue;
        string _mSwitchToken;
        string _mName;
        struct Require
        {
            bool Value: 1;
            bool Required: 1;
            bool Used: 1;
        }      _mRequire = {false, false, false};
        
        CArgumentList<T>::Switch _mSwitchFn = nullptr;
    public:
        
        using List = std::map<string, Argument>;
        
        Argument() = default;
        Argument(string aSwitchToken, string aArgName, bool aReqValue, bool aRequired, CArgumentList<T>::Switch aSwitchFnPtr):
        _mSwitchToken(std::move(aSwitchToken)),
        _mName(std::move(aArgName)),
        _mRequire({aReqValue,aRequired,false}),
        _mSwitchFn(aSwitchFnPtr)
        {}
        
        ~Argument() = default;
        
        bool RequiresValue()
        {
            return _mRequire.Value;
        }
        
        void SetValue(const string &v)
        {
            _mValue = v;
        }
        string Value()
        {
            return _mValue;
        }
        bool &Used()
        {
            return _mRequire.Used;
        }
        
        
    };
    
    CArgumentList() = delete;
    CArgumentList(int aArgc, char** aArgv):
    _mArgc(aArgc),
    _mArgv(aArgv)
    {
    }
    explicit CArgumentList(T &aObj) : _mInvokeObject(aObj)
    {}
    
    CArgumentList& operator << (CArgumentList::Argument&& aArg)
    {
        _mArguments[aArg._mName] = aArg;
        return *this;
    }
    
private:
    typename Argument::List _mArguments;
};
}
//#endif //LSC_CARGUMENTS_H
