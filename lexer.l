%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FOUND(type) printf("Found %s: %s\n", type, yytext);
#define FOUND_OPERATOR() printf("Found operator: %s\n", yytext);
#define FOUND_TYPE() printf("Found type: %s\n", yytext);
#define FOUND_KEYWORD() printf("Found keyword: %s\n", yytext);

char buff[8192];

void FOUND_CHARACTER(const char* text) {
    printf("Found character: %s\n", text);
    BEGIN(INITIAL);
}
%}

%x STRING
%x SYMBOL
%x MULTILINE_STRING
%x ML_COMMENT

%%

\"              { 
			BEGIN(STRING); 
			buff[0] = '\0'; 
		}
<STRING>\"      { 
			printf("Found singleline string: %s\n", buff);
			BEGIN(INITIAL); 
		}
<STRING>[^\"\\\n]+   	{ strcat(buff, yytext); }
<STRING>\\n		{ strcat(buff, "\n"); }
<STRING>\\t		{ strcat(buff, "\t"); }
<STRING>\\b		{ strcat(buff, "\b"); }
<STRING>\\$		{ strcat(buff, "$"); }
<STRING>\\\'		{ strcat(buff, "\'"); }
<STRING>\\\"		{ strcat(buff, "\""); }
<STRING>\\\\		{ strcat(buff, "\\"); }
<STRING>\\u[0-9a-fA-F]{4} { strcat(buff, yytext); }
<STRING>\n|($)		{ 
				printf("Error in singleline string. Has no closed \" in string: %s\n", buff); 
				BEGIN(INITIAL);
			}
<STRING><<EOF>> 	{
    				printf("Error in string. Unexpected end of file: %s\n", buff);
    				BEGIN(INITIAL);
			}

\'		{ 
			BEGIN(SYMBOL); 
			buff[0] = '\0'; 
		}
<SYMBOL>\'      { 
			printf("Found character: %s\n", buff);
			BEGIN(INITIAL); 
		}
<SYMBOL>[^\'\\\n]{1}\'	{ 
				char c[2] = { yytext[0], '\0' };
    				FOUND_CHARACTER(c);
			}
<SYMBOL>\\n\'		{ FOUND_CHARACTER("\n"); }
<SYMBOL>\\t\'		{ FOUND_CHARACTER("\t"); }
<SYMBOL>\\b\'		{ FOUND_CHARACTER("\b"); }
<SYMBOL>\\$\'		{ FOUND_CHARACTER("$"); }
<SYMBOL>\\\'\'		{ FOUND_CHARACTER("\'"); }
<SYMBOL>\\\"\'		{ FOUND_CHARACTER("\""); }
<SYMBOL>\\\\\'		{ FOUND_CHARACTER("\\"); }
<SYMBOL>\\u[0-9a-fA-F]{4}\' {
                      char c[7] = { '\\', 'u', yytext[2], yytext[3], yytext[4],yytext[5], '\0' };
                      FOUND_CHARACTER(c);
                    }
<SYMBOL>\n		{ 
				printf("Error in character. Has no closed \' in character: %s\n", yytext); 
				BEGIN(INITIAL);
			}
<SYMBOL><<EOF>> 	{
    				printf("Error in string. Unexpected end of file: %s\n", buff);
    				BEGIN(INITIAL);
			}
<SYMBOL>[^\'\n]{2,}\'	{ 
				printf("Error in character. has more than 1 character: %s\n", yytext); 
				BEGIN(INITIAL);
			}

\"\"\"          { 
			BEGIN(MULTILINE_STRING); 
			buff[0] = '\0'; 
		}
<MULTILINE_STRING>.|\n     { strcat(buff, yytext); }
<MULTILINE_STRING>\"\"\"   { 
				printf("Found multiline string: %s\n", buff); 
				BEGIN(INITIAL); 
			   }

\/\/[^\n]*	{ printf("Found singleline comment: %s\n", yytext + 2); }

\/\*            { BEGIN(ML_COMMENT); buff[0] = '\0'; }
<ML_COMMENT>[^*]+|\n { strcat(buff, yytext); }
<ML_COMMENT>\*+[^*/] { strcat(buff, yytext); }
<ML_COMMENT>\*+\/    {
                        printf("Found multiline comment: %s\n", buff);
                        BEGIN(INITIAL);
                     }
<ML_COMMENT><<EOF>>  {
                         printf("Found multiline comment: %s\n", buff);
                         BEGIN(INITIAL);
                     }

if	  { FOUND_OPERATOR(); }
else      { FOUND_OPERATOR(); }
when      { FOUND_OPERATOR(); }
for       { FOUND_OPERATOR(); }
in        { FOUND_OPERATOR(); }
while     { FOUND_OPERATOR(); }
do        { FOUND_OPERATOR(); }
break     { FOUND_OPERATOR(); }
continue  { FOUND_OPERATOR(); }
downTo    { FOUND_OPERATOR(); }
step      { FOUND_OPERATOR(); }

var	  	{ FOUND_KEYWORD(); }
val	  	{ FOUND_KEYWORD(); }
fun	  	{ FOUND_KEYWORD(); }
class	  	{ FOUND_KEYWORD(); }
this	  	{ FOUND_KEYWORD(); }
super       { FOUND_KEYWORD(); }
open	  	{ FOUND_KEYWORD(); }
final	  	{ FOUND_KEYWORD(); }
constructor	{ FOUND_KEYWORD(); }
private	  	{ FOUND_KEYWORD(); }
public	  	{ FOUND_KEYWORD(); }
protected	{ FOUND_KEYWORD(); }
enum	  	{ FOUND_KEYWORD(); }

Array       { FOUND_KEYWORD(); }
arrayOf     { FOUND_KEYWORD(); }

null	  { FOUND_TYPE(); }
Int	  { FOUND_TYPE(); }
Double	  { FOUND_TYPE(); }
Float	  { FOUND_TYPE(); }
String	  { FOUND_TYPE(); }
Char	  { FOUND_TYPE(); }
Boolean	  { FOUND_TYPE(); }

true      { printf("Boolean: %s\n", yytext); }
false     { printf("Boolean: %s\n", yytext); }

\+        { FOUND_OPERATOR(); }
-         { FOUND_OPERATOR(); }
\*        { FOUND_OPERATOR(); }
\\        { FOUND_OPERATOR(); }

\=        { FOUND_OPERATOR(); }
\=\=      { FOUND_OPERATOR(); }
\!\=      { FOUND_OPERATOR(); }
\<        { FOUND_OPERATOR(); }
\>        { FOUND_OPERATOR(); }

&&        { FOUND_OPERATOR(); }
\|\|      { FOUND_OPERATOR(); }

\.        { FOUND_OPERATOR(); }
->        { FOUND_OPERATOR(); }

\.\.      { FOUND_OPERATOR(); }
\.\.<     { FOUND_OPERATOR(); }

\(        { FOUND_OPERATOR(); }
\)        { FOUND_OPERATOR(); }
\{        { FOUND_OPERATOR(); }
\}        { FOUND_OPERATOR(); }
\[        { FOUND_OPERATOR(); }
\]        { FOUND_OPERATOR(); }
\;	  { FOUND_OPERATOR(); }

[1-9][0-9]*		 { printf("Integer: %d\n", atoi(yytext)); }
[0-9]+\.[0-9]+		 { printf("Double: %d\n", strtod(yytext, NULL)); }
[0-9]+(\.([0-9])+)*f	 { printf("Float: %d\n", strtof(yytext, NULL)); }
[a-zA-Z_][a-zA-Z0-9_]*   { printf("Identificator: %s\n", yytext); }

\n         { printf("ENDL\n"); }

[ \t]+     { ; }
.          { ; }

%%

int main(void) {
    yylex();
    return 0;
}

int yywrap(void) {
    return 1;
}	