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
        ${CMAKE_SOURCE_DIR}/parser.y
        ${PARSER_CPP}
        DEFINES_FILE ${PARSER_H}
        COMPILE_FLAGS "--report=all --verbose"
)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

include_directories(
        ${CMAKE_SOURCE_DIR}/src/lexer
        ${CMAKE_SOURCE_DIR}/src/nodes
        ${CMAKE_SOURCE_DIR}/src/nodes/expr
        ${CMAKE_SOURCE_DIR}/src/nodes/stmt
        ${CMAKE_SOURCE_DIR}/src/nodes/type
        ${CMAKE_SOURCE_DIR}/src/nodes/varDeclaration
        ${CMAKE_SOURCE_DIR}/src/nodes/fun
        ${CMAKE_SOURCE_DIR}/src/nodes/modifier
        ${CMAKE_SOURCE_DIR}/src/nodes/class
)

add_executable(kotlin_parser
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        src/lexer/TopLevelDeclarationProcessor.cpp
        src/lexer/TopLevelDeclarationProcessor.h
        src/nodes/node.cpp
        src/nodes/node.h
        src/nodes/types.cpp
        src/nodes/types.h
        src/nodes/expr/ExprListNode.cpp
        src/nodes/expr/ExprListNode.h
        src/nodes/stmt/StmtListNode.cpp
        src/nodes/stmt/StmtListNode.h
        src/nodes/expr/ExprNode.cpp
        src/nodes/expr/ExprNode.h
        src/nodes/stmt/StmtNode.cpp
        src/nodes/stmt/StmtNode.h
        src/nodes/type/TypeNode.h
        src/nodes/type/TypeNode.cpp
        src/nodes/varDeclaration/VarDeclaration.cpp
        src/nodes/varDeclaration/VarDeclaration.h
        src/nodes/varDeclaration/VarDeclarationList.cpp
        src/nodes/varDeclaration/VarDeclarationList.h
        src/nodes/fun/FunNode.cpp
        src/nodes/fun/FunNode.h
        src/nodes/class/ClassNode.cpp
        src/nodes/class/ClassNode.h
        src/nodes/class/members/ClassBodyNode.cpp
        src/nodes/class/members/ClassBodyNode.h
        src/nodes/modifier/ModifierMap.cpp
        src/nodes/modifier/ModifierMap.h
        src/nodes/class/Inheritance.cpp
        src/nodes/class/Inheritance.h
        src/nodes/class/members/Constructor.cpp
        src/nodes/class/members/Constructor.h
        src/nodes/class/members/enumEntry/EnumEntry.cpp
        src/nodes/class/members/enumEntry/EnumEntry.h
        src/nodes/class/members/enumEntry/EnumEntryList.cpp
        src/nodes/class/members/enumEntry/EnumEntryList.h
        src/nodes/topLevel/KotlinFileNode.cpp
        src/nodes/topLevel/KotlinFileNode.h
        src/nodes/topLevel/KotlinElementList.cpp
        src/nodes/topLevel/KotlinElementList.h
        src/utils/output/dot.cpp
        src/nodes/class/members/ConstructorArgs.cpp
        src/nodes/class/members/ConstructorArgs.h
        src/semantic/error/SemanticError.cpp
        src/semantic/error/SemanticError.h
        src/semantic/error/SemanticErrorCode.h
        src/semantic/tables/class/ClassTable.cpp
        src/semantic/tables/class/ClassTable.h
        src/semantic/tables/class/ClassTableElement.cpp
        src/semantic/tables/class/ClassTableElement.h
        src/semantic/tables/field/FieldTable.h
        src/semantic/tables/field/FieldTableElement.h
        src/semantic/tables/constant/ConstantTable.cpp
        src/semantic/tables/constant/ConstantTable.h
        src/semantic/tables/constant/ConstantTableElement.cpp
        src/semantic/tables/constant/ConstantTableElement.h
        src/utils/output/constTableToCsv.cpp
        src/utils/math/math.cpp
        src/semantic/tables/method/MethodTable.h
        src/semantic/tables/method/MethodTableElement.h
        src/semantic/tables/variables/TypeTable.h
        src/semantic/tables/variables/FuncParam.h
        src/semantic/tables/variables/LocalVariableTable.h
        src/semantic/tables/variables/TypeNodeType.h
        src/semantic/tables/variables/NodeDescription.h
        src/semantic/transformations/literals/LiteralsTransformator.cpp
        src/semantic/transformations/literals/LiteralsTransformator.h
        src/semantic/transformations/operators/OperatorsTransformator.cpp
        src/semantic/transformations/operators/OperatorsTransformator.h
        src/semantic/transformations/attributing/javaRTLTypesTransformator/JavaRTLTypesTransformator.cpp
        src/semantic/transformations/attributing/javaRTLTypesTransformator/JavaRTLTypesTransformator.h
)
