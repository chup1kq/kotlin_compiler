%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <cstdlib>
    #include <algorithm>

    #include "TopLevelDeclarationProcessor.h"

    TopLevelDeclarationProcessor processor;

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
    bool is_prev_lexem_end_of_stmt = true;
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


if          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
else        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
when        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
for         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
in          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
while       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
do          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
break       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
continue    { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
downTo      { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
step        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }

var         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
val         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }

fun         { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = false; }

class       { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = false; }
this        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
super       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
constructor { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = false; }
enum        { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }

final       { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
open        { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
override    { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
private     { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
public      { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }
protected   { processor.processAppropriateElement(yytext); is_prev_lexem_end_of_stmt = true; }

arrayOf     { processor.foundInappropriateLexem(yytext); printKeyword(yytext); is_prev_lexem_end_of_stmt = false; }

Array       { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; }
Int         { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; }
Double      { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; }
Float       { processor.foundInappropriateLexem(yytext);printType(yytext); is_prev_lexem_end_of_stmt = false; }
String      { processor.foundInappropriateLexem(yytext);printType(yytext); is_prev_lexem_end_of_stmt = false; }
Char        { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; }
Boolean     { processor.foundInappropriateLexem(yytext); printType(yytext); is_prev_lexem_end_of_stmt = false; }

null        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
true        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; }
false       { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); is_prev_lexem_end_of_stmt = false; }

"+"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"-"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"*"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"/"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }

"++"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"--"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"+="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"-="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"*="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false;}
"/="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false;}

"="         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"=="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"!="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"<"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
">"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"&&"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"||"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }

"."         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"?."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"->"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }

".."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"..<"       { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"("         { processor.foundInappropriateLexem(yytext); open_bracket += 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
")"         { processor.foundInappropriateLexem(yytext); open_bracket -= 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"{"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"}"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"["         { processor.foundInappropriateLexem(yytext); open_square_brackets += 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
"]"         { processor.foundInappropriateLexem(yytext); open_square_brackets -= 1; printOperator(yytext); is_prev_lexem_end_of_stmt = false; }
";"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); is_prev_lexem_end_of_stmt = true; }

-?((_[0-9_]+|[0-9][0-9_]*_|[0-9][0-9_]*_\.[0-9_]*|[0-9][0-9_]*\._[0-9_]*)|0{digit}) { std::cout << "Error: invalid numeric literal with underscore: " << yytext << std::endl; }

{number} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::cout << "Integer: " << atoi(remove_underscore(yytext).c_str()) << std::endl;
}

{doubleNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::cout << "Double: " << strtod(remove_underscore(yytext).c_str(), NULL) << std::endl;
}

{floatNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::cout << "Float: " << strtof(remove_underscore(yytext).c_str(), NULL) << std::endl;
}

{binNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::string binStr = remove_underscore(yytext + 2);
    std::cout << "Bin number: " << std::stoul(binStr, nullptr, 2) << std::endl;
}

{hexNumber} {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::string hexStr = remove_underscore(yytext + 2);
    std::cout << "Hex number: " << std::stoul(hexStr, nullptr, 16) << std::endl;
}

{id}  {
    processor.foundInappropriateLexem(yytext);
    is_prev_lexem_end_of_stmt = false;
    std::cout << "Identificator: " << yytext << std::endl;
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
    } else if (c == '|') {
        c_next = yyinput();
        if (c_next == '|') {
            unput(c_next);
            unput(c);
        }
    } else if (c == '!'){
        unput(c);
    } else if (c == '='){
        unput(c);
    } else if (c == '?') {
        c_next = yyinput();
        if (c_next == '.') {
            unput(c_next);
            unput(c);
        }
    } else {
        if ((open_bracket == 0) && (open_square_brackets == 0)) {
            if (!is_prev_lexem_end_of_stmt) {
                printEndOfLine();
            }
        }
        unput(c);
    }
}

[ \t]+                  { ; }
.                       { ; }

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
