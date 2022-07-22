//
// Created by oldlonecoder on 21-11-20.
//

#pragma once

#include <Lsc/IOCon/dllexport.h>
#include <Lsc/IOCon/Geometry.h>
#include <Lsc/IOString/Expect.h>
#include <Lsc/IOCon/Widget.h>

namespace Lsc
{

    /**
     * @brief Screen Console IO Buffer
     *
     *
     * @author oldlonecoder. lussier.serge@gmail.com
     */
    class IOCON_LIB Console
    {
        // No need for a full Rectangle functions here. Just need the screen/console dimensions. Just create inline Rectangle for computations
        Size _mWh;
        Expect<> GetScreenSize();
        Widget::List _mTopLevels;

    public:
        //...
        Expect<> Init();
        int Width() const { return _mWh.WH.X; }
        int Height() const { return _mWh.WH.Y; }
        Size Dimensions() const { return _mWh; }
        static int Terminate();
        static Expect<> RenderWidget(Widget* W);
        static Expect<> RenderScanLine(Widget* W, int LineNum);
        static Expect<> GotoXY(Point XY);

    };

}