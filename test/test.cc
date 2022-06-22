//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"

Lsc::Test::Test(int argc, char **argv):argc(argc), argv(argv)
{

}

int Lsc::Test::execute()
{
    if(HasArgs())
    {
        std::cout << argv[0] << " ==> arg count: " << argc << "...\n";
        CString Str = static_cast<const char*>(argv[1]);
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
    return 0;
}


bool Lsc::Test::HasArgs() const
{
    return argc > 1;
}
