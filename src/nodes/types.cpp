#include "types.h"

std::string modifierToString(ModifierType m) {
    switch (m) {
        case PUBLIC:    return "PUBLIC";
        case PRIVATE:   return "PRIVATE";
        case PROTECTED: return "PROTECTED";
        case OPEN:      return "OPEN";
        case FINAL:     return "FINAL";
        case OVERRIDE:  return "OVERRIDE";
        case NONE:      return "NONE";
        default:        return "UNKNOWN";
    }
}

std::string exprTypeToString(ExprType t) {
    switch (t) {
        case _NULL_LITERAL:       return "NULL_LITERAL";
        case _IDENTIFIER:         return "IDENTIFIER";
        case _BOOL_LITERAL:       return "BOOL_LITERAL";
        case _INTEGER_LITERAL:    return "INTEGER_LITERAL";
        case _FLOAT_LITERAL:      return "FLOAT_LITERAL";
        case _DOUBLE_LITERAL:     return "DOUBLE_LITERAL";
        case _CHAR_LITERAL:       return "CHAR_LITERAL";
        case _STRING_LITERAL:     return "STRING_LITERAL";
        case _PLUS:               return "PLUS";
        case _MINUS:              return "MINUS";
        case _DIV:                return "DIV";
        case _MUL:                return "MUL";
        case _MOD:                return "MOD";
        case _LESS:               return "LESS";
        case _GREAT:              return "GREAT";
        case _LESS_EQUAL:         return "LESS_EQUAL";
        case _GREAT_EQUAL:        return "GREAT_EQUAL";
        case _EQUAL:              return "EQUAL";
        case _NOT_EQUAL:          return "NOT_EQUAL";
        case _FUNCTION_CALL:      return "FUNCTION_CALL";
        case _BRACKETS:           return "BRACKETS";
        case _ASSIGNMENT:         return "ASSIGNMENT";
        case _PLUS_ASSIGNMENT:    return "PLUS_ASSIGNMENT";
        case _MINUS_ASSIGNMENT:   return "MINUS_ASSIGNMENT";
        case _DIV_ASSIGNMENT:     return "DIV_ASSIGNMENT";
        case _MUL_ASSIGNMENT:     return "MUL_ASSIGNMENT";
        case _MOD_ASSIGNMENT:     return "MOD_ASSIGNMENT";
        case _THIS:               return "THIS";
        case _SUPER:              return "SUPER";
        case _RANGE:              return "RANGE";
        case _UNTIL:              return "UNTIL";
        case _STEP:               return "STEP";
        case _PREF_INCREMENT:     return "PREF_INCREMENT";
        case _PREF_DECREMENT:     return "PREF_DECREMENT";
        case _POST_INCREMENT:     return "POST_INCREMENT";
        case _POST_DECREMENT:     return "POST_DECREMENT";
        case _UNARY_PLUS:         return "UNARY_PLUS";
        case _UNARY_MINUS:        return "UNARY_MINUS";
        case _NOT:                return "NOT";
        case _DISJUNCTION:        return "DISJUNCTION";
        case _CONJUNCTION:        return "CONJUNCTION";
        case _FUNC_CALL:          return "FUNC_CALL";
        case _FIELD_ACCESS:       return "FIELD_ACCESS";
        case _SAFE_FIELD_ACCESS:  return "SAFE_FIELD_ACCESS";
        case _FUNC_ACCESS:        return "FUNC_ACCESS";
        case _SAFE_FUNC_ACCESS:   return "SAFE_FUNC_ACCESS";
        case _ARRAY_EXPR:         return "ARRAY_EXPR";
        case _ARRAY_ACCESS:       return "ARRAY_ACCESS";
        default:                  return "UNKNOWN_EXPR";
    }
}

std::string stmtTypeToString(StmtType t) {
    switch (t) {
        case _EXPRESSION: return "EXPRESSION";
        case _IF_STMT:    return "IF_STMT";
        case _WHILE:      return "WHILE";
        case _FOR:        return "FOR";
        case _DO_WHILE:   return "DO_WHILE";
        case _EMPTY:      return "EMPTY";
        case _VAR:        return "VAR";
        case _VAL:        return "VAL";
        case _RETURN:     return "RETURN";
        case _BREAK:      return "BREAK";
        case _CONTINUE:   return "CONTINUE";
        default:          return "UNKNOWN_STMT";
    }
}

std::string typeToString(Type t) {
    switch (t) {
        case _INT:       return "INT";
        case _FLOAT:     return "FLOAT";
        case _DOUBLE:    return "DOUBLE";
        case _CHAR:      return "CHAR";
        case _STRING:    return "STRING";
        case _BOOLEAN:   return "BOOLEAN";
        case _ARRAY:     return "ARRAY";
        case _ID:        return "ID";
        case _UNDEFINED: return "UNDEFINED";
        case _VOID:      return "VOID";
        default:         return "UNKNOWN_TYPE";
    }
}
