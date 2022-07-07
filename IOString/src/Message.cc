//
// Created by oldlonecoder on 21-11-20.
//

#include <lsc/IOString/Message.h>
#include <lsc/IOString/CString.h>

namespace Lsc
{

int Message::_mcIndent = 0;

std::vector<std::string_view> Message::types_text=
{
    {"error"},
    {"warning"},
    {"information"},
    {"exception"},
    {"fatal"},
    {"status"},
    {"debug"  },
    {"output"},
    {"comment"},
    {"syntax error"}
};

std::vector<std::string_view> Message::codes_text=
{
    {"hello"} ,
    {"ok"} ,
    {"rejected"} ,
    {"accepted"} ,
    {"end of file"} ,
    {"end of stream"} ,
    {"null pointer"} ,
    {"not empty"} ,
    {"empty"} ,
    {"unexpected"},
    {"expected"},
    {"\n"},      // dummy string because ...
    {"begin"},
    {"end"},
    {"implement"}

};


std::vector<std::string>      Message::types_ansi256_attr;
std::vector<std::string>      Message::codes_ansi256_attr;

Message::Message(const Message &r)
{
    _mText = r._mText;
    _mType = r._mType;
    _mCode = r._mCode;
    _mSrc = r._mSrc;
    _mComponents = r._mComponents;
    
}

Message::Message(Message &&r) noexcept
{
    _mSrc = std::move(r._mSrc);
    _mText = std::move(r._mText);
    _mComponents = std::move(r._mComponents);
    _mType = r._mType;
    _mCode = r._mCode;
    
}

Message::~Message()
{
    _mText.Clear();
    _mComponents.clear();
}

Message::Message(Message::Type type_, const Source::Location &src)
{
    _mType = type_;
    _mSrc = src;
    if(!src._FunctionName.empty())
    {
        // Use "src_funcname" coorectly soon...;
        ++Message::_mcIndent;
        //...
    }
}

Message &Message::operator<<(Message::Code c_)
{
    String str;
    int i = static_cast<int>(c_);
    str << Message::codes_ansi256_attr[i] << codes_text[i].data();
    _mComponents.emplace_back(str());
    return *this;
}


void Message::InitCodes()
{
    if(!Message::types_ansi256_attr.empty()) return;
    Message::types_ansi256_attr =
    {
        Color::Ansi(Color::DarkRed2),       // err,
        Color::Ansi(Color::Yellow),         //warning,
        Color::Ansi(Color::BlueViolet),     //info,
        Color::Ansi(Color::Cyan3),          //exception,
        Color::Ansi(Color::DeepSkyBlue7),   //fatal,
        Color::Ansi(Color::DarkSlateGray1), //status,
        Color::Ansi(Color::LightCoral),     //debug,
        Color::Ansi(Color::White),          //output,
        Color::Ansi(Color::Grey0),          //comment,
        Color::Ansi(Color::DeepPink1),      //syntax
    };
    
    Message::codes_ansi256_attr =
    {
        Color::Ansi(Color::Lime),           //hello,
        Color::Ansi(Color::LightCyan3),     //ok,
        Color::Ansi(Color::Red3),           //rejected,
        Color::Ansi(Color::LightSeaGreen),  //accepted,
        Color::Ansi(Color::LightPink4),     //eof,
        Color::Ansi(Color::LightPink3),     //eos,
        Color::Ansi(Color::Grey0),          //null_ptr,
        Color::Ansi(Color::LightGoldenrod2),//not_empty,
        Color::Ansi(Color::Grey)            ,//empty,
        Color::Ansi(Color::LightSteelBlue3),//unexpected,
        Color::Ansi(Color::LightSteelBlue3),//expected,
        Color::Ansi(Color::Reset),          //endl,
        Color::Ansi(Color::Reset),          //begin,
        Color::Ansi(Color::Reset),          //end,
        Color::Ansi(Color::White),          //implement,
        Color::Ansi(Color::White),          //_file_,
        Color::Ansi(Color::BlueViolet),//_function_,
        Color::Ansi(Color::BlueViolet),//_fn_,
        Color::Ansi(Color::White),          //_line_
    };
}

/*
 *
 *  <div class="nom de la classe"...> contenu </div>
 *  <span class="nom de la classe"...> contenu </span>
 *  <i class="icone id-icon">text content</i>
 *  <script> code javascript </script>
 */

std::string Message::CC()
{
    String str;
    str += Message::types_ansi256_attr[static_cast<int8_t>(_mType)];
    str += Message::types_text[static_cast<int8_t>(_mType)];
    str += Color::Ansi(Color::White);
    str += ' ';
    
    if(!_mSrc._Filename.empty())
    {
        //str += '\n';
        str += Message::codes_ansi256_attr[static_cast<int16_t>(Message::Code::File)];
        CString fname = _mSrc._Filename.c_str();
        CString::Token::List words;
#ifndef _WIN32
        if(fname.Tokenize(words,"/") > 0)
#else
        if (fname.Tokenize(words, "\\") > 0)
#endif
            str += words.back()();
        else
            str += fname();
        str += ' ';
    }
    if(_mSrc._LineNo > 0)
    {
        str << ": " << Color::White << _mSrc._LineNo << ' ' << Message::codes_ansi256_attr[static_cast<int16_t>(Message::Code::Line)] << ' ';
    }

    if(!_mSrc._FunctionName.empty())
    {
        str += Message::codes_ansi256_attr[static_cast<int16_t>(Message::Code::Function)];
        str+= _mSrc._FunctionName;
        str += ": ";
    }
    
    for(auto const& s : Message::_mComponents)
    {
        str += s;
        //str << ' ';
    }
    _mText = str;
    return str();
}

Message &Message::operator<<(const String &txt_)
{
    _mComponents.push_back(txt_());
    return *this;
}
void Message::Init()
{// ...
    Message::InitCodes();
}
std::string Message::CodeText(Message::Code c)
{
    std::string str;
    if(c == Message::Code::Enter)
    {
        ++Message::_mcIndent;
        return "";
        //...
    }
    else
        if(c==Message::Code::Leave)
        {
            --Message::_mcIndent;
            return "";
            //...
        }
        
    str += Message::codes_ansi256_attr[static_cast<int8_t>(c)];
    str += Message::codes_text[static_cast<int8_t>(c)];
    return str;
}

std::string Message::TypeText(Message::Type t)
{
    std::string str;
    str += Message::types_ansi256_attr[static_cast<int8_t>(t)];
    str += Message::types_text[static_cast<int8_t>(t)];
    return str;
}

}