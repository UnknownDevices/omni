cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/min_size_rel)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/min_size_rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/min_size_rel)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/rel_with_deb_info)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/rel_with_deb_info)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/rel_with_deb_info) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(omni VERSION 1.0 DESCRIPTION "Omni Engine")
set(OMNI_VERSION ${omni_VERSION})

option(OMNI_BUILD_AS_SHARED "Build Omni as a dynamic library" OFF)

if(${OMNI_BUILD_AS_SHARED})
    set(OMNI_STATIC OFF)
    set(OMNI_SHARED ON)
else()
    set(OMNI_STATIC ON)
    set(OMNI_SHARED OFF)
endif()

find_package(magic_enum CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

#----------------------------------------------------------------------------------
# File inclusions
#----------------------------------------------------------------------------------
list(APPEND OMNI_H_FILES
    #*.hpp
    omni/entry_point.hpp
    omni/core.hpp
    omni/config.h
    omni/logger.hpp
    omni/utility/function.hpp
    omni/utility/types_pack.hpp
    omni/utility/values_pack.hpp
    omni/random/shuffle_bag.hpp
    omni/win/window.hpp
    omni/win/window_event.hpp
    omni/win/window_resources.hpp
    omni/win/incl_windows.h
)

list(APPEND OMNI_C_FILES
    #*.cpp
    omni/core.cpp
    omni/logger.cpp
    omni/win/window.cpp
    omni/win/window_resources.cpp
)

list(APPEND STATIC_LIB_RES_FILES
    #*.rc
    $<$<BOOL:${OMNI_STATIC}>:${PROJECT_SOURCE_DIR}/omni/resources.rc>
)
#----------------------------------------------------------------------------------

if(${OMNI_STATIC})
    add_library(omni 
    STATIC
    ${OMNI_H_FILES}
    ${OMNI_C_FILES}
    )
else()
    add_library(omni 
    SHARED
    ${OMNI_H_FILES}
    ${OMNI_C_FILES}
    omni/resources.rc
    )
endif()

set_property(TARGET omni PROPERTY CXX_STANDARD 23)
set_property(TARGET omni PROPERTY CXX_STANDARD_REQUIRED ON) 

add_subdirectory(examples/sandbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)

target_precompile_headers(omni
    PUBLIC
    <bitset>
    <coroutine>
    <vector>
    <array>
    <fstream>
    <format>
    <streambuf>
    <limits.h>
    <set>
    <map>
    <unordered_set>
    <unordered_map>
    <iomanip>
    <iostream>
    <sstream>
    <memory>
    <utility>
    <variant>
    <functional>
    <algorithm>
    <thread>
    <string>
    <cstring>
    <type_traits>
    <cstddef>
    <stdint.h>
    <concepts>
    <exception>
    <ranges>
)

source_group(TREE ${PROJECT_SOURCE_DIR}/omni 
    PREFIX omni/
    FILES 
    ${OMNI_H_FILES}
    ${OMNI_C_FILES}
)

target_compile_definitions(omni
    PUBLIC
    $<$<CONFIG:Debug>:OMNI_DEBUG>
    $<$<CONFIG:Release>:OMNI_RELEASE>     
    $<$<CONFIG:RelWithDebInfo>:OMNI_REL_WITH_DEB_INFO>   
    $<$<CONFIG:MinSizeRel>:OMNI_MIN_SIZE_REL>
    $<IF:$<BOOL:${OMNI_STATIC}>,OMNI_STATIC,OMNI_SHARED>
    PRIVATE
    $<$<BOOL:${OMNI_SHARED}>:OMNI_EXPORT_DLL>
)

target_include_directories(omni 
    PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(omni
    PRIVATE
    /MP
    /Z7
    /Ot
    /W4
    $<$<NOT:$<CONFIG:Debug>>:/GF /GL>
)

target_link_libraries(omni
    PUBLIC  
    spdlog::spdlog
    magic_enum::magic_enum
    D3d12.dll
    D3d11.dll
)

if(${OMNI_SHARED})
    target_link_options(omni
        PRIVATE
        /OPT:REF
        /INCREMENTAL:NO
        $<$<CONFIG:Debug>:/OPT:NOICF /DEBUG:FASTLINK>
        $<$<CONFIG:RelWithDebInfo>:/OPT:ICF /LTCG /DEBUG:FASTLINK>
        $<$<CONFIG:Release>:/OPT:ICF /LTCG>
        $<$<CONFIG:MinSizeRel>:/OPT:ICF /LTCG>
    )    
endif()

configure_file(omni_config.h.in omni_config.h)