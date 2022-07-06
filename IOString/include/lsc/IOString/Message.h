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
    static int _mIndent ;
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
    
};

} // Lsc

//#endif //LSC_MESSAGE_H
