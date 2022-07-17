//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"
#include <lsc/IOString/String.h>


namespace Lsc
{

Test::Test(int argc, char** argv) : argc(argc), argv(argv)
{

}

Test::~Test()
{
    std::cout << "Delete _mpArgs:\n";
    delete _mpArgs;
}

Result<> Test::execute()
{
    Message::InitCodes();
    auto C = InitArguments();
    if (HasArgs())
    {
        String Str;
        Str << Color::BlueViolet <<  argv[0] << Color::Yellow << " ==> Arg count: " << Color::CadetBlue2 << argc << "...\n";
        Str << Color::White << argv[1];
        Message::Debug(SourceLocation) << Str();
        //...
        for (int i{ 1 }; i < argc; i++)
            Message::Output() << Color::Aqua <<  "Arg #" << Color::White << i << ": " << Color::Yellow << argv[i] << Color::Reset;
    }
    else
        return Message::Error() << Message::Code::Expected << " Arguments to the program";
    
    return Message::Code::Accepted;
}

bool Test::HasArgs() const
{
    return argc > 1;
}


Result<> Test::InitArguments()
{
    auto _mpArgs = new CArgumentList<Test>(*this, argc, argv);
    //_mpArgs->PushArg({ "", "CString", false,true,&Test::TheCArg });

    return Message::Code::Accepted;
}


}