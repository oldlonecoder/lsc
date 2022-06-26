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

enum class ArgSwitchFormat : uint8_t
{
    StdShort,      ///< As  -[A-Za-z]+
    StdNamed,      ///< As --Named
    NameColon,    ///< As Name: // Default. @see CArgumentList<>::Argument
    NameEQ        ///< As Name=
};


/*!
 * @brief Command Line arguments holder/processor.
 * 
 * Slowly exploring my own implementation of a cmdline processor, multi-syntax, multi-values, Named and unnamed arguments. 
 * @tparam T Template Name
 */
template<typename T> class CArgumentList
{
    T   &_mInvokeObject;
    int _mArgc = 0;
    char **_mArgv = nullptr;
    
    ArgSwitchFormat _mFormat  = ArgSwitchFormat::NameColon;
    
    CString::List        _mSysArgs; ///< Unnamed (and no switch) Arguments.
    CString::Token::List _mTokens; ///< List of cmdline components;
public:
    using Switch [[maybe_unused]] = int (T::*)(CString); ///< class method address that will be invoked as the switch matches.
    
    /*!
     * @brief Command line argument holder.
     * //...
     *
     * @author Serge Lussier (oldlonecoder)_, lussier.serge@gmail.com
     */
    class IOSTR_LIB Argument
    {
        string _mValue; ///< Assignable Single value argument
        string _mSwitchToken; ///< Configured Switch Token of this Argument
        string _mName; ///< Name and-or Long Name of this argument.
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
        Argument(string aSwitchToken, string aArgName, bool aReqValue, bool aRequired, CArgumentList<T>::Switch aSwitchFnPtr) :
            _mSwitchToken(std::move(aSwitchToken)),
            _mName(std::move(aArgName)),
            _mRequire({aReqValue, aRequired, false}),
            _mSwitchFn(aSwitchFnPtr)
        {}
        
        ~Argument()
        {
            _mValue.clear();
            _mName.clear();
        }
        
        bool HasRequiresValue()
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
    CArgumentList(T &aObj, int aArgc, char **aArgv) :
        _mInvokeObject(aObj),
        _mArgc(aArgc),
        _mArgv(aArgv)
    {
    }
    
    
    CArgumentList &operator<<(typename CArgumentList::Argument &&aArg)
    {
        _mArguments[aArg._mName] = aArg;
        return *this;
    }
    CArgumentList &PushArg(typename CArgumentList::Argument &&aArg)
    {
        return *this;
    }

    CArgumentList& SetGenArgsProc(CArgumentList<T>::Switch aProc)
    {
        _mAnonProc = aProc;
        return *this;
    }
private:
    typename Argument::List _mArguments;
    typename Switch _mAnonProc;
};
}
//#endif //LSC_CARGUMENTS_H
