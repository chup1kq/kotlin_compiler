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
    #include "../src/nodes/fun/FunNode.h"
    #include "../src/nodes/modifier/ModifierMap.h"
    #include "../src/nodes/class/ClassNode.h"
    #include "../src/nodes/class/Inheritance.h"
    #include "../src/nodes/class/members/ClassBodyNode.h"
    #include "../src/nodes/class/members/Constructor.h"
    #include "../src/nodes/class/members/enumEntry/EnumEntry.h"
    #include "../src/nodes/class/members/enumEntry/EnumEntryList.h"
    #include "../src/nodes/topLevel/KotlinFileNode.h"
    #include "../src/nodes/topLevel/KotlinElementList.h"
}

%code {
    #include <iostream>
    using namespace std;

    int yylex();
    void yyerror(KotlinFileNode** out_root, const char* s);
}

%parse-param { KotlinFileNode** out_root }

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
    FunNode* function;
    ModifierMap* modifiers;
    ClassNode* classDeclaration;
    Constructor* contruct;
    ClassBodyNode* classBody;
    Inheritance* inher;
    EnumEntry* enumEntry;
    EnumEntryList* enumEntryList;
    KotlinFileNode* kotlinFile;
    KotlinElementList* kotlinElementList;

}

%token NON

%token IF ELSE
%token FOR WHILE DO
%token <modifiers> VAL VAR
%token RETURN BREAK CONTINUE
%token THIS SUPER
%token ARRAY ARRAY_OF
%token ENDL
%token <identifier>ID
%token IN

%token <modifiers> PRIVATE_FINAL_CLASS PUBLIC_FINAL_CLASS
%token <modifiers> PRIVATE_OPEN_CLASS PUBLIC_OPEN_CLASS

%token <modifiers> PRIVATE_ENUM PUBLIC_ENUM

%token <modifiers> PRIVATE_CONSTRUCTOR PUBLIC_CONSTRUCTOR PROTECTED_CONSTRUCTOR

%token <modifiers> PRIVATE_FUN PUBLIC_FUN
%token <modifiers> PRIVATE_FINAL_FUN PUBLIC_FINAL_FUN PROTECTED_FINAL_FUN
%token <modifiers> PUBLIC_OPEN_FUN PROTECTED_OPEN_FUN
%token <modifiers> PUBLIC_FINAL_OVERRIDE_FUN PROTECTED_FINAL_OVERRIDE_FUN
%token <modifiers> PUBLIC_OPEN_OVERRIDE_FUN PROTECTED_OPEN_OVERRIDE_FUN
%token <modifiers> OPEN_OVERRIDE_FUN FINAL_OVERRIDE_FUN

%token <modifiers> PRIVATE_VAR PUBLIC_VAR
%token <modifiers> PRIVATE_FINAL_VAR PUBLIC_FINAL_VAR PROTECTED_FINAL_VAR
%token <modifiers> PUBLIC_OPEN_VAR PROTECTED_OPEN_VAR
%token <modifiers> PUBLIC_FINAL_OVERRIDE_VAR PROTECTED_FINAL_OVERRIDE_VAR
%token <modifiers> PUBLIC_OPEN_OVERRIDE_VAR PROTECTED_OPEN_OVERRIDE_VAR
%token <modifiers> OPEN_OVERRIDE_VAR FINAL_OVERRIDE_VAR

%token <modifiers> PRIVATE_VAL PUBLIC_VAL
%token <modifiers> PRIVATE_FINAL_VAL PUBLIC_FINAL_VAL PROTECTED_FINAL_VAL
%token <modifiers> PUBLIC_OPEN_VAL PROTECTED_OPEN_VAL
%token <modifiers> PUBLIC_FINAL_OVERRIDE_VAL PROTECTED_FINAL_OVERRIDE_VAL
%token <modifiers> PUBLIC_OPEN_OVERRIDE_VAL PROTECTED_OPEN_OVERRIDE_VAL
%token <modifiers> OPEN_OVERRIDE_VAL FINAL_OVERRIDE_VAL

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
%type <exprList> expr_list argument_list

%type <modifiers> var val fun class class_constructor enum enum_constructor

%type <statement> stmt var_body val_body while_stmt do_while_stmt return_body for_stmt

%type <stmtList> stmt_list stmt_block

%type <typeNode> type nullable_type

%type <varDecl> var_declaration var_declaration_default_value
%type <varDeclList> var_declaration_list

%type <varDeclList> class_declaration_argument_list declaration_argument_list allowed_declaration_params class_allowed_declaration_params
%type <varDecl> declaration_argument class_declaration_argument

%type <function> fun_declaration
%type <classDeclaration> class_declaration enum_declaration
%type <contruct> constructor_declaration
%type <classBody> class_body class_member_list enum_body
%type <inher> inheritance
%type <enumEntry> enum_entry
%type <enumEntryList> enum_entries

%type <kotlinElementList> top_level_declaration_list
%type <kotlinFile> kotlin_file

%start kotlin_file

%%

kotlin_file: top_level_declaration_list { *out_root = new KotlinFileNode($1); }

top_level_declaration_list: class_declaration { $$ = KotlinElementList::addElement(nullptr, $1); }
			  | fun_declaration { $$ = KotlinElementList::addElement(nullptr, $1); }
			  | enum_declaration { $$ = KotlinElementList::addElement(nullptr, $1); }
			  | ';' { $$ = new KotlinElementList(); }
			  | top_level_declaration_list class_declaration { $$ = KotlinElementList::addElement($1, $2); }
			  | top_level_declaration_list fun_declaration { $$ = KotlinElementList::addElement($1, $2); }
			  | top_level_declaration_list enum_declaration { $$ = KotlinElementList::addElement($1, $2); }
			  | top_level_declaration_list ';' { $$ = $1; }
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
    | expr SAFE_CALL ele ID { $$ = ExprNode::createSafeFieldAccessExprNode($4, $1); }
    | expr SAFE_CALL ele ID '(' ')' { $$ = ExprNode::createSafeMethodAccessExprNode($4, NULL, $1); }
    | expr SAFE_CALL ele ID '(' expr_list ')' { $$ = ExprNode::createSafeMethodAccessExprNode($4, $6, $1); }
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
    | expr '[' expr ']' { $$ = ExprNode::createArrayAccessNode($1, $3); }
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

enum_declaration: enum ele ID ele { $$ = ClassNode::createEnumNode($1, $3, nullptr, nullptr); }
		| enum ele ID ele enum_body ele { $$ = ClassNode::createEnumNode($1, $3, nullptr, $5); }
		| enum ele ID ele '(' class_allowed_declaration_params ')' ele { $$ = ClassNode::createEnumNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), nullptr); }
		| enum ele ID ele '(' class_allowed_declaration_params ')' enum_body ele { $$ = ClassNode::createEnumNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), $8); }
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')' ele { $$ = ClassNode::createEnumNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), nullptr); }
		| enum ele ID ele enum_constructor ele '(' class_allowed_declaration_params ')' enum_body ele { $$ = ClassNode::createEnumNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), $10); }
	        ;

enum_body: '{' ele '}' { $$ = new ClassBodyNode(); }
         | '{' enum_entries '}' { $$ = ClassBodyNode::addEnumEntries(nullptr, $2); }
         | '{' enum_entries ',' ele '}' { $$ = ClassBodyNode::addEnumEntries(nullptr, $2); }
         | '{' enum_entries ';' ele '}' { $$ = ClassBodyNode::addEnumEntries(nullptr, $2); }
         | '{' enum_entries ';' ele class_member_list ele '}' { $$ = ClassBodyNode::addEnumEntries($5, $2); }
         ;

enum_entry: ele ID ele { $$ = EnumEntry::createEnumEntry($2, nullptr); }
          | ele ID ele '(' argument_list ')' ele { $$ = EnumEntry::createEnumEntry($2, $5); }
          | ele ID ele '(' argument_list ',' ')' ele { $$ = EnumEntry::createEnumEntry($2, $5); }
          ;

enum_entries: enum_entry { $$ = new EnumEntryList($1); }
            | enum_entries ',' enum_entry { $$ = EnumEntryList::addEnumEntry($1, $3); }
            ;

argument_list: expr { $$ = ExprListNode::addExprToList(nullptr, $1); }
             | argument_list ',' expr { $$ = ExprListNode::addExprToList($1, $3); }
             ;

declaration_argument: var_declaration { $$ = $1; }
		    | var_declaration_default_value { $$ = $1; }
		    ;

declaration_argument_list: declaration_argument { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, $1); }
		         | declaration_argument_list ele ',' ele declaration_argument { $$ = VarDeclarationList::addVarDeclarationToList($1, $5); }
		         ;

allowed_declaration_params: ele { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, nullptr);  }
			  | declaration_argument_list { $$ = $1; }
		          | declaration_argument_list ele ',' { $$ = $1; }
		          ;

class_declaration_argument: var ele declaration_argument { $$ = $3; }
			  | val ele declaration_argument { $$ = $3; }
			  | declaration_argument { $$ = $1; }
			  ;

class_declaration_argument_list: class_declaration_argument { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, $1); }
		               | class_declaration_argument_list ele ',' ele class_declaration_argument { $$ = VarDeclarationList::addVarDeclarationToList($1, $5); }
		               ;

class_allowed_declaration_params: ele { $$ = VarDeclarationList::addVarDeclarationToList(nullptr, nullptr); }
			        | ele class_declaration_argument_list ele { $$ = $2; }
			        | ele class_declaration_argument_list ele ',' ele { $$ = $2; }
			        ;

fun_declaration: fun ele ID ele '(' allowed_declaration_params ')' ele stmt_block { $$ = FunNode::createFunNode(TypeNode::createType(_VOID, false), $1, $3, $6, $9); }
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' ele nullable_type ele stmt_block { $$ = FunNode::createFunNode($11, $1, $3, $6, $13); }
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' ele type ele stmt_block { $$ = FunNode::createFunNode($11, $1, $3, $6, $13); }
	       | fun ele ID ele '(' allowed_declaration_params ')' ele '=' ele expr end_of_stmt { $$ = FunNode::createFunNodeFromExpr(TypeNode::createType(_VOID, false), $1, $3, $6, $11); }
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' ele nullable_type ele '=' ele expr end_of_stmt { $$ = FunNode::createFunNodeFromExpr($11, $1, $3, $6, $15); }
	       | fun ele ID ele '(' allowed_declaration_params ')' ele ':' ele type ele '=' ele expr end_of_stmt { $$ = FunNode::createFunNodeFromExpr($11, $1, $3, $6, $15); }
	       ;

class_declaration: class ele ID ele { $$ = ClassNode::createClassNode($1, $3, nullptr, nullptr, nullptr); }
		 | class ele ID ele class_body ele { $$ = ClassNode::createClassNode($1, $3, nullptr, nullptr, $5); }
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), nullptr, nullptr); }
		 | class ele ID ele '(' class_allowed_declaration_params ')' ele class_body ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), nullptr, $9); }
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), nullptr, nullptr); }
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' ele class_body ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), nullptr, $11); }
		 | class ele ID inheritance { $$ = ClassNode::createClassNode($1, $3, nullptr, $4, nullptr); }
		 | class ele ID inheritance class_body ele { $$ = ClassNode::createClassNode($1, $3, nullptr, $4, $5); }
		 | class ele ID ele '(' class_allowed_declaration_params ')' inheritance { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), $8, nullptr); }
		 | class ele ID ele '(' class_allowed_declaration_params ')' inheritance class_body ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor(ModifierMap::createClassConstructorModifiers(NONE), $6, nullptr), $8, $9); }
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' inheritance { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), $10, nullptr); }
		 | class ele ID ele class_constructor ele '(' class_allowed_declaration_params ')' inheritance class_body ele { $$ = ClassNode::createClassNode($1, $3, Constructor::createPrimaryConstructor($5, $8, nullptr), $10, $11); }
		 ;

inheritance: ':' ele ID ele { $$ = Inheritance::createInheritance($3, nullptr); }
	   | ':' ele ID ele '(' ele ')' ele { $$ = Inheritance::createInheritance($3, nullptr); }
	   | ':' ele ID ele '(' ele expr_list ele ')' ele { $$ = Inheritance::createInheritance($3, $7); }
	   | ':' ele ID ele '(' ele expr_list ele ',' ele ')' ele { $$ = Inheritance::createInheritance($3, $7); }
	   ;

class_body: '{' ele '}' { $$ = new ClassBodyNode(); }
          | '{' ele class_member_list ele '}' { $$ = $3; }
          ;

class_member_list: var_body end_of_stmt { $$ = ClassBodyNode::addMember(nullptr, $1); }
                 | val_body end_of_stmt { $$ = ClassBodyNode::addMember(nullptr, $1); }
                 | fun_declaration { $$ = ClassBodyNode::addMember(nullptr, $1); }
                 | constructor_declaration { $$ = ClassBodyNode::addMember(nullptr, $1); }
                 | class_member_list ele var_body end_of_stmt { $$ = ClassBodyNode::addMember($1, $3); }
                 | class_member_list ele val_body end_of_stmt { $$ = ClassBodyNode::addMember($1, $3); }
                 | class_member_list ele fun_declaration { $$ = ClassBodyNode::addMember($1, $3); }
                 | class_member_list ele constructor_declaration { $$ = ClassBodyNode::addMember($1, $3); }
                 ;

constructor_declaration: class_constructor ele '(' allowed_declaration_params ')' ele stmt_block { $$ = Constructor::createPrimaryConstructor($1, $4, $7); }
		       | class_constructor ele '(' allowed_declaration_params ')' ':' ele THIS ele '(' allowed_declaration_params ')' ele stmt_block { $$ = Constructor::createSecondaryConstructor($1, $4, $14, _THIS, $11); }
		       | class_constructor ele '(' allowed_declaration_params ')' ':' ele SUPER ele '(' allowed_declaration_params ')' ele stmt_block { $$ = Constructor::createSecondaryConstructor($1, $4, $14, _SUPER, $11); }
		       ;

var: VAR { $$ = ModifierMap::createFunOrVarModifiers(NONE, NONE, NONE); }
   | PRIVATE_VAR { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, NONE, NONE); }
   | PUBLIC_VAR { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, NONE, NONE); }
   | PRIVATE_FINAL_VAR { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, FINAL, NONE); }
   | PUBLIC_FINAL_VAR { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, NONE); }
   | PROTECTED_FINAL_VAR { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, NONE); }
   | PUBLIC_OPEN_VAR { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, NONE); }
   | PROTECTED_OPEN_VAR { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, NONE); }
   | PUBLIC_FINAL_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, OVERRIDE); }
   | PROTECTED_FINAL_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, OVERRIDE); }
   | PUBLIC_OPEN_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, OVERRIDE); }
   | PROTECTED_OPEN_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, OVERRIDE); }
   | OPEN_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(NONE, OPEN, OVERRIDE); }
   | FINAL_OVERRIDE_VAR { $$ = ModifierMap::createFunOrVarModifiers(NONE, FINAL, OVERRIDE); }
   ;

val: VAL { $$ = ModifierMap::createFunOrVarModifiers(NONE, NONE, NONE); }
   | PRIVATE_VAL { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, NONE, NONE); }
   | PUBLIC_VAL { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, NONE, NONE); }
   | PRIVATE_FINAL_VAL { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, FINAL, NONE); }
   | PUBLIC_FINAL_VAL { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, NONE); }
   | PROTECTED_FINAL_VAL { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, NONE); }
   | PUBLIC_OPEN_VAL { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, NONE); }
   | PROTECTED_OPEN_VAL { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, NONE); }
   | PUBLIC_FINAL_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, OVERRIDE); }
   | PROTECTED_FINAL_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, OVERRIDE); }
   | PUBLIC_OPEN_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, OVERRIDE); }
   | PROTECTED_OPEN_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, OVERRIDE); }
   | OPEN_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(NONE, OPEN, OVERRIDE); }
   | FINAL_OVERRIDE_VAL { $$ = ModifierMap::createFunOrVarModifiers(NONE, FINAL, OVERRIDE); }
   ;


fun: PRIVATE_FUN { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, NONE, NONE); }
   | PUBLIC_FUN { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, NONE, NONE); }
   | PRIVATE_FINAL_FUN { $$ = ModifierMap::createFunOrVarModifiers(PRIVATE, FINAL, NONE); }
   | PUBLIC_FINAL_FUN { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, NONE); }
   | PROTECTED_FINAL_FUN { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, NONE); }
   | PUBLIC_OPEN_FUN { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, NONE); }
   | PROTECTED_OPEN_FUN { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, NONE); }
   | PUBLIC_FINAL_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, FINAL, OVERRIDE); }
   | PROTECTED_FINAL_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, FINAL, OVERRIDE); }
   | PUBLIC_OPEN_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(PUBLIC, OPEN, OVERRIDE); }
   | PROTECTED_OPEN_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(PROTECTED, OPEN, OVERRIDE); }
   | OPEN_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(NONE, OPEN, OVERRIDE); }
   | FINAL_OVERRIDE_FUN { $$ = ModifierMap::createFunOrVarModifiers(NONE, FINAL, OVERRIDE); }
   ;

class_constructor: PRIVATE_CONSTRUCTOR { $$ = ModifierMap::createClassConstructorModifiers(PRIVATE); }
		 | PUBLIC_CONSTRUCTOR { $$ = ModifierMap::createClassConstructorModifiers(PUBLIC); }
		 | PROTECTED_CONSTRUCTOR { $$ = ModifierMap::createClassConstructorModifiers(PROTECTED); }
		 ;

class: PRIVATE_FINAL_CLASS { $$ = ModifierMap::createClassModifiers(PRIVATE, FINAL); }
     | PUBLIC_FINAL_CLASS { $$ = ModifierMap::createClassModifiers(PUBLIC, FINAL); }
     | PRIVATE_OPEN_CLASS { $$ = ModifierMap::createClassModifiers(PRIVATE, OPEN); }
     | PUBLIC_OPEN_CLASS { $$ = ModifierMap::createClassModifiers(PUBLIC, OPEN); }
     ;

enum_constructor: PRIVATE_CONSTRUCTOR { $$ = ModifierMap::createEnumConstructorModifiers(PRIVATE); }
		;

enum: PRIVATE_ENUM { $$ = ModifierMap::createEnumModifiers(PRIVATE); }
    | PUBLIC_ENUM { $$ = ModifierMap::createEnumModifiers(PUBLIC); }
    ;

%%

void yyerror(KotlinFileNode** out_root, const char* s) {
    std::cerr << "Parser error: " << s << std::endl;
}
