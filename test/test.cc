//
// Created by oldlonecoder on 6/18/22.
//

#include "test.h"
#include <Lsc/IOString/String.h>
#include <Lsc/IOCon/Painter.h>


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
            Message::Warning() << Message::Code::Expected << " Arguments to the program. Continuing with the program...";

        TestWidget();
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

    Expect<> Test::TestWidget()
    {
        Widget* W = new Widget(nullptr,Widget::WTopLevel|Widget::WFloating);
        W->SetGeometry({ 10,3 }, { { 60,20 } });
        W->Attributes().SetColor({Color::DarkBlue,Color::White }) << 0x20;
        Console::RenderWidget(W);
        Painter Pen{ W };
        
        Pen.Goto({ 2,2 });
        Pen.PutString("Test.App \\O/ <Fg:Lime; Bg:DarkBlue>  <icon: Home> Lime On DarkBlue ");
        Pen.Update();
        // ...

        Widget::Dispose(W);
        Console::Terminate();
        return Message::Code::Ok;
    }

}