%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <cstdlib>
    #include <algorithm>

    #include "TopLevelDeclarationProcessor.h"

    TopLevelDeclarationProcessor processor;

    bool is_prev_lexem_end_of_stmt = true;

    void printOperator(char* o);
    void printEndOfLine();
    void printType(char* t);
    void printConstantKeyword(char* ckw);
    void printModifierKeyword(char* mkw);
    void printStatementKeyword(char* skw);
    void printKeyword(char* kw);
    std::string remove_underscore(const char* text);
%}

%x STRING
%x CHARACTER
%x MULTILINE_STRING
%x ML_COMMENT
%x SL_COMMENT

digit          [0-9]
notZeroDigit   [1-9]
binDigit       [01]
hexDigit       [0-9a-fA-F]

num            {digit}(_*{digit})*
exp            [eE][+-]{num}
number         (0|{notZeroDigit}(_*{digit})*)
binNumber      0[bB]({binDigit}(_*{binDigit})*)
hexNumber      0[xX]({hexDigit}(_*{hexDigit})*)
doubleNumber   ({num}(?:(\.{num})(?:({exp}))?|({exp}))|(\.{num})({exp})?)
floatNumber    (?:{num}(?:\.{num})?(?:{exp})?|\.{num}(?:{exp})?)[fF]
id             [a-zA-Z_][a-zA-Z0-9_]*

%%

%{
    std::string buff = "";
    int comment_depth = 0;
    int open_bracket = 0;
    int open_square_brackets = 0;
%}

\" {
    processor.foundInappropriateLexem(yytext);
    buff = "";
    BEGIN(STRING);
}

<STRING>[^\"\\\n]+                   { buff += yytext; }
<STRING,CHARACTER>\\n                   { buff += "\n"; }
<STRING,CHARACTER>\\t                   { buff += "\t"; }
<STRING,CHARACTER>\\b                   { buff += "\b"; }
<STRING,CHARACTER>\\$                   { buff += "$"; }
<STRING,CHARACTER>\\\'                  { buff += "\'"; }
<STRING,CHARACTER>\\\"                  { buff += "\""; }
<STRING,CHARACTER>\\\\                  { buff += "\\"; }
<STRING,CHARACTER>\\u[0-9a-fA-F]{4}     {
    char ch = (char)strtol(yytext + strlen(yytext) - 4, NULL, 16);
    buff += ch;
}
<STRING,CHARACTER>\\. {
    std::cerr << "Error in literal. Unsupported escape sequence: " << yytext << std::endl;
    BEGIN(INITIAL);
}

<STRING>\" {
    is_prev_lexem_end_of_stmt = false;
    std::cout << "Found singleline string: " << buff << std::endl;
    BEGIN(INITIAL);

    yylval.stringLiteral = strdup(buff.c_str());
    return STRING_LITERAL;
}

<STRING,CHARACTER>\n {
    std::cerr << "Error in literal. Found newline: " << buff << std::endl;
    BEGIN(INITIAL);
}

<STRING,CHARACTER><<EOF>> {
    std::cerr << "Error in literal. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}

\' {
    processor.foundInappropriateLexem(yytext);
    buff = "";
    BEGIN(CHARACTER);
}

<CHARACTER>[^\'\\\n]+           { buff += yytext; }
<CHARACTER>\' {
    if(buff.length() == 0) {
        std::cerr << "Error in character. Empty character." << std::endl;
    }
    else if (buff.length() == 1) {
        is_prev_lexem_end_of_stmt = false;
        std::cout << "Found character: " << buff << std::endl;

        yylval.charLiteral = buff[0];
        BEGIN(INITIAL);

        return CHAR_LITERAL;
    }
    else {
        std::cerr << "Error in character. More than 1 character: " << buff << std::endl;
    }
    BEGIN(INITIAL);
}


\"\"\" {
    processor.foundInappropriateLexem(yytext);
    buff = "";
    BEGIN(MULTILINE_STRING);
}
<MULTILINE_STRING>{
    \"\"\"          {
                        is_prev_lexem_end_of_stmt = false;
                        std::cout << "Found multiline string: " << buff << std::endl;
                        BEGIN(INITIAL);

                        yylval.stringLiteral = strdup(buff.c_str());
                        return STRING_LITERAL;
                    }
    [\"]{1,2}       { buff += yytext; }
    [^"]+           { buff += yytext; }
}

<MULTILINE_STRING><<EOF>> {
    std::cerr << "Error in string. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}

"//" {
    BEGIN(SL_COMMENT);
}

<SL_COMMENT>[^\n]+ {
    if (!is_prev_lexem_end_of_stmt) {
        is_prev_lexem_end_of_stmt = false;
    }
    std::cout << "Found singleline comment: " << yytext << std::endl;
    BEGIN(INITIAL);
}

<SL_COMMENT>\n {
    if (!is_prev_lexem_end_of_stmt) {
        is_prev_lexem_end_of_stmt = false;
    }
    std::cout << "Found empty singleline comment." << std::endl;
    BEGIN(INITIAL);
}


"/*" {
    comment_depth = 1;
    buff.clear();
    BEGIN(ML_COMMENT);
}

<ML_COMMENT>{
    "/*" {
        comment_depth++;
        buff += "/*";
    }

    "*/" {
        comment_depth--;
        if (comment_depth == 0) {
            if (!is_prev_lexem_end_of_stmt) {
                is_prev_lexem_end_of_stmt = false;
            }
            std::cout << "Found multiline comment: " << buff << std::endl;
            BEGIN(INITIAL);
        } else {
            buff += "*/";
        }
    }

    [^*/]+  { buff += yytext; }
    "*"[^/] { buff += yytext; }
    "/"[^*] { buff += yytext; }

    <<EOF>> {
        std::cerr << "Error in multiline comment. Unclosed multiline comment: " << buff << std::endl;
        BEGIN(INITIAL);
    }
}

"*/"        { std::cerr << "Error in multiline comment. Has no open sequence." << std::endl; }


if          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return IF; }
else        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return ELSE; }
when        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
for         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return FOR; }
in          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return IN; }
while       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return WHILE; }
do          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return DO; }
break       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return BREAK;}
continue    { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return CONTINUE; }
downTo      { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return DOWN_TO; }
step        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return STEP; }

var         { is_prev_lexem_end_of_stmt = false; return processor.processAppropriateElement(yytext); }
val         { is_prev_lexem_end_of_stmt = false; return processor.processAppropriateElement(yytext); }

fun         { is_prev_lexem_end_of_stmt = false; return processor.processAppropriateElement(yytext); }

class       { is_prev_lexem_end_of_stmt = false; return processor.processAppropriateElement(yytext); }
this        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return THIS; }
super       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; return SUPER; }
constructor { is_prev_lexem_end_of_stmt = false; return processor.processAppropriateElement(yytext); }
enum        { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }

final       { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
open        { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
override    { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
private     { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
public      { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
protected   { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }

arrayOf     { processor.foundInappropriateLexem(yytext); printKeyword(yytext); is_prev_lexem_end_of_stmt = false; return ARRAY_OF; }

Array       { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; return ARRAY; }
Int         { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; return INT_TYPE; }
Double      { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; return DOUBLE_TYPE; }
Float       { processor.foundInappropriateLexem(yytext);printType(yytext); is_prev_lexem_end_of_stmt = false; return FLOAT_TYPE; }
String      { processor.foundInappropriateLexem(yytext);printType(yytext); is_prev_lexem_end_of_stmt = false; return STRING_TYPE; }
Char        { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; return CHAR_TYPE; }
Boolean     { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; return BOOLEAN_TYPE; }

null        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; return NULL_LITERAL; }
true        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; yylval.boolLiteral = false; return TRUE_LITERAL; }
false       { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; yylval.boolLiteral = true; return FALSE_LITERAL; }

"+"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '+'; }
"-"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '-'; }
"*"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '*'; }
"/"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '/'; }
"%"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '%'; }

"++"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return POST_INCREMENT; }
"--"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return POST_DECREMENT; }
"+="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return PLUS_ASSIGNMENT; }
"-="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return MINUS_ASSIGNMENT; }
"*="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return MUL_ASSIGNMENT; }
"/="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return DIV_ASSIGNMENT; }
"%="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return MOD_ASSIGNMENT; }

"="         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '='; }
"=="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return EQUAL; }
"!="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return NOT_EQUAL; }
"<"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '<'; }
">"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '>'; }
"&&"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return AND; }
"||"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return OR; }

"."         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '.'; }
"?."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return SAFE_CALL; }
"->"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }

"!"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '!'; }
"?"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '?'; }
":"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return ':'; }

".."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return RANGE; }
"..<"       { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return UNTIL; }
"("         { processor.foundInappropriateLexem(yytext); open_bracket += 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '('; }
")"         { processor.foundInappropriateLexem(yytext); open_bracket -= 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; return ')'; }
"{"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '{'; }
"}"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '}'; }
"["         { processor.foundInappropriateLexem(yytext); open_square_brackets += 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; return '['; }
"]"         { processor.foundInappropriateLexem(yytext); open_square_brackets -= 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; return ']'; }
";"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = true; return ';'; }

-?((_[0-9_]+|[0-9][0-9_]*_|[0-9][0-9_]*_\.[0-9_]*|[0-9][0-9_]*\._[0-9_]*)|0{digit}) { std::cout << "Error: invalid numeric literal with underscore: " << yytext << std::endl; }

{number} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.intLiteral = atoi(remove_underscore(yytext).c_str());

    std::cout << "Integer: " << yylval.intLiteral << std::endl;
    return INT_LITERAL;
}

{doubleNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.doubleLiteral = strtod(remove_underscore(yytext).c_str(), NULL);

    std::cout << "Double: " << yylval.doubleLiteral << std::endl;
    return DOUBLE_LITERAL;
}

{floatNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.floatLiteral = strtof(remove_underscore(yytext).c_str(), NULL);

    std::cout << "Float: " << yylval.floatLiteral << std::endl;
    return FLOAT_LITERAL;
}

{binNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.intLiteral = std::stoul(remove_underscore(yytext + 2), nullptr, 2);

    std::cout << "Bin number: " << yylval.intLiteral << std::endl;
    return INT_LITERAL;
}

{hexNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.intLiteral = std::stoul(remove_underscore(yytext + 2), nullptr, 16);

    std::cout << "Hex number: " << yylval.intLiteral << std::endl;
    return INT_LITERAL;
}

{id} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;

    yylval.identifier = (char *)malloc(strlen(yytext)+1); strcpy(yylval.identifier, yytext);

    std::cout << "Identificator: " << yylval.identifier << std::endl;
    return ID;
}

\n+ {
    int c;
    int c_next;
    while ((c = yyinput()) == '\n' || c == ' ' || c == '\t') {
        ;
    }

    if (c == '/') {
        c_next = yyinput();
        if (c_next == '/') {
            BEGIN(SL_COMMENT);
        }
        else if (c_next == '*') {
            comment_depth = 1;
            buff.clear();
            BEGIN(ML_COMMENT);
        }
    }
    else if (c == '.') {
        unput(c);
    } else if (c == '&') {
        c_next = yyinput();
        if (c_next == '&') {
            unput(c_next);
            unput(c);
        }
        else {
            unput(c);
        }
    } else if (c == '|') {
        c_next = yyinput();
        if (c_next == '|') {
            unput(c_next);
            unput(c);
        }
        else {
            unput(c);
        }
    } else if (c == '!'){
        unput(c);
    } else if (c == '='){
        unput(c);
    } else if (c == ':'){
        unput(c);
    } else if (c == '?'){
        unput(c);
    } else if (c == '?') {
        c_next = yyinput();
        if (c_next == '.') {
            unput(c_next);
            unput(c);
        }
        else {
            unput(c);
        }
    } else {
        if ((open_bracket == 0) && (open_square_brackets == 0)) {
            if (!is_prev_lexem_end_of_stmt) {
                printEndOfLine();
                unput(c);
                return ENDL;
            }
        }
        unput(c);
    }
}

[ \t]+                  { ; }

.                       { std::cout << "Error: Found inappropriate lexem: " << yytext << std::endl; }

";;"                    { std::cout << "Unexpected sequence: " << yytext << std::endl; }

%%

void printOperator(char* o) {
    std::cout << "Operator: " << o << std::endl;
}

void printEndOfLine() {
    std::cout << "ENDL" << std::endl;
}

void printType(char* t) {
    std::cout << "Type: " << t << std::endl;
}

void printConstantKeyword(char* ckw) {
    std::cout << "Constant keyword: " << ckw << std::endl;
}

void printModifierKeyword(char* mkw) {
    std::cout << "Modifier keyword: " << mkw << std::endl;
}

void printStatementKeyword(char* skw) {
    std::cout << "Statement keyword: " << skw << std::endl;
}

void printKeyword(char* kw) {
    std::cout << "Keyword: " << kw << std::endl;
}

std::string remove_underscore(const char* text) {
    std::string s(text);
    s.erase(std::remove(s.begin(), s.end(), '_'), s.end());
    return s;
}
