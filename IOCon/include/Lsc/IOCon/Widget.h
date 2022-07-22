#pragma once

#include <Lsc/IOCon/dllexport.h>
#include <Lsc/IOString/Encoding/Color.h>
#include <Lsc/IOCon/Geometry.h>
#include <Lsc/IOString/Expect.h>

namespace Lsc
{

    class IOCON_LIB Widget
    {
        Rectangle _mR;
        Widget *_mpParent = nullptr;

    public:
        using List = std::vector<Widget *>;
        using Iterator = Widget::List::iterator;
        using ConstIterator = Widget::List::const_iterator;

        struct IOCON_LIB Cell
        {
            using Type = uint64_t;

            // ============== Bitmasks =============================
            static constexpr uint64_t CharMask = 0x000000000000FFFF;
            // static constexpr uint64_t WChar_B = 0x000000000000FFFF;
            static constexpr uint64_t FGMask = 0x00000000FFFF0000;
            static constexpr uint64_t BGMask = 0x0000FFFF00000000;
            static constexpr uint64_t ATMask = 0xFFFF000000000000;
            static constexpr uint64_t AttrMask = 0xFFFFFFFFFFFF0000;

            static constexpr int FGShift = 0x10;
            static constexpr int BGShift = 0x20;
            static constexpr int ATShift = 0x40;
            // =====================================================

            Widget::Cell::Type C = 0l;
            Cell() = default;
            Cell(Widget::Cell::Type aCell);
            ~Cell() = default;

            Cell &SetFg(Color::Type aFg);
            Cell &SetBg(Color::Type aBg);
            Cell &SetAttribute(Widget::Cell::Type aAttr);

            Cell &operator=(Widget::Cell::Type aC);
            Cell &operator=(Widget::Cell::Type *aC);

            Color::Type Fg();
            Color::Type Bg();
            Cell& SetColor(Color::Set aSet)
            {
                SetFg(aSet.Fg);
                SetBg(aSet.Bg);
                return *this;
            }

            uint16_t Char();
            uint16_t Attributes();

            Widget::Cell &operator<<(uint16_t aCharacter);
            Cell &ResetAttributes(Widget::Cell::Type Bits);
        };

        struct IOCON_LIB Flag
        {
            uint16_t TopLevel : 1;
            uint16_t unused : 15;
            //...
        };

        Widget() = default;
        Widget(Widget *aParentWidget, Widget::Flag aFlags = {1});
        virtual ~Widget();
        //...
        Expect<> SetParent(Widget *aParent);
        Expect<> AppendChild(Widget *aWidget);
        Expect<Widget::Iterator> QueryChild(Widget *aWidget);
        static void Dispose(Widget *W);
        Expect<> Detach();
        Expect<> DetachChild(Widget *W);

        int Width() { return _mR.Width(); };
        int Height() { return _mR.Height(); }

        virtual Expect<> SetGeometry(Point aPt, Size aSz);
        virtual Expect<> Clear();
        bool TopLevel() { return _mFlags.TopLevel == 1; }

        /// <summary>
        /// For now assume everything is valid...
        /// </summary>
        /// <param name="XY"></param>
        /// <returns>Widget::Cell::Type Pointer to the address into the backbuffer </returns>

        Widget::Cell::Type *PeekXY(Point XY)
        {
            return _mpBackBuffer + (XY.Y * _mR.Width() + XY.X); // sizeof(Widget::Cell::Type);
        }

    protected:
        // Internal Management and Operations
        virtual Expect<Widget::Cell::Type *> WAlloc();

    private:
        friend class Console;
        Cell::Type *_mpBackBuffer = nullptr; ///< OWned and managed by Toplevel Widgets
        Flag _mFlags = {0};
        Widget::List _mChildren;
        static Widget::List _mpsTheWidgetsGarbage;
        static int EmptyGarbage();
        Cell _mAttributes;
    };
}
