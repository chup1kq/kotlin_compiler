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
%token IN

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

%token PRIVATE_VAR PUBLIC_VAR
%token PRIVATE_FINAL_VAR PUBLIC_FINAL_VAR PROTECTED_FINAL_VAR
%token PUBLIC_OPEN_VAR PROTECTED_OPEN_VAR
%token PUBLIC_FINAL_OVERRIDE_VAR PROTECTED_FINAL_OVERRIDE_VAR
%token PUBLIC_OPEN_OVERRIDE_VAR PROTECTED_OPEN_OVERRIDE_VAR
%token OPEN_OVERRIDE_VAR FINAL_OVERRIDE_VAR

%token PRIVATE_VAL PUBLIC_VAL
%token PRIVATE_FINAL_VAL PUBLIC_FINAL_VAL PROTECTED_FINAL_VAL
%token PUBLIC_OPEN_VAL PROTECTED_OPEN_VAL
%token PUBLIC_FINAL_OVERRIDE_VAL PROTECTED_FINAL_OVERRIDE_VAL
%token PUBLIC_OPEN_OVERRIDE_VAL PROTECTED_OPEN_OVERRIDE_VAL
%token OPEN_OVERRIDE_VAL FINAL_OVERRIDE_VAL

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
%left RANGE DOWN_TO UNTIL STEP
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

top_level_declaration: class_declaration
		     | fun_declaration
		     | enum_declaration
		     | ';'
		     ;

ele: /* empty */
   | ENDL
   ;

end_of_stmt: ENDL
           | ';'
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
    | expr '.' ele ID '(' ')'
    | expr '.' ele ID '(' expr_list ')'
    | expr SAFE_CALL ele ID
    | expr SAFE_CALL ele ID '(' ')'
    | expr SAFE_CALL ele ID '(' expr_list ')'
    | expr OR ele expr
    | expr AND ele expr
    | expr RANGE ele expr
    | expr DOWN_TO ele expr
    | expr UNTIL ele expr
    | expr STEP ele expr
    | '!' expr
    | '-' ele expr %prec UMINUS
    | '+' ele expr %prec UPLUS
    | '(' expr ')'
    | ID '(' ')'
    | ID '(' expr_list ')'
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
    | ARRAY_OF '(' ')'
    | ARRAY_OF '(' expr_list ')'
    | ARRAY_OF '<' ele nullable_type ele '>' '(' ')'
    | ARRAY_OF '<' ele type ele '>' '(' ')'
    | ARRAY_OF '<' ele nullable_type ele '>' '(' expr_list ')'
    | ARRAY_OF '<' ele type ele '>' '(' expr_list ')'
    | expr '[' expr ']'
    ;

expr_list: expr
	 | expr_list ele ',' ele expr
	 ;

stmt: ';'
    | expr end_of_stmt
    | var_body end_of_stmt
    | val_body end_of_stmt
    | for_stmt
    | while_stmt
    | do_while_stmt
    | return_body end_of_stmt
    | BREAK end_of_stmt
    | CONTINUE end_of_stmt
    ;

stmt_list: stmt
	 | stmt_list stmt
	 ;

stmt_block: '{' ele '}'
	  | '{' ele stmt_list '}'
	  | '{' ele expr '}'
	  | '{' ele stmt_list expr '}'
	  | '{' ele return_body '}'
	  | '{' ele stmt_list return_body '}'
	  | '{' ele BREAK '}'
	  | '{' ele stmt_list BREAK '}'
	  | '{' ele CONTINUE '}'
	  | '{' ele stmt_list CONTINUE '}'
	  | '{' ele var_body '}'
	  | '{' ele stmt_list var_body '}'
	  | '{' ele val_body '}'
	  | '{' ele stmt_list val_body '}'
	  ;

type: INT_TYPE
    | FLOAT_TYPE
    | DOUBLE_TYPE
    | STRING_TYPE
    | CHAR_TYPE
    | BOOLEAN_TYPE
    | ID
    | ARRAY ele '<' ele nullable_type ele '>'
    | ARRAY ele '<' ele type ele '>'
    ;

nullable_type: type '?'
             ;

var_body: var ele var_declaration
        | var ele var_declaration_default_value
        | var ele ID '=' ele expr
        ;

val_body: val ele var_declaration
        | val ele var_declaration_default_value
        | val ele ID '=' ele expr
        ;

var_declaration: ID ele ':' ele nullable_type
	       | ID ele ':' ele type
               ;

var_declaration_default_value: ID ele ':' ele nullable_type '=' ele expr
			     | ID ele ':' ele type '=' ele expr
			     ;

var_declaration_list: ID
		    | var_declaration
		    | var_declaration_list ',' ID
		    | var_declaration_list ',' var_declaration
		    ;

condition_expr: ele '(' expr ')' ele
              ;

if_expr: IF condition_expr stmt_block ELSE stmt_block
       | IF condition_expr expr ELSE stmt_block
       | IF condition_expr expr ELSE stmt
       | IF condition_expr stmt_block
       | IF condition_expr stmt
       ;

while_stmt: WHILE condition_expr stmt_block end_of_stmt
	  | WHILE condition_expr stmt
          ;

for_stmt: FOR ele '(' ID IN expr ')' ele stmt_block end_of_stmt
        | FOR ele '(' ID IN expr ')' ele stmt
	| FOR ele '(' var_declaration IN expr ')' ele stmt_block end_of_stmt
	| FOR ele '(' var_declaration IN expr ')' ele stmt
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt_block end_of_stmt
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt
        ;

do_while_stmt: DO ele stmt_block ele WHILE ele '(' expr ')' end_of_stmt
	     | DO ele stmt WHILE ele '(' expr ')' end_of_stmt
             ;

return_body: RETURN
	   | RETURN expr
	   ;

enum: PRIVATE_ENUM
    | PUBLIC_ENUM
    ;

enum_constructor: PRIVATE_CONSTRUCTOR
		;

enum_declaration: enum ele ID ele
		| enum ele ID ele enum_body ele
		| enum ele ID ele '(' class_allowed_declaration_params ')' ele
		| enum ele ID ele '(' class_allowed_declaration_params ')' enum_body ele
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')' ele
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')' enum_body ele
	        ;

enum_body: '{' ele '}'
         | '{' enum_entries '}'
         | '{' enum_entries ',' ele '}'
         | '{' enum_entries ';' ele '}'
         | '{' enum_entries ';' ele class_member_list ele '}'
         ;

enum_entry: ele ID ele
          | ele ID ele '(' argument_list ')' ele
          | ele ID ele '(' argument_list ',' ')' ele
          ;

enum_entries: enum_entry
            | enum_entries ',' enum_entry
            ;

argument_list: expr
             | argument_list ',' expr
             ;

declaration_argument: var_declaration
		    | var_declaration_default_value
		    ;

declaration_argument_list: declaration_argument
		         | declaration_argument_list ele ',' ele declaration_argument
		         ;

allowed_declaration_params: ele
			  | declaration_argument_list
		          | declaration_argument_list ele ','
		          ;

class_declaration_argument: var ele declaration_argument
			  | val ele declaration_argument
			  | declaration_argument
			  ;

class_declaration_argument_list: class_declaration_argument
		               | class_declaration_argument_list ele ',' ele class_declaration_argument
		               ;

class_allowed_declaration_params: ele
			        | ele class_declaration_argument_list ele
			        | ele class_declaration_argument_list ele ',' ele
			        ;

fun_declaration: fun ele ID ele '(' allowed_declaration_params ')' ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' type ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr end_of_stmt
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele '=' ele expr end_of_stmt
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' type ele '=' ele expr end_of_stmt
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

class_declaration: class ele ID ele
		 | class ele ID ele class_body ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele class_body ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele class_body ele
		 | class ele ID inheritance
		 | class ele ID inheritance class_body ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' inheritance
		 | class ele ID ele '(' class_allowed_declaration_params ')' inheritance class_body ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' inheritance
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' inheritance class_body ele
		 ;

inheritance: ':' ele ID ele
	   | ':' ele ID ele '(' ele ')' ele
	   | ':' ele ID ele '(' ele expr_list ele ')' ele
	   | ':' ele ID ele '(' ele expr_list ele ',' ele ')' ele
	   ;

class_body: '{' ele '}'
          | '{' ele class_member_list ele '}'
          ;

class_member_list: var_body end_of_stmt
                 | val_body end_of_stmt
                 | fun_declaration
                 | constructor_declaration
                 | class_member_list ele var_body end_of_stmt
                 | class_member_list ele val_body end_of_stmt
                 | class_member_list ele fun_declaration
                 | class_member_list ele constructor_declaration
                 ;

constructor_declaration: class_constructor ele '(' allowed_declaration_params ')' ele stmt_block
		       | class_constructor ele '(' allowed_declaration_params ')' ':' ele THIS ele '(' allowed_declaration_params ')' ele stmt_block
		       | class_constructor ele '(' allowed_declaration_params ')' ':' ele SUPER ele '(' allowed_declaration_params ')' ele stmt_block
		       ;

var: VAR
   | PRIVATE_VAR
   | PUBLIC_VAR
   | PRIVATE_FINAL_VAR
   | PUBLIC_FINAL_VAR
   | PROTECTED_FINAL_VAR
   | PUBLIC_OPEN_VAR
   | PROTECTED_OPEN_VAR
   | PUBLIC_FINAL_OVERRIDE_VAR
   | PROTECTED_FINAL_OVERRIDE_VAR
   | PUBLIC_OPEN_OVERRIDE_VAR
   | PROTECTED_OPEN_OVERRIDE_VAR
   | OPEN_OVERRIDE_VAR
   | FINAL_OVERRIDE_VAR
   ;

val: VAL
   | PRIVATE_VAL
   | PUBLIC_VAL
   | PRIVATE_FINAL_VAL
   | PUBLIC_FINAL_VAL
   | PROTECTED_FINAL_VAL
   | PUBLIC_OPEN_VAL
   | PROTECTED_OPEN_VAL
   | PUBLIC_FINAL_OVERRIDE_VAL
   | PROTECTED_FINAL_OVERRIDE_VAL
   | PUBLIC_OPEN_OVERRIDE_VAL
   | PROTECTED_OPEN_OVERRIDE_VAL
   | OPEN_OVERRIDE_VAL
   | FINAL_OVERRIDE_VAL
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

%%

void yyerror(const char* s) {
    cerr << "Parser error: " << s << endl;
}

