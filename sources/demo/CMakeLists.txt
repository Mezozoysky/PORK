cmake_minimum_required(VERSION 3.13)

project(demo
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
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}"
)

set(HEADER_FILES)
set(SOURCE_FILES main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
