cmake_minimum_required(VERSION 3.16.0)

project(sandbox VERSION 1.0 DESCRIPTION "Omni Engine Example: Sandbox")

list(APPEND H_SANDBOX_FILES
)

list(APPEND C_SANDBOX_FILES
    #*.cpp
    WinMain.cpp
)

add_executable(${PROJECT_NAME}
    WIN32
    ${H_SANDBOX_FILES}
    ${C_SANDBOX_FILES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

source_group(TREE ${PROJECT_SOURCE_DIR} 
    FILES 
    ${H_SANDBOX_FILES}
    ${C_SANDBOX_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    omni
)
