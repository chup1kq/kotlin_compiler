%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <cstdlib>

    #define FOUND_TYPE() std::cout << "Found type: " << yytext << std::endl;
    #define FOUND_KEYWORD() std::cout << "Found keyword: " << yytext << std::endl;

    #define FOUND_CHARACTER(text) \
        do { std::cout << "Found character: " << text << std::endl; BEGIN(INITIAL); } while(0)

    std::string buff;
%}



%x STRING
%x SYMBOL
%x MULTILINE_STRING
%x ML_COMMENT

%%

%{
    void printOperator(char* o);
    void printEndOfLine();
    void printType(char* t);
    void printConstantKeyword(char* ckw);
%}

\" {
    BEGIN(STRING);
    buff.clear();
}

<STRING>\" {
    std::cout << "Found singleline string: " << buff << std::endl;
    BEGIN(INITIAL);
}

<STRING>[^\"\\\n]+            { buff += yytext; }
<STRING>\\n                   { buff += "\n"; }
<STRING>\\t                   { buff += "\t"; }
<STRING>\\b                   { buff += "\b"; }
<STRING>\\$                   { buff += "$"; }
<STRING>\\\'                  { buff += "\'"; }
<STRING>\\\"                  { buff += "\""; }
<STRING>\\\\                  { buff += "\\"; }
<STRING>\\u[0-9a-fA-F]{4}     {
    char ch = (char)strtol(yytext + strlen(yytext) - 4, NULL, 16);
    buff += ch;
}

<STRING>\n {
    std::cout << "Error in singleline string. Has no closed \" in string: " << buff << std::endl;
    BEGIN(INITIAL);
}

<STRING><<EOF>> {
    std::cout << "Error in string. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}

\' {
    BEGIN(SYMBOL);
    buff.clear();
}

<SYMBOL>\' {
    std::cout << "Error in character. Has no character." << std::endl;
    BEGIN(INITIAL);
}

<SYMBOL>[^\'\\\n]{1}\' {
    char c[2] = { yytext[0], '\0' };
    FOUND_CHARACTER(c);
}

<SYMBOL>\\n\'   { FOUND_CHARACTER("\n"); }
<SYMBOL>\\t\'   { FOUND_CHARACTER("\t"); }
<SYMBOL>\\b\'   { FOUND_CHARACTER("\b"); }
<SYMBOL>\\$\'   { FOUND_CHARACTER("$"); }
<SYMBOL>\\\'\'  { FOUND_CHARACTER("\'"); }
<SYMBOL>\\\"\'  { FOUND_CHARACTER("\""); }
<SYMBOL>\\\\\'  { FOUND_CHARACTER("\\"); }
<SYMBOL>\\u[0-9a-fA-F]{4}\' {
    char c[7] = { '\\', 'u', yytext[2], yytext[3], yytext[4], yytext[5], '\0' };
    FOUND_CHARACTER(c);
}

<SYMBOL>\n {
    std::cout << "Error in character. Has no closed ' in character: " << yytext << std::endl;
    BEGIN(INITIAL);
}

<SYMBOL><<EOF>> {
    std::cout << "Error in string. Unexpected end of file: " << buff << std::endl;
    BEGIN(INITIAL);
}
<SYMBOL>[^\'\n]{2,}\' {
    std::cout << "Error in character. has more than 1 character: " << yytext << std::endl;
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

if          { FOUND_KEYWORD(); }
else        { FOUND_KEYWORD(); }
when        { FOUND_KEYWORD(); }
for         { FOUND_KEYWORD(); }
in          { FOUND_KEYWORD(); }
while       { FOUND_KEYWORD(); }
do          { FOUND_KEYWORD(); }
break       { FOUND_KEYWORD(); }
continue    { FOUND_KEYWORD(); }
downTo      { FOUND_KEYWORD(); }
step        { FOUND_KEYWORD(); }

var         { FOUND_KEYWORD(); }
val         { FOUND_KEYWORD(); }
fun         { FOUND_KEYWORD(); }
class       { FOUND_KEYWORD(); }
this        { FOUND_KEYWORD(); }
super       { FOUND_KEYWORD(); }
open        { FOUND_KEYWORD(); }
final       { FOUND_KEYWORD(); }
constructor { FOUND_KEYWORD(); }
private     { FOUND_KEYWORD(); }
public      { FOUND_KEYWORD(); }
protected   { FOUND_KEYWORD(); }
enum        { FOUND_KEYWORD(); }

arrayOf     { FOUND_KEYWORD(); }

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

-?0|([1-9][0-9]*)        { std::cout << "Integer: " << atoi(yytext) << std::endl; }
-?[0-9]+\.[0-9]+         { std::cout << "Double: " << strtod(yytext, NULL) << std::endl; }
-?[0-9]+(\.([0-9])+)*f   { std::cout << "Float: " << strtof(yytext, NULL) << std::endl; }
[a-zA-Z_][a-zA-Z0-9_]*   { std::cout << "Identificator: " << yytext << std::endl; }

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