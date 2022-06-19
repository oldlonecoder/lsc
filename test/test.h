//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

#include <lsc/iostr/CArguments.h>

namespace Lsc
{

class Test
{
    CArgumentList<Test>* _mpArgs = nullptr;
    
    char **argv = nullptr;
    int argc = 0;
public:
    Test() = default;
    Test(int argc, char **argv);
    ~Test() = default;
    
    int execute();
    
    [[nodiscard]] bool HasArgs() const;
    
    
};

}