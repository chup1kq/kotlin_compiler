cmake_minimum_required(VERSION 3.10)
project(kotlin_compiler CXX)

set(CMAKE_CXX_STANDARD 20)

find_program(FLEX_EXECUTABLE NAMES win_flex)
find_program(BISON_EXECUTABLE NAMES win_bison)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/lexer-build)
set(LEXER_CPP ${BUILD_DIR}/lexer.cpp)
set(PARSER_CPP ${BUILD_DIR}/parser.cpp)
set(PARSER_H ${BUILD_DIR}/parser.hpp)

FLEX_TARGET(Lexer
        ${CMAKE_SOURCE_DIR}/lexer.l
        ${LEXER_CPP}
)

BISON_TARGET(Parser
        ${CMAKE_SOURCE_DIR}/grammar.y
        ${PARSER_CPP}
        DEFINES_FILE ${PARSER_H}
        COMPILE_FLAGS "--report=all --verbose"
)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src/lexer)

add_executable(kotlin_parser
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        src/lexer/TopLevelDeclarationProcessor.cpp
        src/lexer/TopLevelDeclarationProcessor.h
)
