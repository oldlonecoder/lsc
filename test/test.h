//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

#include <Lsc/IOString/CArguments.h>

#include <Lsc/IOString/Expect.h>

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
    
    Expect<> execute();
    
    [[nodiscard]] bool HasArgs() const;
    [[nodiscard]] Expect<> InitArguments();
        
};

}