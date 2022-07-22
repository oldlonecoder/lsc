

#pragma once
#include <Lsc/IOCon/Widget.h>

namespace Lsc
{
    struct IOCON_LIB Painter
    {
        Widget *UserWidget = nullptr; ///< Callee Widget
        Point Pos{0, 0};
        Rectangle UserClient;
        Widget::Cell Pen{0x20};

        Widget::Cell::Type Attr = 0;

        Point Cursor{0, 0};

        Painter &Goto(Point Pt);
        void PutWChar(uint16_t WChar);
        void PutC(char C);
        void PutString(const String &Str);
        void Clear(Color::Type aBg, Color::Type aFg, uint16_t aChar = 0x20);
        void SetFg(Color::Type aFg);
        void SetBg(Color::Type aBg);
        void Home();
        Painter &Reset();
        //...
    };
}
// namespace Lsc
