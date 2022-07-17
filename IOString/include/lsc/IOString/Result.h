#pragma once


#include <lsc/IOString/Message.h>
#include <optional>



namespace Lsc
{
template<class T = Message::Code> class Result
{
    std::optional<T> _m_ = std::nullopt;
public:
    Result() = default;
    ~Result() = default;

    Result(const T& aV){ _m_ = aV; }

    Result(Result&& ) = default;

    Result(Message&) { }

    Result& operator = (Result&& ) = default;
    Result& operator = (T&& aV)
    {
        if(_m_.has_value())
            _m_.reset();
        _m_ = std::move(aV);
        return *this;
    }

    Result& operator = (const T& aV)
    {
        if(_m_.has_value())
            _m_.reset();
        _m_ = aV;
        return *this;
    }

    Result& operator = (Message& aV)
    {
        if(_m_.has_value())
            _m_.reset();
        return *this;
    }
    
    operator bool() { return _m_.has_value(); }

    auto operator *() 
    {
        if(!_m_)
            throw Message::Error() << Message::Code::Empty;
        return *_m_;
    }

};
}
