
#include <Lsc/IOCon/Painter.h>

namespace Lsc
{

    Painter &Painter::Reset()
    {
        Pen.ResetAttributes(Attr);
        return *this;
    }

    Painter &Painter::Goto(Point Pt)
    {
        Cursor = Pt; // No checks for now; Cursor = Pt.Y * UserWidget.Width() + Pt.X;
        return *this;
    }

    void Painter::PutWChar(uint16_t WChar)
    {
        Pen.C = (Pen.C & ~Widget::Cell::CharMask) | WChar;
    }

    void Painter::PutC(char C)
    {
        Pen.C = (Pen.C & ~Widget::Cell::CharMask) | C;
        *UserWidget->PeekXY(Cursor) = Pen.C;
    }

    void Painter::PutString(const String &Str)
    {
        Widget::Cell::Type *C = UserWidget->PeekXY(Cursor);

        for(auto c : Str()) *C++ = (*C & ~Widget::Cell::CharMask) | c;
        Cursor += {static_cast<int>(Str.Length()),0};
    }


    void Painter::Clear(Color::Type aBg, Color::Type aFg, uint16_t aChar)
    {
        Widget::Cell
    }
    void Painter::SetFg(Color::Type aFg)
    {
    }
    void Painter::SetBg(Color::Type aBg)
    {
    }
    void Painter::Home()
    {
    }
}