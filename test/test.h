//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

#include <lsc/IOString/CArguments.h>

#include <lsc/IOString/Result.h>

namespace Lsc
{

class Test
{
    
    CArgumentList<Test> *_mpArgs = nullptr; ///< Future tests...
    char **argv = nullptr;
    int argc = 0;
public:
    Test() = default;
    Test(int argc, char **argv);
    ~Test();
    
    Result<> execute();
    
    [[nodiscard]] bool HasArgs() const;
    [[nodiscard]] Result<> InitArguments();
        
};

}