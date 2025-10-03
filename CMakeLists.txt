cmake_minimum_required(VERSION 3.10)
project(kotlin_lexer CXX)

set(CMAKE_CXX_STANDARD 20)

find_program(FLEX_EXECUTABLE NAMES win_flex)
find_package(FLEX REQUIRED)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/lexer-build)
set(LEXER_CPP ${BUILD_DIR}/lexer.cpp)

FLEX_TARGET(Lexer
        ${CMAKE_SOURCE_DIR}/lexer.l
        ${LEXER_CPP}
        COMPILE_FLAGS "--nounput --noline --nomain"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

add_executable(kotlin_lexer
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${FLEX_Lexer_OUTPUTS}
)
