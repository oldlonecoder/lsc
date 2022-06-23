//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

#include <lsc/IOString/CArguments.h>

namespace Lsc
{

class Test
{
    
    
    char **argv = nullptr;
    int argc = 0;
public:
    Test() = default;
    Test(int argc, char **argv);
    ~Test() = default;
    
    int execute();
    
    [[nodiscard]] bool HasArgs() const;
    [[nodiscard]] int InitCArgumentList();
    
    
    
};

}