#include <Lsc/IOCon/Widget.h>
#include <Lsc/IOCon/IOCon.h>

namespace Lsc
{

    Widget::List Widget::_mpsTheWidgetsGarbage;


    Widget::Widget(Widget* aParentWidget, Widget::WFlags FlagBits)
    {
        if (aParentWidget)
            SetParent(aParentWidget);
        _mFlags = FlagBits;
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
        C = (C & ~ATMask) | aAttr;
        return *this;
    }
    Widget::Cell &Widget::Cell::operator=(Widget::Cell::Type aC)
    {
        C = aC;
        return *this;
    }

    Widget::Cell &Widget::Cell::operator=(Widget::Cell::Type *aC)
    {
        C = *aC;
        return *this;
    }

    uint16_t Widget::Cell::Attributes()
    {
        return (C & ATMask) >> ATShift;
    }

#pragma region CellOperations
    Widget::Cell &Widget::Cell::SetFg(Color::Type aFg)
    {
        C = (C & ~FGMask) | (static_cast<uint64_t>(aFg) << FGShift);
        return *this;
    }

    Widget::Cell &Widget::Cell::SetBg(Color::Type aBg)
    {
        C = (C & ~BGMask) | (static_cast<uint64_t>(aBg) << BGShift);
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
        C = (C & ~CharMask) | aCharacter;
        return *this;
    }

    Expect<> Widget::SetGeometry(Point aPt, Size aSz)
    {
        _mR.Assign(aPt, aSz);
        Message::Debug(SourceLocation) << " Dimensions: " << _mR.ToString();
        if (_mpBackBuffer)
            delete[] _mpBackBuffer;

        Message::Output() << " Now Allocating the backbuffer";
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

    Expect<> Widget::Update()
    {
        Console::RenderWidget(this);
        return Message::Code::Ok;
    }

    Expect<Widget::Cell::Type *> Widget::WAlloc()
    {
        if (!_mR)
            throw Message::Fatal(SourceLocation) << Message::Code::NullPtr << " Cannot allocate Internal backbufffer memory on invalid geometry";

        if (_mFlags & Widget::WFloating)
        {
            auto Count = _mR.Width() * _mR.Height();
            _mpBackBuffer = new Cell::Type[Count + _mR.Width()];
            Cell C;
            C.SetFg(Color::Type::Grey);
            C.SetBg(Color::Type::White);
            C << 0x20;
            auto B = _mpBackBuffer;
            for (uint64_t x = 0l; x < Count; x++)
                *B++ = C.C;
        }
        else {
            //...
        }

        return _mpBackBuffer;
    }

    Widget::Cell &Widget::Cell::ResetAttributes(Widget::Cell::Type Bits)
    {
        C = (C & CharMask) | Bits;
        return *this;
    }

#pragma endregion CellOperations
}