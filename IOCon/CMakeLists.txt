cmake_minimum_required(VERSION 3.16..3.20 FATAL_ERROR)
# Project name

project(IOCon VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${PROJECT_NAME})
SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
        ${PROJECT_NAME}
        SHARED
        include/Lsc/IOCon/IOCon.h 
        src/IOCon.cc
        include/Lsc/IOCon/Geometry.h
        src/Geometry.cc
        src/Widget.cc 
        include/Lsc/IOCon/Widget.h
        src/Painter.cc 
        include/Lsc/IOCon/Painter.h
)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${PROJECT_NAME}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
			)
ENDIF()

# Only way I found for assemble each of the subproject's include dir.
list(
    APPEND INCLUDE_PATHS
    "${IOSTR_INCLUDE_DIR}"
#    "${CLOG_INCLUDE_DIR}"
    "${IOCON_INCLUDE_DIR}"
#    "${DLLOADER_INCLUDE_DIR}"
#    "${XIO_INCLUDE_DIR}"
)

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${INCLUDE_PATHS}"
        $<INSTALL_INTERFACE:include/${FrameWorkName}/${PROJECT_NAME}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} IOString sqlite3)






install(DIRECTORY
        include/${FrameWorkName}/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${FrameWorkName}/${PROJECT_NAME}"
)

install(TARGETS ${PROJECT_NAME}
		EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
