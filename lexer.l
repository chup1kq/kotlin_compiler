%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <cstdlib>
    #include <algorithm>

    #define FOUND_CHARACTER(text) \
        do { std::cout << "Found character: " << text << std::endl; BEGIN(INITIAL); } while(0)


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
%x SYMBOL
%x MULTILINE_STRING
%x ML_COMMENT

digit          [0-9]
notZeroDigit   [1-9]
binDigit       [01]
hexDigit       [0-9a-fA-F]

number         -?(0|{notZeroDigit}(_*{digit})*)
binNumber      -?0[bB]({binDigit}(_*{binDigit})*)
hexNumber      -?0[xX]({hexDigit}(_*{hexDigit})*)
doubleNumber   -?{digit}(_*{digit})*(\.{digit}(_*{digit})*)?([eE][+-]{digit})?
floatNumber    -?{digit}(_*{digit})*(\.{digit}(_*{digit})*)?([eE][+-]{digit})?[fF]

%%

%{
    std::string buff = "";
%}

\" {
    buff = "";
    BEGIN(STRING);
}

<STRING>[^\"\\\n]+                   { buff += yytext; }
<STRING,SYMBOL>\\n                   { buff += "\n"; }
<STRING,SYMBOL>\\t                   { buff += "\t"; }
<STRING,SYMBOL>\\b                   { buff += "\b"; }
<STRING,SYMBOL>\\$                   { buff += "$"; }
<STRING,SYMBOL>\\\'                  { buff += "\'"; }
<STRING,SYMBOL>\\\"                  { buff += "\""; }
<STRING,SYMBOL>\\\\                  { buff += "\\"; }
<STRING,SYMBOL>\\u[0-9a-fA-F]{4}     {
    char ch = (char)strtol(yytext + strlen(yytext) - 4, NULL, 16);
    buff += ch;
}
<STRING,SYMBOL>\\. {
    std::cerr << "Error in literal. Unsupported escape sequence: " << yytext << std::endl;
    BEGIN(INITIAL);
}

<STRING>\" {
    std::cout << "Found singleline string: " << buff << std::endl;
    BEGIN(INITIAL);
}

<STRING,SYMBOL>\n {
    std::cerr << "Error in literal. Found newline: " << buff << std::endl;
    BEGIN(INITIAL);
}

<STRING,SYMBOL><<EOF>> {
    std::cerr << "Error in literal. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}

\' {
    buff = "";
    BEGIN(SYMBOL);
}

<SYMBOL>[^\'\\\n]+           { buff += yytext; }
<SYMBOL>\' {
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
    BEGIN(MULTILINE_STRING);
    buff.clear();
}
<MULTILINE_STRING>[^\n] { buff += yytext; }
<MULTILINE_STRING>\n    { buff += "\n"; }
<MULTILINE_STRING>\"{3} {
    std::cout << "Found multiline string: " << buff << std::endl;
    BEGIN(INITIAL);
}

<MULTILINE_STRING><<EOF>> {
    std::cerr << "Error in string. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}

\/\/[^\n]*                { std::cout << "Found singleline comment: " << (yytext + 2) << std::endl; }

\/\*                     { BEGIN(ML_COMMENT); buff.clear(); }
<ML_COMMENT>[^*]+|\n     { buff += yytext; }
<ML_COMMENT>\*+[^*/]     { buff += yytext; }
<ML_COMMENT>\*+\/ {
    std::cout << "Found multiline comment: " << buff << std::endl;
    BEGIN(INITIAL);
}
<ML_COMMENT><<EOF>> {
    std::cout << "Found multiline comment: " << buff << std::endl;
    BEGIN(INITIAL);
}

if          { printStatementKeyword(yytext); }
else        { printStatementKeyword(yytext); }
when        { printStatementKeyword(yytext); }
for         { printStatementKeyword(yytext); }
in          { printStatementKeyword(yytext); }
while       { printStatementKeyword(yytext); }
do          { printStatementKeyword(yytext); }
break       { printStatementKeyword(yytext); }
continue    { printStatementKeyword(yytext); }
downTo      { printStatementKeyword(yytext); }
step        { printStatementKeyword(yytext); }

var         { printStatementKeyword(yytext); }
val         { printStatementKeyword(yytext); }

fun         { printStatementKeyword(yytext); }

class       { printStatementKeyword(yytext); }
this        { printStatementKeyword(yytext); }
super       { printStatementKeyword(yytext); }
constructor { printStatementKeyword(yytext); }
enum        { printStatementKeyword(yytext); }

final       { printModifierKeyword(yytext); }
open        { printModifierKeyword(yytext); }
private     { printModifierKeyword(yytext); }
public      { printModifierKeyword(yytext); }
protected   { printModifierKeyword(yytext); }

arrayOf     { printKeyword(yytext); }

Array       { printType(yytext); }
Int         { printType(yytext); }
Double      { printType(yytext); }
Float       { printType(yytext); }
String      { printType(yytext); }
Char        { printType(yytext); }
Boolean     { printType(yytext); }

null        { printConstantKeyword(yytext); }
true        { printConstantKeyword(yytext); }
false       { printConstantKeyword(yytext); }

\+          { printOperator(yytext); }
-           { printOperator(yytext); }
\*          { printOperator(yytext); }
\\          { printOperator(yytext); }

\=          { printOperator(yytext); }
\=\=        { printOperator(yytext); }
\!\=        { printOperator(yytext); }
\<          { printOperator(yytext); }
\>          { printOperator(yytext); }

&&          { printOperator(yytext); }
\|\|        { printOperator(yytext); }

\.          { printOperator(yytext); }
\?\.        { printOperator(yytext); }
->          { printOperator(yytext); }

\.\.        { printOperator(yytext); }
\.\.<       { printOperator(yytext); }

\(          { printOperator(yytext); }
\)          { printOperator(yytext); }
\{          { printOperator(yytext); }
\}          { printOperator(yytext); }
\[          { printOperator(yytext); }
\]          { printOperator(yytext); }
\;          { printOperator(yytext); }

-?((_[0-9_]+|[0-9][0-9_]*_|[0-9][0-9_]*_\.[0-9_]*|[0-9][0-9_]*\._[0-9_]*)|0{digit}) { std::cout << "Error: invalid numeric literal with underscore: " << yytext << std::endl; }
{number}                                                                            { std::cout << "Integer: " << atoi(remove_underscore(yytext).c_str()) << std::endl; }
{doubleNumber}                                                                      { std::cout << "Double: " << strtod(remove_underscore(yytext).c_str(), NULL) << std::endl; }
{floatNumber}                                                                       { std::cout << "Float: " << strtof(remove_underscore(yytext).c_str(), NULL) << std::endl; }
{binNumber} { 
    std::string binStr = remove_underscore(yytext);
    if (binStr.substr(0, 2) == "0b" || binStr.substr(0, 2) == "0B") {
        binStr = binStr.substr(2);
    }
    std::cout << "Bin number: " << std::stoul(binStr, nullptr, 2) << std::endl; 
}

{hexNumber} { 
    std::string hexStr = remove_underscore(yytext);
    if (hexStr.substr(0, 2) == "0x" || hexStr.substr(0, 2) == "0X") {
        hexStr = hexStr.substr(2);
    }
    std::cout << "Hex number: " << std::stoul(hexStr, nullptr, 16) << std::endl; 
}
[a-zA-Z_][a-zA-Z0-9_]*  { std::cout << "Identificator: " << yytext << std::endl; }

\n                      { printEndOfLine(); }

[ \t]+                  { ; }
.                       { ; }

;;                      { std::cout << "Syntax error: " << yytext << std::endl; }

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