cmake_minimum_required(VERSION 3.13)

project(pork-ut
    LANGUAGES CXX
    VERSION ${PORK_VERSION}
    )

add_executable(${PROJECT_NAME})
add_executable(PORK::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    PORK::libpork
    Catch2::Catch2WithMain
    )
target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}"
    )

set(HEADER_FILES
    )
set(SOURCE_FILES
    InputTests.cpp
    TokenTests.cpp
    LexerTests.cpp
    )

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCE_FILES}
    PUBLIC ${HEADER_FILES}
    )
catch_discover_tests(${PROJECT_NAME})

