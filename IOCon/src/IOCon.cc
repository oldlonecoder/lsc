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

}