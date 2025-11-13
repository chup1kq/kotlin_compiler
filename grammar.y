%{
#include <iostream>
using namespace std;

int yylex();
void yyerror(const char* s);
%}

%token NON

%token IF ELSE
%token FOR WHILE DO
%token VAL VAR
%token RETURN BREAK CONTINUE
%token THIS SUPER
%token ARRAY ARRAY_OF
%token ENDL
%token ID
%token DOWN_TO UNTIL STEP IN

%token PRIVATE_FINAL_CLASS PUBLIC_FINAL_CLASS
%token PRIVATE_OPEN_CLASS PUBLIC_OPEN_CLASS

%token PRIVATE_ENUM PUBLIC_ENUM

%token PRIVATE_CONSTRUCTOR PUBLIC_CONSTRUCTOR PROTECTED_CONSTRUCTOR

%token PRIVATE_FUN PUBLIC_FUN
%token PRIVATE_FINAL_FUN PUBLIC_FINAL_FUN PROTECTED_FINAL_FUN
%token PUBLIC_OPEN_FUN PROTECTED_OPEN_FUN
%token PUBLIC_FINAL_OVERRIDE_FUN PROTECTED_FINAL_OVERRIDE_FUN
%token PUBLIC_OPEN_OVERRIDE_FUN PROTECTED_OPEN_OVERRIDE_FUN
%token OPEN_OVERRIDE_FUN FINAL_OVERRIDE_FUN

%token INT_TYPE FLOAT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE BOOLEAN_TYPE

%token INT_LITERAL FLOAT_LITERAL DOUBLE_LITERAL
%token CHAR_LITERAL STRING_LITERAL
%token TRUE_LITERAL FALSE_LITERAL
%token NULL_LITERAL

%right '=' PLUS_ASSIGNMENT MINUS_ASSIGNMENT MUL_ASSIGNMENT DIV_ASSIGNMENT MOD_ASSIGNMENT
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left '<' '>' LESS_EQUAL GREATER_EQUAL
%left RANGE
%left '+' '-'
%left '*' '/' '%'
%nonassoc ':'
%right UMINUS UPLUS INCREMENT DECREMENT '!'
%left POST_INCREMENT POST_DECREMENT '.' SAFE_CALL '?' '[' ']' '(' ')'

%start kotlin_file

%%

kotlin_file: top_level_declaration_list

top_level_declaration_list: top_level_declaration
			  | top_level_declaration_list top_level_declaration
			  ;

top_level_declaration: end_of_stmt
		     | class_declaration
		     | fun_declaration
		     | enum_declaration
		     ;

endl_list: ENDL
         | ENDL endl_list
         ;

ele: /* empty */
   | endl_list
   ;

end_of_stmt: endl_list
           | ';' ele
           ;

expr: INT_LITERAL
    | FLOAT_LITERAL
    | DOUBLE_LITERAL
    | CHAR_LITERAL
    | STRING_LITERAL
    | TRUE_LITERAL
    | FALSE_LITERAL
    | NULL_LITERAL
    | ID
    | if_expr
    | THIS
    | SUPER
    | expr '.' ele ID
    | expr '.' ele ID '(' ele ')'
    | expr '.' ele ID '(' ele expr_list ele ')'
    | expr SAFE_CALL ele ID
    | expr SAFE_CALL ele ID '(' ele ')'
    | expr SAFE_CALL ele ID '(' ele expr_list ele ')'
    | expr OR ele expr
    | expr AND ele expr
    | '!' expr
    | '-' ele expr %prec UMINUS
    | '+' ele expr %prec UPLUS
    | '(' expr ')'
    | ID '(' ele ')'
    | ID '(' ele expr_list ele ')'
    | expr '=' ele expr
    | expr '+' ele expr
    | expr '-' ele expr
    | expr '*' ele expr
    | expr '/' ele expr
    | expr '%' ele expr
    | expr '<' ele expr
    | expr '>' ele expr
    | expr GREATER_EQUAL ele expr
    | expr LESS_EQUAL ele expr
    | expr EQUAL ele expr
    | expr NOT_EQUAL ele expr
    | expr PLUS_ASSIGNMENT ele expr
    | expr MINUS_ASSIGNMENT ele expr
    | expr MUL_ASSIGNMENT ele expr
    | expr DIV_ASSIGNMENT ele expr
    | expr MOD_ASSIGNMENT ele expr
    | INCREMENT ele expr
    | DECREMENT ele expr
    | expr INCREMENT %prec POST_INCREMENT
    | expr DECREMENT %prec POST_DECREMENT
    | ARRAY_OF '(' ele ')'
    | ARRAY_OF '(' ele expr_list ele ')'
    | ID '[' expr ']'
    ;

expr_list: expr
	 | expr_list ele ',' ele expr
	 ;

expr_ws: ele expr ele
       ;

stmt: ';'
    | expr end_of_stmt
    | var_stmt
    | val_stmt
    | for_stmt
    | while_stmt
    | do_while_stmt
    | return_stmt
    | BREAK end_of_stmt
    | CONTINUE end_of_stmt
    ;

stmt_list: stmt
	 | stmt_list stmt
	 ;

stmt_block: '{' ele '}'
	  | '{' ele stmt_list ele '}'
	  | stmt
	  ;

type: INT_TYPE
    | FLOAT_TYPE
    | DOUBLE_TYPE
    | STRING_TYPE
    | CHAR_TYPE
    | BOOLEAN_TYPE
    | ID
    | ARRAY ele '<' ele nullable_type ele '>'
    ;

nullable_type: type
             | type ele '?'
             ;

var_stmt: VAR ele var_declaration end_of_stmt
        | VAR ele var_declaration '=' ele expr end_of_stmt
        | VAR ele ID '=' ele expr end_of_stmt
        ;

val_stmt: VAL ele var_declaration end_of_stmt
        | VAL ele var_declaration '=' ele expr end_of_stmt
        | VAL ele ID '=' ele expr end_of_stmt
        ;

var_declaration: ID ele ':' ele nullable_type
               ;

var_declaration_default_value: var_declaration '=' ele expr
			     ;

condition_expr: ele '(' expr_ws ')' ele
              ;

if_expr: IF condition_expr stmt_block ELSE stmt_block
       | IF condition_expr expr ELSE stmt_block
       | IF condition_expr stmt_block
       ;

while_stmt: WHILE condition_expr stmt_block end_of_stmt
          ;

for_stmt: FOR ele '(' expr_ws IN range_expr ')' ele stmt_block end_of_stmt
        | FOR ele '(' expr_ws IN expr_ws ')' ele stmt_block end_of_stmt
        ;

do_while_stmt: DO ele stmt_block ele WHILE ele '(' expr_ws ')' end_of_stmt
             ;

range_expr: expr_ws RANGE expr_ws step_expr
          | expr_ws DOWN_TO expr_ws step_expr
          | expr_ws UNTIL expr_ws step_expr
          ;

step_expr: /* empty */
         | STEP expr_ws
         ;

return_stmt: RETURN end_of_stmt
	   | RETURN expr end_of_stmt
	   ;

enum: PRIVATE_ENUM
    | PUBLIC_ENUM
    ;

enum_constructor: PRIVATE_CONSTRUCTOR
		;

enum_declaration: enum ele ID
		| enum ele ID ele enum_body
		| enum ele ID ele '(' class_allowed_declaration_params ')'
		| enum ele ID ele '(' class_allowed_declaration_params ')' enum_body
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')'
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')' enum_body
	        ;

enum_body: '{' ele '}'
         | '{' enum_entries '}'
         | '{' enum_entries ',' ele '}'
         | '{' enum_entries ';' ele '}'
         | '{' enum_entries ';' ele fun_declaration_list ele '}'
         ;

enum_entry: ele ID ele
          | ele ID ele '(' argument_list ')' ele
          | ele ID ele '(' argument_list ',' ele ')' ele
          ;

enum_entries: enum_entry
            | enum_entries ',' enum_entry
            ;

argument_list: expr_ws
             | argument_list ',' expr_ws
             ;

declaration_argument: var_declaration
		    | var_declaration_default_value
		    ;

declaration_argument_list: declaration_argument
		         | declaration_argument_list ele ',' ele declaration_argument
		         ;

allowed_declaration_params: ele
		          | ele declaration_argument_list ele
		          | ele declaration_argument_list ele ',' ele
		          ;

class_declaration_argument: VAR ele declaration_argument
			  | VAL ele declaration_argument
			  ;

class_declaration_argument_list: class_declaration_argument
		               | class_declaration_argument_list ele ',' ele class_declaration_argument
		               ;

class_allowed_declaration_params: ele
			        | ele declaration_argument_list ele
			        | ele declaration_argument_list ele ',' ele
			        | ele class_declaration_argument_list ele
			        | ele class_declaration_argument_list ele ',' ele
			        ;

fun: PRIVATE_FUN
   | PUBLIC_FUN
   | PRIVATE_FINAL_FUN
   | PUBLIC_FINAL_FUN
   | PROTECTED_FINAL_FUN
   | PUBLIC_OPEN_FUN
   | PROTECTED_OPEN_FUN
   | PUBLIC_FINAL_OVERRIDE_FUN
   | PROTECTED_FINAL_OVERRIDE_FUN
   | PUBLIC_OPEN_OVERRIDE_FUN
   | PROTECTED_OPEN_OVERRIDE_FUN
   | OPEN_OVERRIDE_FUN
   | FINAL_OVERRIDE_FUN
   ;

fun_declaration: fun ele ID ele '(' allowed_declaration_params ')' ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele '=' ele expr
	       ;

fun_declaration_list: fun_declaration
		    | fun_declaration_list ele fun_declaration
		    ;

class_constructor: PRIVATE_CONSTRUCTOR
		 | PUBLIC_CONSTRUCTOR
		 | PROTECTED_CONSTRUCTOR
		 ;

class: PRIVATE_FINAL_CLASS
     | PUBLIC_FINAL_CLASS
     | PRIVATE_OPEN_CLASS
     | PUBLIC_OPEN_CLASS
     ;

class_declaration: class ele ID
		 | class ele ID ele class_body
		 | class ele ID ele '(' class_allowed_declaration_params ')'
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele class_body
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')'
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele class_body
		 | class ele ID ele inheritance
		 | class ele ID ele inheritance ele class_body
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele inheritance
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele inheritance ele class_body
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele inheritance
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele inheritance ele class_body
		 ;

inheritance: ID ele '(' ele ')'
	   | ID ele '(' ele expr_list ele ')'
	   | ID ele '(' ele expr_list ele ',' ele ')'
	   ;

class_body: '{' ele '}'
          | '{' ele class_member_list ele '}'
          ;

class_member_list: class_member
                 | class_member_list ele class_member
                 ;

class_member: var_stmt
            | val_stmt
            | fun_declaration
            | constructor_declaration
            ;

constructor_declaration: class_constructor '(' allowed_declaration_params ')' ele stmt_block
                       ;

%%

void yyerror(const char* s) {
    cerr << "Parser error: " << s << endl;
}

