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

\/\/[^\n]*                { std::cout << "Found singleline comment: " << (yytext + 2) << std::endl; }

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


if          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
else        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
when        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
for         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
in          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
while       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
do          { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
break       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
continue    { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
downTo      { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
step        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }

var         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
val         { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }

fun         { processor.processAppropriateElement(yytext);  }

class       { processor.processAppropriateElement(yytext);  }
this        { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
super       { processor.foundInappropriateLexem(yytext); printStatementKeyword(yytext); }
constructor { processor.processAppropriateElement(yytext);  }
enum        { processor.processAppropriateElement(yytext);  }

final       { processor.processAppropriateElement(yytext);  }
open        { processor.processAppropriateElement(yytext);  }
override    { processor.processAppropriateElement(yytext);  }
private     { processor.processAppropriateElement(yytext);  }
public      { processor.processAppropriateElement(yytext);  }
protected   { processor.processAppropriateElement(yytext);  }

arrayOf     { processor.foundInappropriateLexem(yytext); printKeyword(yytext); }

Array       { processor.foundInappropriateLexem(yytext); printType(yytext); }
Int         { processor.foundInappropriateLexem(yytext); printType(yytext); }
Double      { processor.foundInappropriateLexem(yytext); printType(yytext); }
Float       { processor.foundInappropriateLexem(yytext);printType(yytext); }
String      { processor.foundInappropriateLexem(yytext);printType(yytext); }
Char        { processor.foundInappropriateLexem(yytext); printType(yytext); }
Boolean     { processor.foundInappropriateLexem(yytext); printType(yytext); }

null        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); }
true        { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); }
false       { processor.foundInappropriateLexem(yytext); printConstantKeyword(yytext); }

"+"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"-"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"*"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"/"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }

"++"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"--"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"+="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"-="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"*="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"/="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }

"="         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"=="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"!="        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"<"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
">"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"&&"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"||"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }

"."         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"?."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"->"        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }

".."        { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"..<"       { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"("         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
")"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"{"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"}"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"["         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
"]"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }
";"         { processor.foundInappropriateLexem(yytext); printOperator(yytext); }

-?((_[0-9_]+|[0-9][0-9_]*_|[0-9][0-9_]*_\.[0-9_]*|[0-9][0-9_]*\._[0-9_]*)|0{digit}) { std::cout << "Error: invalid numeric literal with underscore: " << yytext << std::endl; }

{number} {
    processor.foundInappropriateLexem(yytext);
    std::cout << "Integer: " << atoi(remove_underscore(yytext).c_str()) << std::endl;
}

{doubleNumber} {
    processor.foundInappropriateLexem(yytext);
    std::cout << "Double: " << strtod(remove_underscore(yytext).c_str(), NULL) << std::endl;
}

{floatNumber} {
    processor.foundInappropriateLexem(yytext);
    std::cout << "Float: " << strtof(remove_underscore(yytext).c_str(), NULL) << std::endl;
}

{binNumber} {
    processor.foundInappropriateLexem(yytext);
    std::string binStr = remove_underscore(yytext + 2);
    std::cout << "Bin number: " << std::stoul(binStr, nullptr, 2) << std::endl;
}

{hexNumber} {
    processor.foundInappropriateLexem(yytext);
    std::string hexStr = remove_underscore(yytext + 2);
    std::cout << "Hex number: " << std::stoul(hexStr, nullptr, 16) << std::endl;
}

{id}  {
    processor.foundInappropriateLexem(yytext);
    std::cout << "Identificator: " << yytext << std::endl;
}

\n                      { printEndOfLine(); }

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
