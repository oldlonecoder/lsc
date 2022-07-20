

#pragma once


#include <Lsc/IOString/Message.h>
#include <any>

namespace Lsc
{

template<typename T = Message::Code>  class  Expect
{
    
    std::any _mA;
    bool _mSt = false;

public:
    Expect() = default;
    
    Expect(Message &m)
    {
        //Message msg = m;
        _mA = m;
        _mSt   = false;
    }
    
    Expect(const T &v)
    {
        _mA = v; // Must be const ?
        _mSt   = true;
    }
    
    Expect(Expect &&e) noexcept
    {
        _mA = std::move(e._mA);
        _mSt   = e._mSt;
    }
    
    Expect(const Expect &e)
    {
        _mSt = e._mSt;
        _mA = e._mA;
    }
    
    Expect &operator=(Message &m)
    {
        _mA.reset();
        _mSt  = false;
        //Message msg = m;
        _mA = m;
        return *this;
    }
    
    Expect &operator=(Expect &&e) noexcept
    {
        _mA.reset();
        _mA = std::move(e._mA);
        _mSt = std::move(e._mSt);
        return *this;
    }
    
    Expect &operator=(const Expect &e)
    {
        if(&e == this)
            return *this;
        _mA.reset();
        _mA = e._mA;
        _mSt = e._mSt;
        return *this;
    }
    
    Expect &operator=(const T &v)
    {
        //        /if(mF)
        _mA.reset();
        _mA = v;
        _mSt = true;
        return *this;
    }
    
    explicit operator bool() const
    { return _mSt; }
    
    Message& operator()()
    {
        if(!_mSt)
            return std::any_cast<Message&>(_mA);

        throw Message( Message::Type::Err ) << ": " << " - Expected value type is present. Use deref(*) prefix operator.";
    }
        
    auto operator*()
    {
        if(!_mSt)
        {
            _mA.reset();
            throw Message( Message::Type::Err ) << ": " << " - Expected value type failed assign.";
        }

        return std::any_cast<T>(_mA);
    }
    
    void reset()
    {
        _mA.reset();
        _mSt = false;
    }
    
    ~Expect()
    {
        //Reset();
    }
     


};


}