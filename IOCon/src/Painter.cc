
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
        return *this;
    }

    void Painter::PutWChar(uint16_t WChar)
    {
    }
    void Painter::PutC(char C)
    {
    }
    void Painter::PutString(const String &Str)
    {
    }
    void Painter::Clear(Color::Type aBg, Color::Type aFg, uint16_t aChar)
    {
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