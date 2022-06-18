//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"

Lsc::Test::Test(int argc, char **argv):argc(argc), argv(argv)
{

}

int Lsc::Test::exec()
{
    std::cout << "arg count: " << argc << "...\n";
    for(int i{1}; i<argc; i++)
        std::cout << "\033[38;5;118mArg #\033[38;5;255m" << i+1 << "\033[38;5;0m: \"" << argv[i] << "\"\n";
    return 0;
}
