cmake_minimum_required(VERSION 3.16.0)

project(sandbox VERSION 1.0 DESCRIPTION "Omni Engine Example: Sandbox")

list(APPEND SANDBOX_H_FILES
)

list(APPEND SANDBOX_C_FILES
    #*.cpp
    main.cpp
)

add_executable(${PROJECT_NAME}
    ${SANDBOX_H_FILES}
    ${SANDBOX_C_FILES}
    ${STATIC_LIB_RES_FILES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

source_group(TREE ${PROJECT_SOURCE_DIR} 
    FILES 
    ${SANDBOX_H_FILES}
    ${SANDBOX_C_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(sandbox
    PRIVATE
    /MP
    /Z7
    /Ot
    /W4
    $<$<NOT:$<CONFIG:Debug>>:/GF /GL>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    omni
)

target_link_options(sandbox
    PRIVATE
    /OPT:REF
    /INCREMENTAL:NO
    $<$<CONFIG:Debug>:/OPT:NOICF /DEBUG:FASTLINK>
    $<$<CONFIG:RelWithDebInfo>:/OPT:ICF /LTCG /DEBUG:FASTLINK>
    $<$<CONFIG:Release>:/OPT:ICF /LTCG>
    $<$<CONFIG:MinSizeRel>:/OPT:ICF /LTCG>
)    
