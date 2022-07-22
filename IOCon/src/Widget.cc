#include <Lsc/IOCon/Widget.h>

namespace Lsc
{

    Widget::List Widget::_mpsTheWidgetsGarbage;

    Widget::Widget(Widget *aParentWidget, Widget::Flag aFlags) : _mpParent(aParentWidget), _mFlags(aFlags)
    {
        if (aParentWidget)
            SetParent(aParentWidget);
    }

    Widget::~Widget()
    {

        if (_mpBackBuffer)
            delete[] _mpBackBuffer;
    }

    int Widget::EmptyGarbage()
    {
        int sz = Widget::_mpsTheWidgetsGarbage.size();
        if (Widget::_mpsTheWidgetsGarbage.empty())
            return 0;
        for (auto *W : Widget::_mpsTheWidgetsGarbage)
        {
            delete W;
        }
        return sz;
    }

    Expect<> Widget::Detach()
    {
        if (_mpParent)
            _mpParent->DetachChild(this);
        return Message::Code::Ok;
    }

    Expect<> Widget::DetachChild(Widget *W)
    {
        if (Iterator Child; (Child = *QueryChild(W)) != _mChildren.end())
            _mChildren.erase(Child);
        else
            return Message::Code::NotExist;

        return Message::Code::Ok;
    }

    void Widget::Dispose(Widget *W)
    {
        Widget::_mpsTheWidgetsGarbage.push_back(W);
    }

    Expect<> Widget::SetParent(Widget *aParent)
    {
        _mpParent = aParent;
        if (aParent)
        {
            aParent->AppendChild(this);
        }
        return Message::Code::Ok;
    }

    Expect<> Widget::AppendChild(Widget *aWidget)
    {
        if (*QueryChild(aWidget) != _mChildren.end())
        {
            _mChildren.emplace_back(aWidget);
            return Message::Code::Accepted;
        }
        return Message::Code::Exist;
    }

    Expect<Widget::Iterator> Widget::QueryChild(Widget *aWidget)
    {
        if (_mChildren.empty())
            return _mChildren.end();

        return std::find(_mChildren.begin(), _mChildren.end(), aWidget);
    }

    Widget::Cell::Cell(Widget::Cell::Type aCell) : C(aCell) {}
    Widget::Cell &Widget::Cell::SetAttribute(Widget::Cell::Type aAttr)
    {
        C &= ~ATMask | aAttr;
        return *this;
    }

    uint16_t Widget::Cell::Attributes()
    {
        return (C & ATMask) >> ATShift;
    }

#pragma region Operations
    Widget::Cell &Widget::Cell::SetFg(Color::Type aFg)
    {
        C &= ~FGMask | static_cast<uint16_t>(aFg) << FGShift;
        return *this;
    }

    Widget::Cell &Widget::Cell::SetBg(Color::Type aBg)
    {
        C &= ~BGMask | static_cast<uint16_t>(aBg) << BGShift;
        return *this;
    }

    Color::Type Widget::Cell::Fg()
    {
        return static_cast<Color::Type>((C & FGMask) >> FGShift);
    }

    Color::Type Widget::Cell::Bg()
    {
        return static_cast<Color::Type>((C & BGMask) >> BGShift);
    }
    uint16_t Widget::Cell::Char()
    {
        return C & CharMask;
    }

    Widget::Cell &Widget::Cell::operator<<(uint16_t aCharacter)
    {
        C &= ~CharMask | aCharacter;
        return *this;
    }

    Expect<> Widget::SetGeometry(Point aPt, Size aSz)
    {
        _mR.Assign(aPt, aSz);
        if (_mpBackBuffer)
            delete[] _mpBackBuffer;

        WAlloc();
        return Message::Code::Ok;
    }

    Expect<> Widget::Clear()
    {
        if (!_mpBackBuffer)
            return Message::Code::NullPtr;

        auto Count = _mR.Width() * _mR.Height();
        auto B = _mpBackBuffer;
        Cell C(*_mpBackBuffer);
        C << 0x20;
        for (uint64_t x = 0l; x < Count; x++)
            *B++ = C.C;

        return Message::Code::Ok;
    }

    Expect<Widget::Cell::Type *> Widget::WAlloc()
    {
        if (!_mR)
            throw Message::Fatal(SourceLocation) << Message::Code::NullPtr << " Cannot allocate Internal backbufffer memory on invalid geometry";

        auto Count = _mR.Width() * _mR.Height();
        _mpBackBuffer = new Cell::Type[Count + _mR.Width()];
        Cell C;
        C.SetFg(Color::Type::Grey);
        C.SetBg(Color::Type::Black);
        C << 0x20;
        auto B = _mpBackBuffer;
        for (uint64_t x = 0l; x < Count; x++)
            *B++ = C.C;

        return _mpBackBuffer;
    }

#pragma endregion Operations
}