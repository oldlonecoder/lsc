enable_testing()


include(CTest)

add_test(NAME WhereAreTheArgs  COMMAND $<TARGET_FILE:Test.app> )
set_property(TEST WhereAreTheArgs PROPERTY WILL_FAIL 10)
add_test(NAME String:CTor-DTor COMMAND $<TARGET_FILE:Test.app> String:CTor-DTor )
#set_property(TEST WhereAreTheArgs PROPERTY WILL_SUCCEED 3)
add_test(NAME String:CArgumentList COMMAND $<TARGET_FILE:Test.app> String:CArgumentList Arg1="Arg1-Value: \\{04.2f} \\{C:Yellow}" Arg2 Arg3:*.*)


add_custom_command(TARGET Test.app
        POST_BUILD
        COMMAND ctest -C $<CONFIGURATION> -V )
