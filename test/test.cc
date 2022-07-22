//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"
#include <Lsc/IOString/String.h>

namespace Lsc
{

    Test::Test(int argc, char **argv) : argc(argc), argv(argv)
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
            Str << Color::BlueViolet << argv[0] << Color::Yellow << " ==> Arg count: " << Color::CadetBlue2 << argc << "...\n";
            Str << Color::White << argv[1];
            Message::Debug(SourceLocation) << Str();
            //...
            for (int i{1}; i < argc; i++)
                Message::Output() << Color::Aqua << "Arg #" << Color::White << i << ": " << Color::Yellow << argv[i] << Color::Reset;
        }
        else
            return Message::Error() << Message::Code::Expected << " Arguments to the program";

        TestIOCon();
        TestWidgetCellColours();
        return Message::Code::Accepted;
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
        Str << S.ToString() << Color::Reset;
        std::cout << Str() << '\n';

        Str = "[%08b]";
        Str << 0x20;
        Message::Debug(SourceLocation) << Color::White << "0x20:  " << Str() << " ;";
        Str = "Cell: [%08b]";
        Widget::Cell C{0x20};
        Str << C.C;
        Message::Debug(SourceLocation) << Color::White << Str() << " ;";
        return Message::Code::Ok;
    }

    Expect<> Test::TestWidgetCellColours()
    {
        Message::Debug(SourceLocation) << " Testing Cell contents bits:";

        Widget::Cell Cell{0x20};
        Cell.SetFg(Color::BlueViolet).SetBg(Color::LightCyan3) << 'A';
        Message::Output() << "Cell Attributes:" << Color::BlueViolet
                          << " BlueViolet " << Color::Reset << " Bg: " << Color::AnsiBack(Color::Cyan3) << "  Cyan3  " << Color::Reset << ':';
        String Str = "Bits: [%08b]";
        Str << Cell.C;
        Message::Output() << Str();
        Message::Output() << " To be continued ( Fg() & Bg() )";
        return Message::Code::Ok;
    }

}