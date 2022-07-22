#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

#include <Lsc/IOCon/IOCon.h>


namespace Lsc
{


Expect<> Console::GetScreenSize()
{
    int w,h;    
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    _mWh = {static_cast<int>(csbi.srWindow.Right-csbi.srWindow.Left+1), static_cast<int>(csbi.srWindow.Bottom-csbi.srWindow.Top+1)};
#elif defined(__linux__)
    struct winsize win;
    ioctl(fileno(stdout), TIOCGWINSZ, &win);
    _mWh  = {static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)};
#endif // Windows/Linux
    return Message::Code::Ok;
}


Expect<> Console::Init()
{
    auto R = GetScreenSize();
    if(!R)
        return Message::Fatal(SourceLocation) << Message::Code::Rejected << " Getting screen/console dimensions...";
    //... To be continued
    
    return Message::Code::Implement;
}

int Console::Terminate()
{
    int Count = Widget::EmptyGarbage();
    Message::Debug(SourceLocation) << " Widget release count : " << Color::Yellow << Count << Color::Reset;

    return 0;
}

Expect<> Console::RenderWidget(Widget* W)
{
    if (!W->TopLevel())
        return Message::Code::Rejected;

    Console::GotoXY(W->_mR.A);
    
    std::string str = W->_mR.A.ToString();
    std::cout << str << "This is the Result of Console::GotoXY(" << str << ")...\n";
    
    for (int Y = 0; Y < W->Height(); Y++) Console::RenderScanLine(W, Y);

    Message::Debug(SourceLocation) << " Check Console::GotoXY... (coords):" << W->_mR.A.ToString();
    return Expect<>();
}


Expect<> Console::RenderScanLine(Widget* W, int LineNum)
{
    Widget::Cell::Type* C = W->PeekXY({ 0,0 });
    Widget::Cell::Type* P = C;
    Widget::Cell Cell{*C};
    std::cout << Color::AnsiBack(Cell.Bg()) << Color::Ansi(Cell.Fg());
    Widget::Cell PCell{ *P };
    for (int X = 1; X < W->_mR.Width(); X++)
    {
        PCell.C = *P;
        Cell.C = *C;
        if (PCell.Bg() != Cell.Bg())
            std::cout << Color::AnsiBack(Cell.Bg());
        if (PCell.Fg() != Cell.Fg())
            std::cout << Color::Ansi(Cell.Fg());
        std::cout << (char)(*C & Widget::Cell::CharMask);
        P = C++;      
    }

    return Message::Code::Accepted;
}

Expect<> Console::GotoXY(Point XY)
{
    std::cout << "\033[" << XY.Y << ';' << XY.X << "H";
    std::flush(std::cout);
    return Message::Code::Accepted;
}

}