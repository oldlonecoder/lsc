//
// Created by Mobile Klinik on 2022-07-06.
//

//#ifndef LSC_MESSAGE_H
//#define LSC_MESSAGE_H
#pragma once
#include <lsc/IOString/String.h>
#include <lsc/IOString/Encoding/Color.h>

namespace Lsc
{
namespace Source
{

struct IOSTR_LIB Location
{
    std::string _FunctionName;
    std::string _Filename;
    int _LineNo;
};

}

#define SourceLocation {__FUNCTION__, __FILE__, __LINE__}
#define SourceLongName {__PRETTY_FUNCTION__,"",__LINE__}
#define SourceName {__FUNCTION__,__FILE__,__LINE__}
#define SourceFile {__FUNCTION__,__FILE__,__LINE__}



class IOSTR_LIB Message
{
    String _mText;
    Source::Location _mSrc = {};
    Color::Format _mFormat = Color::Format::Ansi256;
    std::vector<std::string> _mComponents;
    static int _mcIndent ;
public:
    
    enum class Type : int8_t
    {
        Err,
        Warning,
        Info,
        Exception,
        Fatal,
        Status,
        Debug,
        Output,
        Comment,
        Syntax
    };
    
    enum class Code : uint16_t
    {
        Hello,
        Ok,
        Rejected,
        Accepted,
        Eof,
        Eos,
        NullPtr,
        NotEmpty,
        Empty,
        Unexpected,
        Expected,
        Endl,
        Begin,
        End,
        Implement,
        File,
        Function,
        ShortFunction,
        Line,
        Enter, // indent
        Leave  // unindent
    };
    
       using collection = std::vector<Message>;
    
    Message()=default;
    Message(const Message& r);
    Message(Message&& r) noexcept;
    
    Message(Message::Type type_, const Source::Location& src={});
    ~Message();
    
    
    Message& operator = (const Message&) = default;
    Message& operator = (Message&&) noexcept = default;
    
    Message& operator << (Message::Code c_);
    Message& operator << (const String& txt_);
    template<typename T> Message& operator<<(const T& arg_)
    {
        
         String str;
         str << arg_;
         _mComponents.push_back(str());
        return *this;
    }
    std::string CC();
    std::string Text() { return _mText(); }
    static void Init();
    Message::Code MessageCode() { return _mCode; }
    Message::Type MessageType() { return _mType; }
    static std::string CodeText(Message::Code c);
    static std::string TypeText(Message::Type t);

private:
    Message::Code _mCode = Message::Code::Hello;
    Message::Type _mType = Message::Type::Output;
    
    static std::vector<std::string_view> types_text;
    static std::vector<std::string_view> codes_text;
    static std::vector<std::string>      types_ansi256_attr;
    static std::vector<std::string>      codes_ansi256_attr;
    friend class Logger;
    static void InitCodes();
};

} // Lsc

//#endif //LSC_MESSAGE_H
