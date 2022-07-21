//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"
#include <Lsc/IOString/String.h>


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

Expect<> Test::execute()
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
    
    return TestIOCon();
    //return Message::Code::Accepted;
}

bool Test::HasArgs() const
{
    return argc > 1;
}


Expect<> Test::InitArguments()
{
    auto _mpArgs = new CArgumentList<Test>(*this, argc, argv);
    //_mpArgs->PushArg({ "", "CString", false,true,&Test::TheCArg });

    return Message::Code::Accepted;
}

Expect<> Test::TestIOCon()
{
    Console Con;
    auto R = Con.Init();
    std::cout << " Console.Init : " << Message::CodeText(*R) << "; \n";
    String Str;
    Str << " This terminal dimensions: " << Color::Lime << Con.Width() << Color::Reset << "x" << Color::Lime << Con.Height() << Color::Reset << '\n';
    std::cout << Str();
    Str = "Test Size values:";
    Size S = Con.Dimensions();
    Str << S.ToString();
    std::cout << Str() << '\n';
    return Message::Code::Ok;
}

}