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

expr: INT_LITERAL { $$ = createIntNode($1); }
    | FLOAT_LITERAL { $$ = createFloatNode($1); }
    | DOUBLE_LITERAL { $$ = createDoubleNode($1); }
    | CHAR_LITERAL { $$ = createCharNode($1); }
    | STRING_LITERAL { $$ = createStringNode($1); }
    | TRUE_LITERAL { $$ = createBoolNode($1); }
    | FALSE_LITERAL { $$ = createBoolNode($1); }
    | NULL_LITERAL { $$ = createNullNode($1); }
    | ID { $$ = createIDNode($1); }
    | if_expr
    | THIS { $$ = createThisExprNode(); }
    | SUPER { $$ = createSuperExprNode(); }
    | expr '.' ele ID { $$ = createFieldAccessExprNode($4, $1); }
    | expr '.' ele ID '(' ')' { $$ = createMethodAccessExprNode($4, NULL, $1); }
    | expr '.' ele ID '(' expr_list ')' { $$ = createFuncCallExprNode($4, $6, $1); }
    | expr SAFE_CALL ele ID
    | expr SAFE_CALL ele ID '(' ')'
    | expr SAFE_CALL ele ID '(' expr_list ')'
    | expr OR ele expr { $$ = createExprNode(OR, $1, $4); }
    | expr AND ele expr { $$ = createExprNode(AND, $1, $4); }
    | expr RANGE ele expr { $$ = createRangeExprNode($1, $4); }
    | expr DOWN_TO ele expr { $$ = createRangeExprNode($1, $4); }
    | expr UNTIL ele expr { $$ = createExprNode(UNTIL, $1, $4); }
    | expr STEP ele expr { $$ = createExprNode(STEP, $1, $4); }
    | '!' expr { $$ = createNoteExprNode($2); }
    | '-' ele expr %prec UMINUS { $$ = createUnaryExprNode(UNARY_MINUS, $1); }
    | '+' ele expr %prec UPLUS { $$ = createUnaryExprNode(UNARY_PLUS, $1); }
    | '(' expr ')' { $$ = createBracketExprNode($2); }
    | ID '(' ')' { $$ = createFunctionCallExprNode($1, null); }
    | ID '(' expr_list ')' { $$ = createFunctionCallExprNode($1, $3); }
    | expr '=' ele expr { $$ = createExprNode(EQUAL, $1, $4); }
    | expr '+' ele expr { $$ = createExprNode(PLUS, $1, $4); }
    | expr '-' ele expr { $$ = createExprNode(MINUS, $1, $4); }
    | expr '*' ele expr { $$ = createExprNode(MUL, $1, $4); }
    | expr '/' ele expr { $$ = createExprNode(DIV, $1, $4); }
    | expr '%' ele expr { $$ = createExprNode(MOD, $1, $4); }
    | expr '<' ele expr { $$ = createExprNode(LESS, $1, $4); }
    | expr '>' ele expr { $$ = createExprNode(GREAT, $1, $4); }
    | expr GREATER_EQUAL ele expr { $$ = createExprNode(GREAT_EQUAL, $1, $4); }
    | expr LESS_EQUAL ele expr { $$ = createExprNode(LESS_EQUAL, $1, $4); }
    | expr EQUAL ele expr { $$ = createExprNode(EQUAL, $1, $4); }
    | expr NOT_EQUAL ele expr { $$ = createExprNode(NOT_EQUAL, $1, $4); }
    | expr PLUS_ASSIGNMENT ele expr { $$ = createAssignmentExprNode(PLUS_ASSIGNMENT, $1, $4); }
    | expr MINUS_ASSIGNMENT ele expr { $$ = createAssignmentExprNode(MINUS_ASSIGNMENT, $1, $4); }
    | expr MUL_ASSIGNMENT ele expr { $$ = createAssignmentExprNode(MUL_ASSIGNMENT, $1, $4); }
    | expr DIV_ASSIGNMENT ele expr { $$ = createAssignmentExprNode(DIV_ASSIGNMENT, $1, $4); }
    | expr MOD_ASSIGNMENT ele expr { $$ = createAssignmentExprNode(MOD_ASSIGNMENT, $1, $4); }
    | INCREMENT ele expr { $$ = createPrefExprNode(PREF_INCREMENT, $3); }
    | DECREMENT ele expr { $$ = createPrefExprNode(PREF_DECREMENT, $3); }
    | expr INCREMENT %prec POST_INCREMENT { $$ = createPostExprNode(PREF_INCREMENT, $3); }
    | expr DECREMENT %prec POST_DECREMENT { $$ = createPostExprNode(PREF_DECREMENT, $3); }
    | ARRAY_OF '(' ')' { $$ = createArrayExprNode(null, null); }
    | ARRAY_OF '(' expr_list ')' { $$ = createArrayExprNode(null, $3); }
    | ARRAY_OF '<' ele nullable_type ele '>' '(' ')' { $$ = createArrayExprNode($4, null); }
    | ARRAY_OF '<' ele type ele '>' '(' ')' { $$ = createArrayExprNode($4, null); }
    | ARRAY_OF '<' ele nullable_type ele '>' '(' expr_list ')' { $$ = createArrayExprNode($4, $8); }
    | ARRAY_OF '<' ele type ele '>' '(' expr_list ')' { $$ = createArrayExprNode($4, $8); }
    | ID '[' expr ']'
    ;

expr_list: expr
	 | expr_list ele ',' ele expr
	 ;

expr_ws: ele expr ele
       ;

stmt: ';' {$$ = createEmptyStmt();}
    | expr end_of_stmt {$$ = createStmtFromExpr($1);}
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
	  | '{' ele stmt_list '}'
	  | '{' ele expr '}'
	  | '{' ele stmt_list expr '}'
	  ;

type: INT_TYPE { $$ = createType(_INT, false); }
    | FLOAT_TYPE { $$ = createType(_FLOAT, false); }
    | DOUBLE_TYPE { $$ = createType(_FLOAT, false); }
    | STRING_TYPE { $$ = createType(_FLOAT, false); }
    | CHAR_TYPE { $$ = createType(_FLOAT, false); }
    | BOOLEAN_TYPE { $$ = createType(_FLOAT, false); }
    | ID { $$ = createType(_FLOAT, false); }
    | ARRAY ele '<' ele nullable_type ele '>' { $$ = createArrayType(false, $5); }
    ;

nullable_type: type ele '?' { $$ = makeNullableType($1) }
             ;

var_stmt: var ele var_declaration end_of_stmt
        | var ele var_declaration_default_value end_of_stmt
        | var ele ID '=' ele expr end_of_stmt
        ;

val_stmt: val ele var_declaration end_of_stmt
        | val ele var_declaration_default_value end_of_stmt
        | val ele ID '=' ele expr end_of_stmt
        ;

var_declaration: ID ele ':' ele nullable_type
               ;

var_declaration_default_value: ID ele ':' ele nullable_type '=' ele expr
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
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt_block end_of_stmt
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt
        ;

do_while_stmt: DO ele stmt_block ele WHILE ele '(' expr ')' end_of_stmt
	     | DO ele stmt WHILE ele '(' expr ')' end_of_stmt
             ;

return_stmt: RETURN end_of_stmt
	   | RETURN expr end_of_stmt
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

allowed_declaration_params: declaration_argument_list
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
	       | fun ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr end_of_stmt
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele '=' ele expr end_of_stmt
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

class_declaration: class ele ID ele
		 | class ele ID ele class_body ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele class_body ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele class_body ele
		 | class ele ID ele inheritance ele
		 | class ele ID ele inheritance ele class_body ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele inheritance ele
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele inheritance ele class_body ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele inheritance ele
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele inheritance ele class_body ele
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

%%

void yyerror(const char* s) {
    cerr << "Parser error: " << s << endl;
}
