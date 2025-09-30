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

stmt_list: '{' endl_list_e '}'
	 | '{' endl_list_e stmt endl_list_e '}'
	 ;