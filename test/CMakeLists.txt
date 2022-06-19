cmake_minimum_required(VERSION 3.16)

#I keep bare minimum of the QT5 or 6+ components framework access in cmake.
# They are disabled in this template.

project(Test.app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#find_package(Qt5 COMPONENTS Widgets REQUIRED)


# Only way I found for assemble each of the subproject's include dir.
list(
    APPEND INCLUDE_PATHS
    "${IOSTR_INCLUDE_DIR}"
#    "${CLOG_INCLUDE_DIR}"
#    "${ANSIUI_INCLUDE_DIR}"
#    "${DLLOADER_INCLUDE_DIR}"
#    "${XIO_INCLUDE_DIR}"
)



add_executable(${PROJECT_NAME}
#	components.ui
#    mainwindow.h
#    mainwindow.cc
    test.cc
    main.cc
#    resources.qrc
)


target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${INCLUDE_PATHS}>"
        )


target_link_libraries(${PROJECT_NAME} iostr)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )

enable_testing()


include(CTest)

add_test(NAME WhereAreTheArgs  COMMAND $<TARGET_FILE:Test.app> )
set_property(TEST WhereAreTheArgs PROPERTY WILL_FAIL 1)
add_test(NAME String:CTor-DTor COMMAND $<TARGET_FILE:Test.app> String:CTor-DTor )
add_test(NAME String:CArgumentList COMMAND $<TARGET_FILE:Test.app> String:CArgumentList Arg1="Arg1-Value" Arg2 Arg3:*.*)


add_custom_command(TARGET Test.app
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> -V )
