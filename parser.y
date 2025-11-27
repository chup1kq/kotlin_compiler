%code requires {
    #include "../src/nodes/Node.h"
    #include "../src/nodes/types.h"
    #include "../src/nodes/expr/ExprListNode.h"
    #include "../src/nodes/expr/ExprNode.h"
    #include "../src/nodes/stmt/StmtListNode.h"
    #include "../src/nodes/stmt/StmtNode.h"
    #include "../src/nodes/type/TypeNode.h"
    #include "../src/nodes/varDeclaration/VarDeclaration.h"
    #include "../src/nodes/varDeclaration/VarDeclarationList.h"
}

%code {
    #include <iostream>
    using namespace std;

    int yylex();
    void yyerror(const char* s);
}


%union {
    int intLiteral;
    char charLiteral;
    float floatLiteral;
    double doubleLiteral;
    char* stringLiteral;
    bool boolLiteral;
    char* identifier;

    ExprNode* expression;
    ExprListNode* exprList;
    StmtNode* statement;
    StmtListNode* stmtList;
    TypeNode* typeNode;
    VarDeclaration* varDecl;
    VarDeclarationList* varDeclList;
}

%token NON

%token IF ELSE
%token FOR WHILE DO
%token VAL VAR
%token RETURN BREAK CONTINUE
%token THIS SUPER
%token ARRAY ARRAY_OF
%token ENDL
%token <identifier>ID
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

%token <intLiteral>INT_LITERAL
%token <floatLiteral>FLOAT_LITERAL
%token <doubleLiteral>DOUBLE_LITERAL
%token <charLiteral>CHAR_LITERAL
%token <stringLiteral>STRING_LITERAL
%token <boolLiteral>TRUE_LITERAL
%token <boolLiteral>FALSE_LITERAL
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

%type <expression> expr if_expr condition_expr
%type <exprList> expr_list

%type <statement> stmt var_body val_body while_stmt do_while_stmt return_body for_stmt

%type <stmtList> stmt_list stmt_block

%type <typeNode> type nullable_type

%type <varDecl> var_declaration var_declaration_default_value
%type <varDeclList> var_declaration_list

%type <varDeclList> class_declaration_argument_list declaration_argument_list argument_list
%type <varDecl> declaration_argument class_declaration_argument

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

expr: INT_LITERAL { $$ = ExprNode::createIntNode($1); }
    | FLOAT_LITERAL { $$ = ExprNode::createFloatNode($1); }
    | DOUBLE_LITERAL { $$ = ExprNode::createDoubleNode($1); }
    | CHAR_LITERAL { $$ = ExprNode::createCharNode($1); }
    | STRING_LITERAL { $$ = ExprNode::createStringNode($1); }
    | TRUE_LITERAL { $$ = ExprNode::createBoolNode($1); }
    | FALSE_LITERAL { $$ = ExprNode::createBoolNode($1); }
    | NULL_LITERAL { $$ = ExprNode::createNullNode(); }
    | ID { $$ = ExprNode::createIDNode($1); }
    | if_expr { $$ = $1; }
    | THIS { $$ = ExprNode::createThisExprNode(); }
    | SUPER { $$ = ExprNode::createSuperExprNode(); }
    | expr '.' ele ID { $$ = ExprNode::createFieldAccessExprNode($4, $1); }
    | expr '.' ele ID '(' ')' { $$ = ExprNode::createMethodAccessExprNode($4, NULL, $1); }
    | expr '.' ele ID '(' expr_list ')' { $$ = ExprNode::createMethodAccessExprNode($4, $6, $1); }
    | expr SAFE_CALL ele ID
    | expr SAFE_CALL ele ID '(' ')'
    | expr SAFE_CALL ele ID '(' expr_list ')'
    | expr OR ele expr { $$ = ExprNode::createExprNode(_DISJUNCTION, $1, $4); }
    | expr AND ele expr { $$ = ExprNode::createExprNode(_CONJUNCTION, $1, $4); }
    | expr RANGE ele expr { $$ = ExprNode::createRangeExprNode($1, $4); }
    | expr DOWN_TO ele expr { $$ = ExprNode::createRangeExprNode($1, $4); }
    | expr UNTIL ele expr { $$ = ExprNode::createExprNode(_UNTIL, $1, $4); }
    | expr STEP ele expr { $$ = ExprNode::createExprNode(_STEP, $1, $4); }
    | '!' expr { $$ = ExprNode::createNotExprNode($2); }
    | '-' ele expr %prec UMINUS { $$ = ExprNode::createUnaryExprNode(_UNARY_MINUS, $3); }
    | '+' ele expr %prec UPLUS { $$ = ExprNode::createUnaryExprNode(_UNARY_PLUS, $3); }
    | '(' expr ')' { $$ = ExprNode::createBracketExprNode($2); }
    | ID '(' ')' { $$ = ExprNode::createFunctionCallExprNode($1, NULL); }
    | ID '(' expr_list ')' { $$ = ExprNode::createFunctionCallExprNode($1, $3); }
    | expr '=' ele expr { $$ = ExprNode::createExprNode(_EQUAL, $1, $4); }
    | expr '+' ele expr { $$ = ExprNode::createExprNode(_PLUS, $1, $4); }
    | expr '-' ele expr { $$ = ExprNode::createExprNode(_MINUS, $1, $4); }
    | expr '*' ele expr { $$ = ExprNode::createExprNode(_MUL, $1, $4); }
    | expr '/' ele expr { $$ = ExprNode::createExprNode(_DIV, $1, $4); }
    | expr '%' ele expr { $$ = ExprNode::createExprNode(_MOD, $1, $4); }
    | expr '<' ele expr { $$ = ExprNode::createExprNode(_LESS, $1, $4); }
    | expr '>' ele expr { $$ = ExprNode::createExprNode(_GREAT, $1, $4); }
    | expr GREATER_EQUAL ele expr { $$ = ExprNode::createExprNode(_GREAT_EQUAL, $1, $4); }
    | expr LESS_EQUAL ele expr { $$ = ExprNode::createExprNode(_LESS_EQUAL, $1, $4); }
    | expr EQUAL ele expr { $$ = ExprNode::createExprNode(_EQUAL, $1, $4); }
    | expr NOT_EQUAL ele expr { $$ = ExprNode::createExprNode(_NOT_EQUAL, $1, $4); }
    | expr PLUS_ASSIGNMENT ele expr { $$ = ExprNode::createAssignmentExprNode(_PLUS_ASSIGNMENT, $1, $4); }
    | expr MINUS_ASSIGNMENT ele expr { $$ = ExprNode::createAssignmentExprNode(_MINUS_ASSIGNMENT, $1, $4); }
    | expr MUL_ASSIGNMENT ele expr { $$ = ExprNode::createAssignmentExprNode(_MUL_ASSIGNMENT, $1, $4); }
    | expr DIV_ASSIGNMENT ele expr { $$ = ExprNode::createAssignmentExprNode(_DIV_ASSIGNMENT, $1, $4); }
    | expr MOD_ASSIGNMENT ele expr { $$ = ExprNode::createAssignmentExprNode(_MOD_ASSIGNMENT, $1, $4); }
    | INCREMENT ele expr { $$ = ExprNode::createPrefExprNode(_PREF_INCREMENT, $3); }
    | DECREMENT ele expr { $$ = ExprNode::createPrefExprNode(_PREF_DECREMENT, $3); }
    | expr INCREMENT %prec POST_INCREMENT { $$ = ExprNode::createPostExprNode(_PREF_INCREMENT, $1); }
    | expr DECREMENT %prec POST_DECREMENT { $$ = ExprNode::createPostExprNode(_PREF_DECREMENT, $1); }
    | ARRAY_OF '(' ')' { $$ = ExprNode::createArrayExprNode(NULL, NULL); }
    | ARRAY_OF '(' expr_list ')' { $$ = ExprNode::createArrayExprNode(NULL, $3); }
    | ARRAY_OF '<' ele nullable_type ele '>' '(' ')' { $$ = ExprNode::createArrayExprNode($4, NULL); }
    | ARRAY_OF '<' ele type ele '>' '(' ')' { $$ = ExprNode::createArrayExprNode($4, NULL); }
    | ARRAY_OF '<' ele nullable_type ele '>' '(' expr_list ')' { $$ = ExprNode::createArrayExprNode($4, $8); }
    | ARRAY_OF '<' ele type ele '>' '(' expr_list ')' { $$ = ExprNode::createArrayExprNode($4, $8); }
    | expr '[' expr ']'
    ;

expr_list: expr { $$ = ExprListNode::addExprToList(nullptr, $1); }
	 | expr_list ele ',' ele expr { $$ = ExprListNode::addExprToList($1, $5); }
	 ;

stmt: ';' { $$ = StmtNode::createEmptyStmt(); }
    | expr end_of_stmt { $$ = StmtNode::createStmtFromExpr($1); }
    | var_body end_of_stmt { $$ = $1; }
    | val_body end_of_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | while_stmt  { $$ = $1; }
    | do_while_stmt { $$ = $1; }
    | return_body end_of_stmt { $$ = $1; }
    | BREAK end_of_stmt { $$ = StmtNode::createBreakNode(); }
    | CONTINUE end_of_stmt { $$ = StmtNode::createContinueNode(); }
    ;

stmt_list: stmt { $$ = StmtListNode::addStmtToList(nullptr, $1); }
	 | stmt_list stmt { $$ = StmtListNode::addStmtToList($1, $2); }
	 ;

stmt_block: '{' ele '}' { $$ = StmtListNode::addStmtToList(nullptr, nullptr); }
	  | '{' ele stmt_list '}' { $$ = $3; }
	  | '{' ele expr '}' { $$ = StmtListNode::addExprToStmtList(nullptr, $3); }
	  | '{' ele stmt_list expr '}' { $$ = StmtListNode::addExprToStmtList($3, $4); }
	  | '{' ele return_body '}' { $$ = StmtListNode::addStmtToList(nullptr, $3); }
	  | '{' ele stmt_list return_body '}' { $$ = StmtListNode::addStmtToList($3, $4); }
	  | '{' ele BREAK '}' { $$ = StmtListNode::addStmtToList(nullptr, StmtNode::createBreakNode()); }
	  | '{' ele stmt_list BREAK '}' { $$ = StmtListNode::addStmtToList($3, StmtNode::createBreakNode()); }
	  | '{' ele CONTINUE '}' { $$ = StmtListNode::addStmtToList(nullptr, StmtNode::createContinueNode()); }
	  | '{' ele stmt_list CONTINUE '}' { $$ = StmtListNode::addStmtToList($3, StmtNode::createContinueNode()); }
	  | '{' ele var_body '}' { $$ = StmtListNode::addStmtToList(nullptr, $3); }
	  | '{' ele stmt_list var_body '}' { $$ = StmtListNode::addStmtToList($3, $4); }
	  | '{' ele val_body '}' { $$ = StmtListNode::addStmtToList(nullptr, $3); }
	  | '{' ele stmt_list val_body '}' { $$ = StmtListNode::addStmtToList($3, $4); }
	  ;

type: INT_TYPE { $$ = TypeNode::createType(_INT, false); }
    | FLOAT_TYPE { $$ = TypeNode::createType(_FLOAT, false); }
    | DOUBLE_TYPE { $$ = TypeNode::createType(_DOUBLE, false); }
    | STRING_TYPE { $$ = TypeNode::createType(_STRING, false); }
    | CHAR_TYPE { $$ = TypeNode::createType(_CHAR, false); }
    | BOOLEAN_TYPE { $$ = TypeNode::createType(_BOOLEAN, false); }
    | ID { $$ = TypeNode::createType(_ID, false); }
    | ARRAY ele '<' ele nullable_type ele '>' { $$ = TypeNode::createArrayType(false, $5); }
    | ARRAY ele '<' ele type ele '>' { $$ = TypeNode::createArrayType(true, $5); }
    ;

nullable_type: type '?' { $$ = TypeNode::makeNullableType($1); }
             ;

var_body: var ele var_declaration { $$ = StmtNode::createVarOrValStmtNode(_VAR, $3); }
        | var ele var_declaration_default_value { $$ = StmtNode::createVarOrValStmtNode(_VAR, $3); }
        | var ele ID '=' ele expr { $$ = StmtNode::createVarOrValStmtNode(_VAR, VarDeclaration::createVarDeclaration($3, NULL, $6)); }
        ;

val_body: val ele var_declaration { $$ = StmtNode::createVarOrValStmtNode(_VAL, $3); }
        | val ele var_declaration_default_value { $$ = StmtNode::createVarOrValStmtNode(_VAL, $3); }
        | val ele ID '=' ele expr { $$ = StmtNode::createVarOrValStmtNode(_VAL, VarDeclaration::createVarDeclaration($3, NULL, $6)); }
        ;

var_declaration: ID ele ':' ele nullable_type { $$ = VarDeclaration::createVarDeclaration($1, $5, NULL); }
	       | ID ele ':' ele type { $$ = VarDeclaration::createVarDeclaration($1, $5, NULL); }
               ;

var_declaration_default_value: ID ele ':' ele nullable_type '=' ele expr { $$ = VarDeclaration::createVarDeclaration($1, $5, $8); }
			     | ID ele ':' ele type '=' ele expr { $$ = VarDeclaration::createVarDeclaration($1, $5, $8); }
			     ;

var_declaration_list: ID { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, VarDeclaration::createVarDeclaration($1, NULL, NULL)); }
		    | var_declaration { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, $1); }
		    | var_declaration_list ',' ID { $$ = VarDeclarationList::addVarDeclarationToList($1, VarDeclaration::createVarDeclaration($3, NULL, NULL)); }
		    | var_declaration_list ',' var_declaration { $$ = VarDeclarationList::addVarDeclarationToList($1, $3); }
		    ;

condition_expr: ele '(' expr ')' ele { $$ = $3; }
              ;

if_expr: IF condition_expr stmt_block ELSE stmt_block { $$ = ExprNode::createIfNode($2, $3, $5); }
       | IF condition_expr expr ELSE stmt_block { $$ = ExprNode::createIfNode($2, StmtListNode::addExprToStmtList(nullptr, $3), $5); }
       | IF condition_expr expr ELSE stmt { $$ = ExprNode::createIfNode($2, StmtListNode::addExprToStmtList(nullptr, $3), StmtListNode::addStmtToList(nullptr, $5)); }
       | IF condition_expr stmt_block { $$ = ExprNode::createIfNode($2, $3, nullptr); }
       | IF condition_expr stmt { $$ = ExprNode::createIfNode($2, StmtListNode::addStmtToList(nullptr, $3), nullptr); }
       ;

while_stmt: WHILE condition_expr stmt_block end_of_stmt { $$ = StmtNode::createCycleNodeFromBlockStmt(_WHILE, $2, $3); }
	  | WHILE condition_expr stmt { $$ = StmtNode::createCycleNodeFromSingleStmt(_WHILE, $2, $3); }
          ;

for_stmt: FOR ele '(' ID IN expr ')' ele stmt_block end_of_stmt { $$ = StmtNode::createForNodeFromBlockStmt(VarDeclaration::createVarDeclaration($4, NULL, NULL), $6, $9); }
        | FOR ele '(' ID IN expr ')' ele stmt { $$ = StmtNode::createForNodeFromSingleStmt(VarDeclaration::createVarDeclaration($4, NULL, NULL), $6, $9); }
	| FOR ele '(' var_declaration IN expr ')' ele stmt_block end_of_stmt { $$ = StmtNode::createForNodeFromBlockStmt( $4, $6, $9); }
        | FOR ele '(' var_declaration IN expr ')' ele stmt { $$ = StmtNode::createForNodeFromSingleStmt( $4, $6, $9); }
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt_block end_of_stmt { $$ = StmtNode::createForNodeFromBlockStmtWithSeveralId($5, $8, $11); }
	| FOR ele '(' '(' var_declaration_list ')' IN expr ')' ele stmt { $$ = StmtNode::createForNodeFromSingleStmtWithSeveralId($5, $8, $11); }
        ;

do_while_stmt: DO ele stmt_block ele WHILE ele '(' expr ')' end_of_stmt { $$ = StmtNode::createCycleNodeFromBlockStmt(_DO_WHILE, $8, $3); }
	     | DO ele stmt WHILE ele '(' expr ')' end_of_stmt { $$ = StmtNode::createCycleNodeFromSingleStmt(_DO_WHILE, $7, $3); }
             ;

return_body: RETURN { $$ = StmtNode::createReturnNode(NULL); }
	   | RETURN expr { $$ = StmtNode::createReturnNode($2); }
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

argument_list: expr
             | argument_list ',' expr
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

fun_declaration: fun ele ID ele '(' allowed_declaration_params ')' ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' type ele stmt_block
	       | fun ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr end_of_stmt
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' nullable_type ele '=' ele expr end_of_stmt
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' type ele '=' ele expr end_of_stmt
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

class_member: var_body end_of_stmt
            | val_body end_of_stmt
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
