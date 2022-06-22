cmake_minimum_required(VERSION 3.13..3.22 FATAL_ERROR)
## Project name
set(FrameWorkName "lsc")
project(${FrameWorkName} VERSION 0.1.0 LANGUAGES CXX)

message ("**************** " ${FrameWorkName} " Master Project ********************")
#
option(BUILD_IOSTR "Enable building the common string utilities (IOStr) library" ON)
#option(BUILD_LOGGER "Enable building the c-logger sub module (Logger) library" ON)
#option(BUILD_ANSIUI "Enable building the ansi-ui submodule (AnsiUI)library" ON)
#option(BUILD_DLPLUGIN "Enable building the dynamic lib loader submodule (DLPlugin) library" ON)
#option(BUILD_XIO "Enable building the dynamic lib loader submodule (Scripture) library" ON)
option(BUILD_TESTS "Enable building the global tests (Test.app) application " ON)
#

SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


message("${FrameWorkName} Root Directory:" ${FRAMEWORK_ROOT})



if(BUILD_IOSTR)
    SET(IOSTR_INCLUDE_DIR ${FRAMEWORK_ROOT}/IOString/include)
    add_subdirectory(IOString)
    message ("iostr sub project:" "libIOString.{so/dll/dylib} set -> ${IOSTR_INCLUDE_DIR}")
endif()


#
#if(BUILD_LOGGER)
#    SET(LOGGER_INCLUDE_DIR ${FRAMEWORK_ROOT}/logger/include)
#    add_subdirectory(logger)
#    message ("submodule build:" "liblogger.{so/dll/dylib} set -> ${LOGGER_INCLUDE_DIR}")
#endif()
#
#if(BUILD_ANSIUI)
#    SET(ANSIUI_INCLUDE_DIR ${FRAMEWORK_ROOT}/ansi-ui/include)
#    add_subdirectory(ansi-ui)
#    message ("submodule build:" "libansiui.{so/dll/dylib} set -> ${ANSIUI_INCLUDE_DIR}")
#endif()
#
#if(BUILD_DLLOADER)
#    SET(DLLOADER_INCLUDE_DIR ${FRAMEWORK_ROOT}/dlloader/include)
#    add_subdirectory(dlloader)
#    message ("submodule build:" "libdlloader.{so/dll/dylib} set -> ${DLLOADER_INCLUDE_DIR}")
#endif()
#
#if(BUILD_XIO)
#    SET(XIO_INCLUDE_DIR ${FRAMEWORK_ROOT}/xio/include)
#    add_subdirectory(xio)
#    message ("submodule build:" "libxio.{so/dll/dylib} set -> ${XIO_INCLUDE_DIR}")
#endif()
#
if(BUILD_TESTS)
    add_subdirectory(test)
    message ("test application set to build")
endif()

#
#
#IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
#	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
#			${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
#			)
#ENDIF()
#
#
## uninstall target
#if(NOT TARGET uninstall)
#  configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#    IMMEDIATE @ONLY)
#
#  add_custom_target(uninstall
#    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
#endif()
