cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(omni VERSION 1.0 DESCRIPTION "Omni Engine")

option(OMNI_GEN_EXAMPLE_SANDBOX "Add subdirectory examples/sandbox" ON)

set(OMNI_VERSION ${omni_VERSION})

if(WIN32)
    set(OMNI_GEN_FOR_WINDOWS_PLATFORM TRUE)
else()
    message(FATAL_ERROR "Omni only supports Windows for the time being!")
endif()

#----------------------------------------------------------------------------------
# File inclusions
#----------------------------------------------------------------------------------
list(APPEND H_OMNI_FILES
    #*.hpp
    omni/entry_point.hpp
    omni/core.hpp
    omni/logger.hpp
    omni/dx12_window.hpp
    omni/utility/function.hpp
    omni/utility/types_pack.hpp
    omni/utility/values_pack.hpp
    omni/random/shuffle_bag.hpp
)

list(APPEND C_OMNI_FILES
    #*.cpp
    omni/dx12_window.cpp
    omni/core.cpp
    omni/logger.cpp
)

if(OMNI_GEN_FOR_WINDOWS_PLATFORM)
    list(APPEND H_OMNI_FILES
    )

    list(APPEND C_OMNI_FILES
    )
endif()

#----------------------------------------------------------------------------------

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} 
    SHARED
    ${H_OMNI_FILES}
    ${C_OMNI_FILES}
    omni/resources.rc
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON) 

add_subdirectory(vendor/spdlog)
set_property(TARGET spdlog PROPERTY CXX_STANDARD 23)
set_property(TARGET spdlog PROPERTY CXX_STANDARD_REQUIRED ON) 

if(OMNI_GEN_EXAMPLE_SANDBOX)
    add_subdirectory(examples/sandbox)
endif()

target_precompile_headers(${PROJECT_NAME} 
    PUBLIC
    <Windows.h>
    <coroutine>
    <vector>
    <array>
    <fstream>
    <format>
    <streambuf>
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
    ${H_OMNI_FILES}
    ${C_OMNI_FILES}
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    $<$<CONFIG:Debug>:OMNI_DEBUG>
    $<$<CONFIG:Release>:OMNI_RELEASE>     
    PRIVATE
    OMNI_BUILD_DLL
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC  
    spdlog::spdlog
)

configure_file(omni_config.h.in omni_config.h)