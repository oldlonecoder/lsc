//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"
#include <lsc/IOString/String.h>

Lsc::Test::Test(int argc, char **argv) : argc(argc), argv(argv)
{

}

int Lsc::Test::execute()
{
    if(HasArgs())
    {
        std::cout << argv[0] << " ==> Arg count: " << argc << "...\n";
        CString Str = argv[1];
        std::cout << "argv[1]:" << Str() << '\n';
        //...
        for(int i {1}; i < argc; i++)
            std::cout << "\033[38;5;118mArg #\033[38;5;255m" << i << "\033[38;5;255m: \33[38;5;98m\"" << argv[i] << "\"\033[0m\n";
    }
    else
    {
        std::cout << " Oops! Where are the arguments?\n";
        return 1;
    }
    return TestCString();
}

bool Lsc::Test::HasArgs() const
{
    return argc > 1;
}
int Lsc::Test::InitCArgumentList()
{
    auto Args = CArgumentList<Test>(*this, argc, argv);
    Args.PushArg({"", "CString", false,true,&Test::TheCArg});
    
    return 0;
}

int Lsc::Test::TheCArg(Lsc::CString aStr)
{
    return 0;
}

int Lsc::Test::TestString()
{
    std::cout << __PRETTY_FUNCTION__ << " - Lsc::String :\n";
    String Str = "    Hello, Lsc::World!\n";
    std::cout << Str() << '\n';
    Str = "    Arg: %d";
    Str << 42;
    std::cout << Str() << '\n';
    return 0;
    
    return 0;
}
int Lsc::Test::TestCString()
{
    String aStr = "New CString.";
    CString cStr  = aStr.c_str();
    std::cout << __PRETTY_FUNCTION__ << " - " << cStr() << '\n';
    return CStringTokenize();
}
int Lsc::Test::CStringTokenize()
{
    CString aStr = "CString:Token";
    CString::Token::List  Tokens;
    auto count = aStr.Tokenize(Tokens);
    for(auto Token : Tokens)
    {
        std::cout << "Token: '" << Token() << "'\n";
    }
    
    return 0;
}
