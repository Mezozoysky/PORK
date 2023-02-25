cmake_minimum_required(VERSION 3.13)

project(libpork
    LANGUAGES CXX
    VERSION ${PORK_VERSION}
    )

add_library(${PROJECT_NAME} STATIC)
add_library(PORK::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME machine
    )
target_link_libraries(${PROJECT_NAME}
    INTERFACE
    #    spdlog::spdlog
    PORK::common
    fmt::fmt
    )
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    #    $<TARGET_PROPERTY:spdlog::spdlog,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    )

set(HEADER_FILES
    Input.hpp
    Token.hpp
    Lexer.hpp
    Parser.hpp
    Generator.hpp
    ASTNode.hpp
    StackMachine.hpp
    Machine.hpp
    )
set(SOURCE_FILES
    Input.cpp
    Token.cpp
    Lexer.cpp
    Parser.cpp
    Generator.cpp
    ASTNode.cpp
    StackMachine.cpp
    Machine.cpp
    )

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCE_FILES}
    PUBLIC ${HEADER_FILES}
    )

if(PORK_BUILD_UT)
    add_subdirectory(ut)
endif()
