#ifndef KOTLIN_COMPILER_TYPES_H
#define KOTLIN_COMPILER_TYPES_H
#include <string>

enum ModifierType {
    PUBLIC,
    PRIVATE,
    PROTECTED,
    OPEN,
    FINAL,
    OVERRIDE,
    NONE
};

enum ExprType {
    _NULL_LITERAL,
    _IDENTIFIER,
    _BOOL_LITERAL,
    _INTEGER_LITERAL,
    _FLOAT_LITERAL,
    _DOUBLE_LITERAL,
    _CHAR_LITERAL,
    _STRING_LITERAL,
    _PLUS,
    _MINUS,
    _DIV,
    _MUL,
    _MOD,
    _LESS,
    _GREAT,
    _LESS_EQUAL,
    _GREAT_EQUAL,
    _EQUAL,
    _NOT_EQUAL,
    _FUNCTION_CALL,
    _BRACKETS,
    _ASSIGNMENT,
    _PLUS_ASSIGNMENT,
    _MINUS_ASSIGNMENT,
    _DIV_ASSIGNMENT,
    _MUL_ASSIGNMENT,
    _MOD_ASSIGNMENT,
    _THIS,
    _SUPER,
    _RANGE,
    _DOWN_TO,
    _UNTIL,
    _STEP,
    _PREF_INCREMENT,
    _PREF_DECREMENT,
    _POST_INCREMENT,
    _POST_DECREMENT,
    _UNARY_PLUS,
    _UNARY_MINUS,
    _NOT,
    _DISJUNCTION,
    _CONJUNCTION,
    _FUNC_CALL,
    _FIELD_ACCESS,
    _SAFE_FIELD_ACCESS,
    _FUNC_ACCESS,
    _SAFE_FUNC_ACCESS,
    _ARRAY_EXPR,
    _ARRAY_ACCESS,
};

enum StmtType {
    _IF_STMT,
    _EXPRESSION,
    _WHILE,
    _FOR,
    _DO_WHILE,
    _EMPTY,
    _VAR,
    _VAL,
    _RETURN,
    _BREAK,
    _CONTINUE,
};

enum Type {
    _INT,
    _FLOAT,
    _DOUBLE,
    _CHAR,
    _STRING,
    _BOOLEAN,
    _ARRAY,
    _ID,
    _UNDEFINED,
    _VOID
};

enum BaseLiteral {
    _FROM_NONE,
    _FROM_INT,
    _FROM_DOUBLE,
    _FROM_BOOLEAN,
    _FROM_CHAR,
    _FROM_STRING,
    _FROM_UNIT,
};


std::string modifierToString(ModifierType m);
std::string exprTypeToString(ExprType t);
std::string stmtTypeToString(StmtType t);
std::string typeToString(Type t);


#endif //KOTLIN_COMPILER_TYPES_H