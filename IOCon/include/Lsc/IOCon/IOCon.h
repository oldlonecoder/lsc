//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_IOSTR_H
//#define VXIO_FRAMEWORK_IOSTR_H

#pragma once
#include <Lsc/IOCon/dllexport.h>
#include <Lsc/IOCon/Geometry>
#include <Lsc/IOString/Expect.h>



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

public:

    //...
    Expect<> Init();
    int Width() const { return _mWh.WH.X; }
    int Height() const { return _mWh.WH.Y; }
    Size Dimensions() const { return _mWh; }

};

}