//
// Created by oldlonecoder on 6/18/22.
//

#pragma once

#include <Lsc/IOString/CArguments.h>
//#include <Lsc/IOString/Expect.h>
#include <Lsc/IOCon/IOCon.h>

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

        [[maybe_unused]] bool HasArgs() const;
        [[maybe_unused]] Expect<> InitArguments();
        [[maybe_unused]] Expect<> TestIOCon();
        [[maybe_unused]] Expect<> TestWidgetCellColours();
        [[maybe_unused]] Expect<> TestWidget();

    };

}