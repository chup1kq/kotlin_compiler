%{
#include <iostream>
using namespace std;

// Объявляем yylex() и yyerror(), чтобы избежать ошибок
int yylex();
void yyerror(const char* s);
%}

%token NON

%token IF ELSE
%token FOR WHILE DO
%token VAL VAR
%token FUN RETURN
%token CLASS THIS OPEN OVERRIDE SUPER CONSTRUCTOR ENUM
%token PUBLIC PROTECTED PRIVATE
%token ENDL
%token ID

%token PRIVATE_FINAL_CLASS PUBLIC_FINAL_CLASS INTERNAL_FINAL_CLASS
%token PRIVATE_OPEN_CLASS PUBLIC_OPEN_CLASS INTERNAL_OPEN_CLASS

%token PRIVATE_ENUM PUBLIC_ENUM

%token INT_TYPE FLOAT_TYPE DOUBLE_TYPE SRTING_TYPE CHAR_TYPE BOOLEAN_TYPE

%token INT_LITERAL FLOAT_LITERAL DOUBLE_LITERAL
%token CHAR_LITERAL STRING_LITERAL
%token TRUE_LITERAL FALSE_LITERAL
%token NULL_LITERAL

%nonassoc INCREMENT DECREMENT
%nonassoc ENDL
%nonassoc '{' ':'
%right '=' PLUS_ASSIGNMENT MINUS_ASSIGNMENT MUL_ASSIGNMENT DIV_ASSIGNMENT MOD_ASSIGNMENT
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left '>' '<' GREATER_EQUAL LESS_EQUAL
%left IN
%left RANGE DOWN_TO STEP UNTIL
%left '+' '-'
%left '*' '/' '%'
%left UMINUS UPLUS
%right PREF_INCREMENT PREF_DECREMENT '!'
%left POST_INCREMENT POST_DECREMENT '.' SAFE_CALL
%nonassoc '(' ')' '[' ']'

%start kotlin_file

%%

kotlin_file: top_level_declaration_list

top_level_declaration_list: top_level_declaration
			  | top_level_declaration_list ele top_level_declaration
			  ;

top_level_declaration: end_of_stmt
		     | class_declaration
		     | fun_declaration
		     | enum_declaration
		     ;

endl_list: ENDL
	 | endl_list ENDL
  	 ;

// endl_list_e
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
    | '(' expr_ws ')'
    | if_expr
    | THIS
    | SUPER
    | ID ele '(' ele expr_list_e ele ')'
    | expr ele '=' ele expr
    | expr ele '+' ele expr
    | expr ele '-' ele expr
    | expr ele '*' ele expr
    | expr ele '/' ele expr
    | expr ele '%' ele expr
    | expr ele '<' ele expr
    | expr ele '>' ele expr
    | expr ele GREATER_EQUAL ele expr
    | expr ele LESS_EQUAL ele expr
    | expr ele EQUAL ele expr
    | expr ele NOT_EQUAL ele expr
    | expr ele PLUS_ASSIGNMENT ele expr
    | expr ele MINUS_ASSIGNMENT ele expr
    | expr ele MUL_ASSIGNMENT ele expr
    | expr ele DIV_ASSIGNMENT ele expr
    | expr ele MOD_ASSIGNMENT ele expr
    | expr ele '.' ele ID
    | expr ele '.' ele ID '(' ele expr_list_e ele ')'
    ;

expr_list: expr
	 | expr_list ele ',' ele expr
	 ;

expr_list_e: /* empty */
	   | expr_list
	   ;

expr_ws: ele expr ele
       ;

stmt: ';' ele
    | expr endl_list
    | expr ';' ele
    | var_stmt
    | val_stmt
    | if_stmt
    | for_stmt
    | while_stmt
    | do_while_stmt
    | return_stmt
    ;

stmt_list: stmt
	 | stmt_list stmt
	 ;

stmt_block: '{' ele '}'
	  | '{' ele stmt_list ele '}'
	  ;

single_or_block_stmt: stmt
                    | stmt_block
                    ;

type: INT_TYPE
    | FLOAT_TYPE
    | DOUBLE_TYPE
    | SRTING_TYPE
    | CHAR_TYPE
    | BOOLEAN_TYPE
    ;

nullable_type: type nullable_e
             ;

nullable_e: /* empty */
	  | ele '?'
          ;

var_stmt: VAR ele var_declaration end_of_stmt
        | VAR ele var_declaration ele '=' ele expr end_of_stmt
        | VAR ele ID ele '=' ele expr end_of_stmt
        ;

val_stmt: VAL ele var_declaration end_of_stmt
        | VAL ele var_declaration ele '=' ele expr end_of_stmt
        | VAL ele ID ele '=' ele expr end_of_stmt
        ;

var_declaration: ID ele ':' ele nullable_type
               ;

var_declaration_default_value: var_declaration ele '=' ele expr
			     ;

condition_expr: ele '(' expr_ws ')' ele
              ;

if_expr: IF condition_expr single_or_block_stmt ELSE single_or_block_stmt end_of_stmt
       ;

if_stmt: IF condition_expr single_or_block_stmt end_of_stmt
       ;

while_stmt: WHILE condition_expr single_or_block_stmt end_of_stmt
          ;

for_stmt: FOR ele '(' expr_ws IN range_expr ')' ele single_or_block_stmt end_of_stmt
        | FOR ele '(' expr_ws IN expr_ws ')' ele single_or_block_stmt end_of_stmt
        ;

do_while_stmt: DO ele single_or_block_stmt ele WHILE condition_expr end_of_stmt
             ;

range_expr: expr_ws RANGE expr_ws step_expr
          | expr_ws DOWN_TO expr_ws step_expr
          | expr_ws UNTIL expr_ws step_expr
          ;

step_expr: /* empty */
         | STEP expr_ws
         ;

return_stmt: RETURN
	   | RETURN end_of_stmt
	   | RETURN expr end_of_stmt
	   ;



class_access_modifier: /* empty */
		     | PUBLIC
                     | PROTECTED
                     | PRIVATE
                     ;

enum_access_modifier: /* empty */
		    | PUBLIC
		    | PRIVATE
		    ;

enum_primary_constructor: /* empty */
			| CONSTRUCTOR
			| PRIVATE ele CONSTRUCTOR
			;

enum_declaration: enum_access_modifier ele ENUM ele CLASS ele ID ele enum_body
	        | enum_access_modifier ele ENUM ele CLASS ele ID ele enum_primary_constructor ele '(' allowed_declaration_params ')' enum_body
	        ;

enum_body: /* empty */
         | '{' ele '}'
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
		          | declaration_argument_list
		          | declaration_argument_list ',' ele
		          ;

fun_declaration: FUN ele ID ele '(' allowed_declaration_params ')' ele stmt_block ele
	       | FUN ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele stmt_block ele
	       | FUN ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr ele
	       | FUN ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele '=' ele expr ele
	       ;

fun_declaration_list: fun_declaration
		    | fun_declaration_list ele fun_declaration
		    ;

class_primary_constructor: /* empty */
                        | CONSTRUCTOR
                        | PRIVATE ele CONSTRUCTOR
                        | PUBLIC ele CONSTRUCTOR
                        | PROTECTED ele CONSTRUCTOR
                        ;

class_declaration: class_access_modifier ele CLASS ele ID ele class_body
                | class_access_modifier ele CLASS ele ID ele class_primary_constructor ele '(' allowed_declaration_params ')' class_body
                ;

class_body: /* empty */
        | '{' ele '}'
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

constructor_declaration: class_primary_constructor '(' allowed_declaration_params ')' ele stmt_block
                    ;

%%

void yyerror(const char* s) {
    cerr << "Parser error: " << s << endl;
}

