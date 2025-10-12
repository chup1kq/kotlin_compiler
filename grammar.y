%token IF ELSE
%token FOR WHILE DO
%token VAL VAR
%token FUN RETURN
%token CLASS THIS OPEN OVERRIDE SUPER CONSTRUCTOR
%token PUBLIC PROTECTED PRIVATE
%token ENDL
%token ID

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

endl_list: ENDL
	     | endl_list ENDL
	     ;

endl_list_e: /* empty */
	       | endl_list
	       ;

end_of_stmt: endl_list
           | ';' endl_list_e
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
    ;

expr_ws: endl_list_e expr endl_list_e
       ;

stmt: ';' endl_list_e
    | expr endl_list
    | expr ';' endl_list_e
    | var_stmt
    | val_stmt
    | multy_declaration_stmt
    | if_stmt
    | for_stmt
    | while_stmt
    | do_while_stmt
    | return_stmt
    ;

stmt_list: stmt
	 | stmt_list stmt
	 ;

stmt_block: '{' endl_list_e '}'
	  | '{' endl_list_e stmt_list endl_list_e '}'
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

nullable_e: endl_list_e '?'
          ;

var_stmt: VAR endl_list_e var_declaration end_of_stmt
        | VAR endl_list_e var_declaration endl_list_e '=' expr_ws end_of_stmt
        | VAR endl_list_e ID endl_list_e '=' expr_ws end_of_stmt
        ;

val_stmt: VAL endl_list_e var_declaration end_of_stmt
        | VAL endl_list_e var_declaration endl_list_e '=' expr_ws end_of_stmt
        | VAL endl_list_e ID endl_list_e '=' expr_ws end_of_stmt
        ;

var_declaration: ID enld_list_e ':' endl_list_e nullable_type
               ;

condition_expr: endl_list_e '(' expr_ws ')' endl_list_e
              ;

if_expr: IF condition_expr expr_ws ELSE expr_ws end_of_stmt
       ;

if_stmt: IF condition_expr single_or_block_stmt end_of_stmt
       | IF condition_expr single_or_block_stmt ELSE single_or_block_stmt end_of_stmt
       | IF condition_expr expr_ws end_of_stmt
       ;

while_stmt: WHILE condition_expr single_or_block_stmt end_of_stmt
          ;

for_stmt: FOR endl_list_e '(' expr_ws IN range_expr ')' endl_list_e single_or_block_stmt end_of_stmt
        | FOR endl_list_e '(' expr_ws IN expr_ws ')' endl_list_e single_or_block_stmt end_of_stmt
        ;

do_while_stmt: DO endl_list_e single_or_block_stmt endl_list_e WHILE condition_expr
             ;

range_expr: expr_ws ".." expr_ws step_expr
          | expr_ws DOWN_TO expr_ws step_expr
          | expr_ws UNTIL expr_ws step_expr
          ;

step_expr: /* empty */
         | STEP expr_ws
         ;

access_modifier: PUBLIC
               | PROTECTED
               | PRIVATE
               ;

enum_class: ENUM CLASS ID endl_list_e '{' enum_entries '}'
          ;

enum_entry: endl_list_e ID endl_list_e
          | endl_list_e ID '(' argument_list ')' endl_list_e
          ;

enum_entries: enum_entry
            | enum_entries ',' enum_entry
            ;

argument_list: expr_ws
             | argument_list ',' expr_ws
             ;