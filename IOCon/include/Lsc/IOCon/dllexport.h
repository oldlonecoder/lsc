//
// Created by oldlonecoder on 6/18/22.
//

#pragma once



#ifdef _WIN32
#   pragma warning(disable : 4996)
#   pragma warning(disable : 4005)
#   pragma warning(disable : 4267)
//class __declspec(dllimport) std::thread;
#   define _CRT_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS

#   ifdef IOCON_DLL_EXPORT
#       define IOCON_LIB __declspec(dllexport)
#   else
#       define IOCON_LIB __declspec(dllimport)
#   endif
#else
#   define IOCON_LIB   __attribute__ ((visibility ("default"))) // Not ms-Windows/MSVC
#endif

// Mandatory. Globally used where it needs to.

//---------------------------------------------


// --  Basic pseudo reflexion macros : --
#ifndef DECLOBJ_ID
#define DECLOBJ_ID \
    public:\
        virtual const char*  class_name() const noexcept; \
    private:\


#define  CLASSNAME_IMPL(CLASS) \
         const char*   CLASS::class_name() const noexcept{ return #CLASS; }
#endif
//----------------------------------------------------------------------------


#ifndef _WIN32
#   define _strdup  strdup
#   include <thread>
#   include <cstdint>
#else
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


