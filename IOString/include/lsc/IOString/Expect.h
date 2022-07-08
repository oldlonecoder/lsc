

#pragma once


#include <lsc/IOString/Message.h>
#include <any>

namespace Lsc
{

template<typename T> IOSTR_LIB class Expect
{
    
   std::any value;
    bool state = false;

public:
    expect() = default;
    
    expect(rem &m)
    {
        //rem msg = m;
        value = m;
        state   = false;
    }
    
    expect(const T &v)
    {
        value = v; // Must remove const ?
        state   = true;
    }
    
    expect(expect &&e) noexcept
    {
        value = std::move(e.value);
        state   = e.state;
    }
    
    expect(const expect &e)
    {
        state = e.state;
        value = e.value;
    }
    
    expect &operator=(rem &m)
    {
        value.reset();
        state  = false;
        rem msg = m;
        value = std::move(msg);
        return *this;
    }
    
    expect &operator=(expect &&e) noexcept
    {
        value.reset();
        value = std::move(e.value);
        state = std::move(e.state);
        return *this;
    }
    
    expect &operator=(const expect &e)
    {
        if(&e == this)
            return *this;
        value.reset();
        value = e.value;
        state = e.state;
        return *this;
    }
    
    expect &operator=(const T &v)
    {
        //        /if(mF)
        value.reset();
        value = v;
        state = true;
        return *this;
    }
    
    explicit operator bool() const
    { return state; }
    
    rem operator()()
    {
        if(!state)
            return std::any_cast<rem&>(value);
        return std::any_cast<rem>(value);
    }
    
    auto &operator*()
    {
        if(!state)
        {
            value.reset();
            throw rem( rem::type::err ) << ": " << " - Expected value was not returned. >>\n >> ";
        }

        return std::any_cast<T&>(value);
    }
    
    void reset()
    {
        value.reset();
        state = false;
    }
    
    ~expect()
    {
        //Reset();
    }
     


};


}