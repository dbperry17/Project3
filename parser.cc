/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "parser.h"

using namespace std;

bool testing = false;
bool testParse = false;
bool testParseAll = false;
bool testStore = false;
bool testPrint = true;
bool testMatch = true;

void tester()
{
    if(testing)
    {
        cout << "\n\nSTARTING PROGRAM" << endl;
    }
    else
    {
        testParse = false;
        testParseAll = false;
        testStore = false;
        testPrint = false;
        testMatch = false;
    }
}

struct Parser::idListNode
{
    string id;
    idListNode *next;
};

struct Parser::Symbol
{
    string id;
    TokenType flag;
    int type;
    bool declared; //0 for implicit, 1 for explicit
    bool printed = 0;
};

//So that I don't have to remember what type number each one is
enum
{
    myBool = 0, myInt, myLong, myReal, myString
};

vector<Parser::Symbol> symTable;
int typeNum = 5;

/****
 *  NOTE: While dealing with testing for 1.4 errors
 *  I realized it would probably be more practical
 *  to make a function.
 *
 *  I decided against it because I was almost done
 *  pasting the code into the relevant functions,
 *  and I didn't want to redo stuff just for an
 *  asthetic difference
 */



/***********************
 * Teacher's functions *
 ***********************/

void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    if(testParse && (expected_type != END_OF_FILE))
        cout << "\nExpecting a Terminal" << endl;


    else if(testParse)
        cout << "End of file" << endl;

    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    else if(testParse && (expected_type != END_OF_FILE))
        cout << "Terminal found: " << t.lexeme << endl;
    return t;
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing
/*
 * program				→	decl	            body
 * decl					→	type_decl_section	var_decl_section
 * type_decl_section	→	TYPE            	type_decl_list
 * type_decl_section	→	ε
 * type_decl_list		→	type_decl	        type_decl_list
 * type_decl_list		→	type_decl
 * type_decl			→	id_list	COLON	    type_name       	SEMICOLON
 * type_name			→	REAL
 * type_name			→	INT
 * type_name			→	BOOLEAN
 * type_name			→	STRING
 * type_name			→	LONG
 * type_name			→	ID
 * var_decl_section		→	VAR             	var_decl_list
 * var_decl_section		→	ε
 * var_decl_list		→	var_decl	        var_decl_list
 * var_decl_list		→	var_decl
 * var_decl				→	id_list	COLON	    type_name	        SEMICOLON
 * id_list				→	ID	                COMMA	            id_list
 * id_list				→	ID
 * body					→	LBRACE	            stmt_list	        RBRACE
 * stmt_list			→	stmt	            stmt_list
 * stmt_list			→	stmt
 * stmt					→	assign_stmt
 * stmt					→	while_stmt
 * stmt					→	do_stmt
 * stmt					→	switch_stmt
 */

//program	→	decl	body
void Parser::parse_program()
{
    if(testParse)
        cout << "\nParsing: " << "program" << endl;

    loadDefaultSyms(); //my function
    // program -> decl body
    parse_decl();
    parse_body();
    if(testParse)
        cout << "Done Parsing: " << "program" << endl;
}

//decl	→	type_decl_section	var_decl_section
void Parser::parse_decl()
{
    if(testParseAll)
        cout << "\nParsing: " << "decl" << endl;

    // decl -> type_decl_section var_decl_section
    parse_type_decl_section();
    parse_var_decl_section();

    if(testParseAll)
        cout << "Done Parsing: " << "decl" << endl;
}

//type_decl_section	→	TYPE	type_decl_list
//type_decl_section	→	ε
void Parser::parse_type_decl_section()
{
    if(testParseAll)
        cout << "\nParsing: " << "type_decl_section" << endl;

    // type_decl_section -> TYPE type_decl_list
    // type_decl_section -> epsilon
    Token t = lexer.GetToken();
    if (t.token_type == TYPE)
    {
        // type_decl_section -> TYPE type_decl_list
        parse_type_decl_list();
    }
    else if (t.token_type == LBRACE || t.token_type == VAR)
    {
        // type_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error();
    }
    if(testParseAll)
        cout << "Done Parsing: " << "type_decl_section" << endl;
}

//type_decl_list	→	type_decl	type_decl_list
//type_decl_list	→	type_decl
void Parser::parse_type_decl_list()
{
    if(testParseAll)
        cout << "\nParsing: " << "type_decl_list" << endl;

    // type_decl_list -> type_decl
    // type_decl_list -> type_decl type_decl_list
    parse_type_decl();
    Token t = peek();
    if (t.token_type == LBRACE || t.token_type == VAR)
    {
        // type_decl_list -> type_decl
    }
    else if (t.token_type == ID)
    {
        // type_decl_list -> type_decl type_decl_list
        parse_type_decl_list();
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "type_decl_list" << endl;
}

/***************************************
 * Teacher's Functions that I adjusted *
 ***************************************/

//type_decl	→	id_list	COLON	type_name	SEMICOLON
//Checks for errors 1.1 and 1.2
void Parser::parse_type_decl()
{
    if(testStore)
        cout << "\nParsing: " << "type_decl" << endl;

    // type_decl -> id_list COLON type_name SEMICOLON
    idListNode *head = parse_id_list();

    //check to see if any items in list are already in symbol table
    idListNode *current = head;
    vector<string> idVec;
    Symbol tmpSym;
    tmpSym.type = -2; //to make changing easier;
    tmpSym.flag = TYPE;
    tmpSym.declared = 1;
    while(current != NULL)
    {
        Symbol checkSym = declCheck(current->id);

        if(checkSym.type != -1) //already in symbol table
        {
            //Checking whether it's error 1.1 or 1.2
            if(checkSym.declared)
            {
                //Explicit type redeclared explicitly (error code 1.1)
                //An explicitly declared type can be declared again explicitly by
                //appearing as part of an id_list in a type declaration.
                errorCode(1, 1, checkSym.id);
            }
            else
            {
                //Implicit type redeclared explicitly (error code 1.2)
                //An implicitly declared type can be declared again explicitly by
                //appearing as part of an id_list in a type declaration.
                errorCode(1, 2, checkSym.id);
            }
        }

        //check if any items in list are repeats
        for(int iter = 0; iter < (int)idVec.size(); iter++)
        {
            //Remember, string comparison returns 0 if strings are equal
            if(current->id.compare((idVec[iter])) == 0)
            {
                errorCode(1, 1, current->id);
            }
        }
        idVec.push_back(current->id);

        //putting variables in symbol table
        tmpSym.id = current->id;
        symTable.push_back(tmpSym);

        current = current->next;
    }

    expect(COLON);
    int type = parse_type_name(TYPE);

    for(int i = 5; i < (int)symTable.size(); i++)
    {
        if(symTable[i].type == -2)
            symTable[i].type = type;
    }

    expect(SEMICOLON);

    if(testStore)
        cout << "Done Parsing: " << "type_decl" << endl;
}

//type_name	→	REAL
//type_name	→	INT
//type_name	→	BOOLEAN
//type_name	→	STRING
//type_name	→	LONG
//type_name	→	ID
//Checks for error 2.2
int Parser::parse_type_name(TokenType flag)
{
    if(testParseAll)
        cout << "\nParsing: " << "type_name" << endl;

    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID
    Token tok = lexer.GetToken();


    //Listed in different order than in spec
    //so as to make outputting to spec easier
    if (tok.token_type == BOOLEAN)
    {
        // type_name -> BOOLEAN
        return myBool;
    }
    else if (tok.token_type == INT)
    {
        // type_name -> INT
        return myInt;
    }
    else if (tok.token_type == LONG)
    {
        // type_name -> LONG
        return myLong;
    }
    else if (tok.token_type == REAL)
    {
        // type_name -> REAL
        return myReal;
    }
    else if (tok.token_type == STRING)
    {
        // type_name -> STRING
        return myString;
    }
    else if (tok.token_type == ID)
    {
        // type_name -> ID
        Symbol checkSym = declCheck(tok.lexeme);
        //Implicit declaration
        if (checkSym.type == -1)
        {
            //all items in id_List are of a new,
            //implicitly declared type, so add that
            //new type to symbol table first
            checkSym.flag = TYPE;
            checkSym.type = typeNum;
            typeNum++;


            symTable.push_back(checkSym);
        }
        else if((checkSym.flag == VAR) && (flag == VAR))
        {
            //Variable used as a type (error code 2.2)
            //If an explicitly declared variable is used as type_name
            //in a variable declaration, the variable is used as a type.
            errorCode(2, 2, checkSym.id);
        }

        return checkSym.type;
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "type_name" << endl;

    return -1;
}


/*******************************
 * More of Teacher's functions *
 *******************************/

//var_decl_section	→	VAR	var_decl_list
//var_decl_section	→	ε
void Parser::parse_var_decl_section()
{
    if(testParseAll)
        cout << "\nParsing: " << "var_decl_section" << endl;

    // var_decl_section -> VAR var_decl_list
    // var_decl_section -> epsilon
    Token t = lexer.GetToken();
    if (t.token_type == VAR)
    {
        // var_decl_section -> VAR var_decl_list
        parse_var_decl_list();
    }
    else if (t.token_type == LBRACE)
    {
        // var_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "var_decl_section" << endl;

}

//var_decl_list	→	var_decl	var_decl_list
//var_decl_list	→	var_decl
void Parser::parse_var_decl_list()
{
    if(testParseAll)
        cout << "\nParsing: " << "var_decl_list" << endl;

    // var_decl_list -> var_decl
    // var_decl_list -> var_decl var_decl_list
    parse_var_decl();
    Token t = peek();
    if (t.token_type == LBRACE)
    {
        // var_decl_list -> var_decl
    }
    else if (t.token_type == ID)
    {
        // var_decl_list -> var_decl var_decl_list
        parse_var_decl_list();
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "var_decl_list" << endl;;
}

/********************************************
 * More Teacher's Functions that I adjusted *
 ********************************************/

//var_decl	→	id_list	COLON	type_name	SEMICOLON
//Checks for errors 1.3 and 2.1
void Parser::parse_var_decl()
{
    if(testStore)
        cout << "\nParsing: " << "var_decl" << endl;

    // var_decl -> id_list COLON type_name SEMICOLON
    idListNode *head = parse_id_list();

    //check to see if any items in list are already used
    idListNode *current = head;

    vector<string> idVec;
    Symbol tmpSym;
    tmpSym.type = -2; //to make changing easier;
    tmpSym.flag = VAR;
    tmpSym.declared = 1;
    while(current != NULL)
    {
        //check if variable is in symbol table
        Symbol checksym = declCheck(current->id);
        if(checksym.type != -1) //variable already in symbol table
        {
            if(checksym.flag == TYPE)
            {
                //Programmer-defined type redeclared as variable (error code 1.3)
                //If a previously declared type appears again in an id_list of a
                //variable declaration, the type is redeclared as a variable.
                errorCode(1, 3, checksym.id);
            }
            else if(checksym.flag == VAR)
            {
                //Variable declared more than once (error code 2.1)
                //An explicitly declared variable can be declared again
                //explicitly by appearing as part of an id_list in a variable
                //declaration.
                errorCode(2, 1, checksym.id);
            }
        }

        //check if any items in list are repeats
        for(int iter = 0; iter < (int)idVec.size(); iter++)
        {
            //Remember, string comparison returns 0 if strings are equal
            if(current->id.compare((idVec[iter])) == 0)
            {
                errorCode(2, 1, current->id);
            }
        }
        idVec.push_back(current->id);

        //putting variables in symbol table
        tmpSym.id = current->id;
        symTable.push_back(tmpSym);

        current = current->next;
    }

    expect(COLON);

    int type = parse_type_name(VAR);

    for(int i = 5; i < (int)symTable.size(); i++)
    {
        if(symTable[i].type == -2)
            symTable[i].type = type;
    }


    expect(SEMICOLON);

    if(testStore)
        cout << "Done Parsing: " << "var_decl" << endl;
}

//id_list	→	ID	COMMA	id_list
//id_list	→	ID
Parser::idListNode* Parser::parse_id_list()
{
    if(testParseAll)
        cout << "\nParsing: " << "id_list" << endl;

    // id_list -> ID
    // id_list -> ID COMMA id_list
    Token t1 = peek();
    idListNode *result = new idListNode;
    expect(ID);
    Token t2 = lexer.GetToken();
    if (t2.token_type == COMMA)
    {
        // id_list -> ID COMMA id_list
        //General case
        result->id = t1.lexeme;
        result->next = parse_id_list();
        return result;
    }
    else if (t2.token_type == COLON)
    {
        // id_list -> ID
        lexer.UngetToken(t2);
        result->id = t1.lexeme;
        result->next = NULL;
        return result;
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "id_list" << endl;

    return NULL; //I'm tired of getting warnings when I compile
}


/*******************************
 * More of Teacher's functions *
 *******************************/

//body	→	LBRACE	stmt_list	RBRACE
void Parser::parse_body()
{
    if(testParseAll)
        cout << "\nParsing: " << "body" << endl;

    // body -> LBRACE stmt_list RBRACE
    expect(LBRACE);
    parse_stmt_list();
    expect(RBRACE);

    if(testParse)
        cout << "Done Parsing: " << "body" << endl;
}

//stmt_list	→	stmt	stmt_list
//stmt_list	→	stmt
void Parser::parse_stmt_list()
{
    if(testParseAll)
        cout << "\nParsing: " << "stmt_list" << endl;

    // stmt_list -> stmt
    // stmt_list -> stmt stmt_list
    parse_stmt();
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID ||
        t.token_type == SWITCH || t.token_type == DO)
    {
        // stmt_list -> stmt stmt_list
        parse_stmt_list();
    }
    else if (t.token_type == RBRACE)
    {
        // stmt_list -> stmt
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "stmt_list" << endl;
}

//stmt	→	assign_stmt
//stmt	→	while_stmt
//stmt	→	do_stmt
//stmt	→	switch_stmt
void Parser::parse_stmt()
{
    if(testParseAll)
        cout << "\nParsing: " << "stmt" << endl;

    // stmt -> assign_stmt
    // stmt -> while_stmt
    // stmt -> do_stmt
    // stmt -> switch_stmt
    Token t = peek();
    if (t.token_type == ID)
    {
        // stmt -> assign_stmt
        parse_assign_stmt();
    }
    else if (t.token_type == WHILE)
    {
        // stmt -> while_stmt
        parse_while_stmt();
    }
    else if (t.token_type == DO)
    {
        // stmt -> do_stmt
        parse_do_stmt();
    }
    else if (t.token_type == SWITCH)
    {
        // stmt -> switch_stmt
        parse_switch_stmt();
    }
    else
    {
        syntax_error();
    }

    if(testParseAll)
        cout << "Done Parsing: " << "stmt" << endl;
}


/************************
 * Functions I finished *
 ************************/

/*
 * assign_stmt	→	ID	        EQUAL	    expr	SEMICOLON
 * while_stmt	→	WHILE   	condition	body
 * do_stmt		→	DO	        body	    WHILE	condition	SEMICOLON
 * switch_stmt	→	SWITCH  	ID	        LBRACE	case_list	RBRACE
 * case_list	→	case	    case_list
 * case_list	→	case
 * case			→	CASE    	NUM     	COLON	body
 * expr			→	term    	PLUS    	expr
 * expr			→	term
 * term			→	factor	    MULT	    term
 * term			→	factor  	DIV	        term
 * term			→	factor
 * factor		→	LPAREN	    expr	    RPAREN
 * factor		→	NUM
 * factor		→	REALNUM
 * factor		→	ID
 * condition	→	ID
 * condition	→	primary     relop	    primary
 * primary		→	ID
 * primary		→	NUM
 * primary		→	REALNUM
 * relop		→	GREATER
 * relop		→	GTEQ
 * relop		→	LESS
 * relop		→	NOTEQUAL
 * relop		→	LTEQ
 */

//assign_stmt -> ID EQUAL expr SEMICOLON
//checks for 1.4 error for LHS
//checks for C1 type mismatch
void Parser::parse_assign_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "assign_stmt" << endl;


    Token t = peek();
    Symbol checkSym = declCheck(t.lexeme);

    //Programmer-defined type used as variable (error code 1.4)
    //If a previously declared type appears in the body of the program, the
    //type is used as a variable.
    if (checkSym.flag == TYPE)
        errorCode(1, 4, checkSym.id);
    if(checkSym.type == -1)
    {
        checkSym.flag = VAR;
        checkSym.type = typeNum;
        typeNum++;
        symTable.push_back(checkSym);
    }


    expect(ID);
    expect(EQUAL);
    int y = parse_expr();
    int z = unify(checkSym.type, y);
    //C1: The left hand side of an assignment should have the same
    //type as the right hand side of that assignment
    if(z == -1)
        typeMismatch(t.line_no, "C1");

    expect(SEMICOLON);

    if(testParse)
        cout << "Done Parsing: " << "assign_stmt" << endl;
}

//while_stmt -> WHILE condition body
//checks for C4 type mismatch
void Parser::parse_while_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "while_stmt" << endl;

    expect(WHILE);

    Token t = peek(); //to get the line number
    int x = parse_condition();
    //C4: condition should be of type BOOLEAN
    if(x != myBool)
        typeMismatch(t.line_no, "C4");

    parse_body();

    if(testParse)
        cout << "Done Parsing: " << "while_stmt" << endl;
}

//do_stmt -> DO body WHILE condition SEMICOLON
//checks for C4 type mismatch
void Parser::parse_do_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "do_stmt" << endl;

    expect(DO);
    parse_body();
    expect(WHILE);

    Token t = peek(); //to get the line number
    int x = parse_condition();
    //C4: condition should be of type BOOLEAN
    if(x != myBool)
        typeMismatch(t.line_no, "C4");

    expect(SEMICOLON);

    if(testParse)
        cout << "Done Parsing: " << "do_stmt" << endl;
}

//switch_stmt -> SWITCH ID LBRACE case_list RBRACE
//checks for C5 type mismatch
void Parser::parse_switch_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "switch_stmt" << endl;

    //type mismatch check
    expect(SWITCH);

    Token t = lexer.GetToken();
    if(t.token_type == ID)
    {
        Symbol checkSym = declCheck(t.lexeme);
        if (checkSym.type == -1) //symbol has been implicitly declared as INT
        {
            checkSym.type = myInt;
            checkSym.flag = VAR;
            symTable.push_back(checkSym);
        }
        else if (checkSym.type > 4) //symbol is of unknown type that is equivalent to INT
            unify(checkSym.type, myInt);
        else if (checkSym.type != myInt) //Symbol is built-in type that is not an INT.
        {
            //C5: The variable that follows the SWITCH keyword in switch_stmt should be
            //of type INT
            typeMismatch(t.line_no, "C5");
        }
    }

    expect(LBRACE);
    parse_case_list();
    expect(RBRACE);

    if(testParse)
        cout << "Done Parsing: " << "switch_stmt" << endl;
}

//case_list -> case case_list
//case_list -> case
void Parser::parse_case_list()
{
    if(testParse)
        cout << "\nParsing: " << "case_list" << endl;

    parse_case();
    Token t = peek();
    if(t.token_type == CASE)
    {
        //case_list -> case case_list
        parse_case_list();
    }
    else if(t.token_type == RBRACE)
    {
        //case_list -> case
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "case_list" << endl;

}

//case -> CASE NUM COLON body
void Parser::parse_case()
{
    if(testParse)
        cout << "\nParsing: " << "case" << endl;

    expect(CASE);
    expect(NUM);
    expect(COLON);
    parse_body();

    if(testParse)
        cout << "Done Parsing: " << "case" << endl;
}

//expr -> term PLUS expr
//expr -> term
//checks for C2 type mismatch
int Parser::parse_expr()
{
    if (testParse)
        cout << "\nParsing: " << "expr" << endl;

    int x = parse_term();

    Token t = lexer.GetToken();
    if (t.token_type == PLUS)
    {
        //expr -> term PLUS expr
        int y = parse_expr();
        int z = unify(x, y);
        //C2: The operands of an operation ( PLUS , MULT , and DIV )
        //should have the same type (it can be any type, including STRING and
        //BOOLEAN )
        if(z == -1)
            typeMismatch(t.line_no, "C2");
        return z;

    }
    else if ((t.token_type == SEMICOLON) || (t.token_type == RPAREN))
    {
        //expr -> term
        lexer.UngetToken(t);
        return x;
    }
    else
        syntax_error();

    if (testParse)
        cout << "Done Parsing: " << "expr" << endl;

    return -1;
}

//term -> factor MULT term
//term -> factor DIV term
//term -> factor
//checks for C2 type mismatch
int Parser::parse_term()
{
    if(testParse)
        cout << "\nParsing: " << "term" << endl;

    int x = parse_factor();
    Token t = lexer.GetToken();
    if((t.token_type == MULT) || (t.token_type == DIV))
    {
        //term -> factor MULT term
        //term -> factor DIV term
        int y = parse_term();
        int z = unify(x, y);
        //C2: The operands of an operation ( PLUS , MULT , and DIV )
        //should have the same type (it can be any type, including STRING and
        //BOOLEAN )
        if(z == -1)
            typeMismatch(t.line_no, "C2");
        return z;
    }
    else if((t.token_type == PLUS) || (t.token_type == SEMICOLON) || (t.token_type == RPAREN))
    {
        //term -> factor
        lexer.UngetToken(t);
        return x;
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "term" << endl;

    return -1;
}

//factor -> LPAREN expr RPAREN
//factor -> NUM
//factor -> REALNUM
//factor -> ID
int Parser::parse_factor()
{
    if(testParse)
        cout << "\nParsing: " << "factor" << endl;

    Token t = lexer.GetToken();
    int typeReturn = -1;

    if(t.token_type == LPAREN)
    {
        //factor -> LPAREN expr RPAREN
        typeReturn = parse_expr();
        expect(RPAREN);

        return typeReturn;
    }
    else if((t.token_type == NUM) || (t.token_type == REALNUM) || (t.token_type == ID))
    {
        if(t.token_type == NUM)
            typeReturn = myInt;
        else if(t.token_type == REALNUM)
            typeReturn = myReal;
        else if(t.token_type == ID)
        {
            Symbol checkSym = declCheck(t.lexeme);
            //Programmer-defined type used as variable (error code 1.4)
            //If a previously declared type appears in the body of the
            //program, the type is used as a variable.
            if(checkSym.flag == TYPE)
                errorCode(1, 4, checkSym.id);
            if(checkSym.type == -1)
            {
                checkSym.flag = VAR;
                checkSym.type = typeNum;
                typeNum++;
                symTable.push_back(checkSym);
            }
            typeReturn = checkSym.type;
        }

        t = lexer.GetToken();

        if((t.token_type == MULT) || (t.token_type == DIV) ||
           (t.token_type == PLUS) || (t.token_type == SEMICOLON) ||
           (t.token_type == RPAREN))
        {
            //factor -> NUM
            //factor -> REALNUM
            //factor -> ID
            lexer.UngetToken(t);
        }
        else
            syntax_error();

        return typeReturn;
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "factor" << endl;

    return -1;
}

//condition -> ID
//condition -> primary relop primary
//checks for error 1.4
//checks for C3 type mismatch
int Parser::parse_condition()
{
    if(testParse)
        cout << "\nParsing: " << "condition" << endl;

    Token t = lexer.GetToken();
    if((t.token_type == NUM) || (t.token_type == REALNUM))
    {
        //condition -> primary relop primary
        int x;
        if(t.token_type == NUM)
            x = myInt;
        else
            x = myReal;
        parse_relop();
        int y = parse_primary();
        //C3: The operands of a relational operator (see relop in
        //grammar) should have the same type (it can be any type,
        //including STRING and BOOLEAN )
        if(unify(x, y) == -1)
            typeMismatch(t.line_no, "C3");
        else
            return myBool;
    }
    else if(t.token_type == ID)
    {
        Token t2 = peek();

        if((t2.token_type == GREATER) || (t2.token_type == GTEQ) ||
           (t2.token_type == LESS) || (t2.token_type == LTEQ) || (t2.token_type == NOTEQUAL))
        {
            //condition -> primary relop primary
            lexer.UngetToken(t);
            int x = parse_primary();
            parse_relop();
            int y = parse_primary();
            //C3: The operands of a relational operator (see relop in
            //grammar) should have the same type (it can be any type,
            //including STRING and BOOLEAN )
            int z = unify(x, y);
            if(z == -1)
                typeMismatch(t.line_no, "C3");
            else
                return myBool;
        }
        else if((t2.token_type == LBRACE) || (t2.token_type == SEMICOLON))
        {
            //condition -> ID
            Symbol checkSym = declCheck(t.lexeme);
            //Programmer-defined type used as variable (error code 1.4)
            //If a previously declared type appears in the body of the program,
            //the type is used as a variable.
            if(checkSym.flag == TYPE)
                errorCode(1, 4, checkSym.id);
            else if(checkSym.type == -1) //Boolean implicitly declared
            {
                checkSym.flag = VAR;
                checkSym.type = myBool;
                symTable.push_back(checkSym);
                return myBool;
            }
            else if(checkSym.type == myBool)
                return myBool;
            else
                return  -1;
        }
        else
            syntax_error();
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "condition" << endl;

    return -1;
}

//primary -> ID
//primary -> NUM
//primary -> REALNUM
int Parser::parse_primary()
{
    if(testParse)
        cout << "\nParsing: " << "primary" << endl;

    Token t = lexer.GetToken();
    if((t.token_type == ID) || (t.token_type == NUM) || (t.token_type == REALNUM))
    {
        //primary -> ID
        if(t.token_type == ID)
        {
            Symbol checkSym = declCheck(t.lexeme);
            //Programmer-defined type used as variable (error code 1.4)
            //If a previously declared type appears in the body of the
            //program, the type is used as a variable.
            if(checkSym.flag == TYPE)
                errorCode(1, 4, checkSym.id);
            if(checkSym.type == -1)
            {
                checkSym.flag = VAR;
                checkSym.type = typeNum;
                typeNum++;
                symTable.push_back(checkSym);
            }

            return checkSym.type;
        }
        //primary -> NUM
        else if(t.token_type == NUM)
            return myInt;
        else if(t.token_type == REALNUM)
            return myReal;
        //primary -> REALNUM
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "primary" << endl;

    return -1;
}

//relop-> GREATER
//relop-> GTEQ
//relop-> LESS
//relop-> NOTEQUAL
//relop-> LTEQ
void Parser::parse_relop()
{
    if(testParse)
        cout << "\nParsing: " << "relop" << endl;

    Token t = lexer.GetToken();
    if(testMatch)
        cout << t.lexeme << endl;
    if((t.token_type == GREATER) || (t.token_type == GTEQ) ||
       (t.token_type == LESS) || (t.token_type == LTEQ) || (t.token_type == NOTEQUAL))
    {
        //relop-> GREATER
        //relop-> GTEQ
        //relop-> LESS
        //relop-> NOTEQUAL
        //relop-> LTEQ
    }
    else
        syntax_error();

    if(testParse)
        cout << "Done Parsing: " << "relop" << endl;
}


/************************************
 * Functions I created from scratch *
 ************************************/

//Loads int, real, boolean, string, and long into symbol table
void Parser::loadDefaultSyms()
{
    //Listed in different order than in spec
    //so as to make outputting to spec easier
    Symbol tempSym;
    tempSym.flag = TYPE;
    tempSym.declared = 0; //counting defaults as implicit declarations
    tempSym.id = "BOOLEAN";
    tempSym.type = myBool;
    symTable.push_back(tempSym);

    tempSym.id = "INT";
    tempSym.type = myInt;
    symTable.push_back(tempSym);

    tempSym.id = "LONG";
    tempSym.type = myLong;
    symTable.push_back(tempSym);

    tempSym.id = "REAL";
    tempSym.type = myReal;
    symTable.push_back(tempSym);

    tempSym.id = "STRING";
    tempSym.type = myString;
    symTable.push_back(tempSym);

}

//outputs error code
//cat = category 1 or 2, spec = specific error
void Parser::errorCode(int cat, int spec, string symbol)
{
    cout << "ERROR CODE " << cat << "." << spec << " " << symbol << endl;
    exit(1);
}

//outputs type mismatch error
void Parser::typeMismatch(int lineNo, string constraint)
{
    cout << "TYPE MISMATCH " << lineNo << " " << constraint << endl;
    exit(1);
}

//Check to see if item is in symbol table
Parser::Symbol Parser::declCheck(string name)
{
    Symbol notFound;
    notFound.id = name;
    notFound.flag = ERROR;
    notFound.type = -1;
    notFound.declared = 0;

    for(int iter = 0; iter < (int)symTable.size(); iter++)
    {
        //Remember, string comparison returns 0 if strings are equal
        if(name.compare((symTable[iter]).id) == 0)
        {
            return symTable[iter];
        }
    }

    return notFound;
}


//Print types and variables
void Parser::print()
{
    if(testPrint)
    {
        cout << "\nList of IDs:" << endl;
        cout << "NAME \t\tFLAG \tTYPE \tEXP_DECL" << endl;
        for (int i = 0; i < (int)symTable.size(); i++)
        {
            if((i == myBool))
                cout << symTable[i].id << " \t";
            else
                cout << symTable[i].id << " \t\t";

            if (symTable[i].flag == TYPE)
                cout << "TYPE \t";
            else if (symTable[i].flag == VAR)
                cout << "VAR \t";
            else
                cout << "ERROR :\t";
            cout << symTable[i].type << " \t";
            cout << boolalpha << symTable[i].declared << endl;

        }
        cout << "End of list\n" << endl;
    }

    /*
    //From PDF file
    for each built-in type T:
    {
        output T
        output all names that are type-equivalent with T in order of their appearance
        mark outputted names to avoid re-printing them later
        output "#\n"
    }
    if there are unprinted names left:
    {
        for each unprinted name N in order of appearance:
        {
            output N
            output all other names that are type-equivalent with N in order of their appearance
            output "#\n"
        }
    }
    */

    for(int i = 0; i < 5; i++)
    {
        cout << symTable[i].id << " ";
        for(int j = 5; j < (int)symTable.size(); j++)
        {
            if(symTable[j].type == i)
            {
                cout << symTable[j].id << " ";
                symTable[j].printed = true;
            }
        }
        cout << "#" << endl;
    }

    for(int i = 5; i < (int)symTable.size(); i++)
    {
        if(!symTable[i].printed)
        {
            for(int j = 5; j < (int)symTable.size(); j++) //can start with i because all priors will have gone through
            {
                if(symTable[j].type == symTable[i].type)
                {
                    cout << symTable[j].id << " ";
                    symTable[j].printed = true;
                }
            }
            cout << "#" << endl;
        }
    }

}

//Determine which common type (if possible) typeNum1 and typeNum2 can be equal to
int Parser::unify(int typeNum1, int typeNum2)
{
    int newType;

    if(typeNum1 == typeNum2) //both types are the same
        newType = typeNum1;
    else if((typeNum1 < 5) && (typeNum2 < 5)) //not same type, are different built-in types
        newType = -1; //-1 for type mismatch
    else if(typeNum1 < 5) //first is built in, second is not
    {
        //change all symbols of typeNum2 to that of typeNum1
        for(int i = 0; i < (int)symTable.size(); i++)
        {
            if(symTable[i].type == typeNum2)
                symTable[i].type = typeNum1;
        }
        newType = typeNum1;
    }
    else //either the second is built in and the first is not, or neither is built in
    {
        //change all symbols of typeNum1 to that of typeNum2
        for(int i = 0; i < (int)symTable.size(); i++)
        {
            if(symTable[i].type == typeNum1)
                symTable[i].type = typeNum2;
        }
        newType = typeNum2;
    }

    return newType;
}


//Teacher's function
void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
}

int main()
{
    tester();

    Parser parser;

    parser.ParseInput();

    parser.print();
}