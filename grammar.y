%token IF ELSE
%token FOR WHILE DO
%token VAL VAR
%token FUN RETURN
%token CLASS THIS OPEN OVERRIDE SUPER CONSTRUCTOR
%token PUBLIC PROTECTED PRIVATE
%token ENDL
%token ID

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
%left POST_INCREMENT POST_DECREMENT '.'
%nonassoc '(' ')' '[' ']'

%start kotlin_file

%%

endl_list: ENDL
	     | endl_list ENDL
	     ;

endl_list_e: /* empty */
	       | endl_list
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
    | '(' endl_list_e expr endl_list_e ')'
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
	      | '{' stmt_list '}'
	      ;

single_or_block_stmt: stmt
                    | stmt_block

condition_expr: endl_list_e '(' expr ')' endl_list_e
              ;

if_stmt: IF condition_expr single_or_block_stmt
       | IF condition_expr expr
       | IF condition_expr single_or_block_stmt ELSE single_or_block_stmt
       | IF condition_expr ELSE expr
       ;

while_stmt: WHILE condition_expr stmt
          ;

for_stmt: FOR '(' expr IN range_expr ')' stmt
        | FOR '(' expr IN expr ')' stmt
        ;

do_while_stmt: DO stmt WHILE condition_expr
             ;

range_expr: expr ".." expr step_expr
          | expr DOWN_TO expr step_expr
          | expr UNTIL expr step_expr
          ;

step_expr: /* empty */
         | STEP expr
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


argument_list: expr
             | argument_list ',' expr
             ;