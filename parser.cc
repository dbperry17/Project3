/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

bool testing = true;
bool testParse = false;
bool testParseAll = false;
bool testStore = true;

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
    }
}


/***********************
 * Teacher's functions *
 ***********************/

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
void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}


//Wrote special version for testing purposes
void Parser::syntax_error(int x)
{
    if(testParse)
        cout << "Syntax Error: " << x << endl;
    else
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

void Parser::parse_program()
{
    if(testParse)
        cout << "\nParsing: " << "program" << endl;
    // program -> decl body
    parse_decl();
    parse_body();
    if(testParse)
        cout << "Done Parsing: " << "program" << endl;
}

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

void Parser::parse_type_decl_section()
{
    if(testParseAll)
        cout << "\nParsing: " << "decl_section" << endl;

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
        cout << "Done Parsing: " << "decl_section" << endl;
}

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

void Parser::parse_type_decl()
{
    if(testParseAll)
        cout << "\nParsing: " << "type_decl" << endl;

    // type_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);

    if(testParseAll)
        cout << "Done Parsing: " << "type_decl" << endl;
}

void Parser::parse_type_name()
{
    if(testParseAll)
        cout << "\nParsing: " << "type_name" << endl;

    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID
    Token t = lexer.GetToken();
    if (t.token_type == REAL)
    {
        // type_name -> REAL
    }
    else if (t.token_type == INT)
    {
        // type_name -> INT
    }
    else if (t.token_type == BOOLEAN)
    {
        // type_name -> BOOLEAN
    }
    else if (t.token_type == STRING)
    {
        // type_name -> STRING
    }
    else if (t.token_type == LONG)
    {
        // type_name -> LONG
    }
    else if (t.token_type == ID)
    {
        // type_name -> ID
    }
    else
    {
        syntax_error();
    }



    if(testParseAll)
        cout << "Done Parsing: " << "type_name" << endl;
}

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

void Parser::parse_var_decl()
{
    if(testParseAll)
        cout << "\nParsing: " << "var_decl" << endl;

    // var_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);

    if(testParseAll)
        cout << "Done Parsing: " << "var_decl" << endl;
}

void Parser::parse_id_list()
{
    if(testParse)
        cout << "\nParsing: " << "id_list" << endl;

    // id_list -> ID
    // id_list -> ID COMMA id_list

    expect(ID);
    Token t = lexer.GetToken();
    if (t.token_type == COMMA)
    {
        // id_list -> ID COMMA id_list
        parse_id_list();
    }
    else if (t.token_type == COLON)
    {
        // id_list -> ID
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error();
    }

    if(testParse)
        cout << "Done Parsing: " << "id_list" << endl;
}


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
void Parser::parse_assign_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "assign_stmt" << endl;

    // TODO
    expect(ID);
    expect(EQUAL);
    parse_expr();
    expect(SEMICOLON);

    if(testParse)
        cout << "Done Parsing: " << "assign_stmt" << endl;
}

//while_stmt -> WHILE condition body
void Parser::parse_while_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "while_stmt" << endl;

    // TODO
    expect(WHILE);
    parse_condition();
    parse_body();

    if(testParse)
        cout << "Done Parsing: " << "while_stmt" << endl;
}

//do_stmt -> DO body WHILE condition SEMICOLON
void Parser::parse_do_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "do_stmt" << endl;

    // TODO
    expect(DO);
    parse_body();
    expect(WHILE);
    parse_condition();
    expect(SEMICOLON);

    if(testParse)
        cout << "Done Parsing: " << "do_stmt" << endl;
}

//switch_stmt -> SWITCH ID LBRACE case_list RBRACE
void Parser::parse_switch_stmt()
{
    if(testParse)
        cout << "\nParsing: " << "switch_stmt" << endl;

    // TODO
    expect(SWITCH);
    expect(ID);
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

    // TODO
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
        syntax_error(8);

    if(testParse)
        cout << "Done Parsing: " << "case_list" << endl;

}

//case -> CASE NUM COLON body
void Parser::parse_case()
{
    if(testParse)
        cout << "\nParsing: " << "case" << endl;

    // TODO
    expect(CASE);
    expect(NUM);
    expect(COLON);
    parse_body();

    if(testParse)
        cout << "Done Parsing: " << "case" << endl;
}

//expr -> term PLUS expr
//expr -> term
void Parser::parse_expr()
{
    if (testParse)
        cout << "\nParsing: " << "expr" << endl;

    // TODO
    parse_term();
    Token t = lexer.GetToken();
    if (t.token_type == PLUS)
    {
        //expr -> term PLUS expr
        parse_expr();
    }
    else if ((t.token_type == SEMICOLON) || (t.token_type == RPAREN))
    {
        //expr -> term
        lexer.UngetToken(t);
    }
    else
        syntax_error(7);

    if (testParse)
        cout << "Done Parsing: " << "expr" << endl;
}

//term -> factor MULT term
//term -> factor DIV term
//term -> factor
void Parser::parse_term()
{
    if(testParse)
        cout << "\nParsing: " << "term" << endl;

    // TODO
    parse_factor();
    Token t = lexer.GetToken();
    if(t.token_type == MULT)
    {
        //term -> factor MULT term
        parse_term();
    }
    else if(t.token_type == DIV)
    {
        //term -> factor DIV term
        parse_term();
    }
    else if((t.token_type == PLUS) || (t.token_type == SEMICOLON) || (t.token_type == RPAREN))
    {
        //term -> factor
        lexer.UngetToken(t);
    }
    else
        syntax_error(6);

    if(testParse)
        cout << "Done Parsing: " << "term" << endl;
}

//factor -> LPAREN expr RPAREN
//factor -> NUM
//factor -> REALNUM
//factor -> ID
void Parser::parse_factor()
{
    if(testParse)
        cout << "\nParsing: " << "factor" << endl;

    // TODO
    Token t = lexer.GetToken();
    if(testParse)
    {
        cout << "Token: " << endl;
        t.Print();
    }

    if(t.token_type == LPAREN)
    {
        //factor -> LPAREN expr RPAREN
        parse_expr();
        expect(RPAREN);
    }
    else if((t.token_type == NUM) || (t.token_type == REALNUM) || (t.token_type == ID))
    {
        t = lexer.GetToken();
        if(testParse)
        {
            cout << "Token: " << endl;
            t.Print();
        }

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
            syntax_error(5);
    }
    else
        syntax_error(4);

    if(testParse)
        cout << "Done Parsing: " << "factor" << endl;
}

//condition -> ID
//condition -> primary relop primary
void Parser::parse_condition()
{
    if(testParse)
        cout << "\nParsing: " << "condition" << endl;

    // TODO
    Token t = lexer.GetToken();
    if((t.token_type == NUM) || (t.token_type == REALNUM))
    {
        //condition -> primary relop primary
        parse_primary();
        parse_relop();
        parse_primary();
    }
    else if(t.token_type == ID)
    {

        Token t2 = peek();


        if((t2.token_type == GREATER) || (t2.token_type == GTEQ) ||
           (t2.token_type == LESS) || (t2.token_type == LTEQ) || (t2.token_type == NOTEQUAL))
        {
            //condition -> primary relop primary
            lexer.UngetToken(t);
            parse_primary();
            parse_relop();
            parse_primary();
        }
        else if((t2.token_type == LBRACE) || (t2.token_type == SEMICOLON))
        {
            //condition -> ID
        }
        else
            syntax_error(3);
    }
    else
        syntax_error(2);

    if(testParse)
        cout << "Done Parsing: " << "condition" << endl;
}

//primary -> ID
//primary -> NUM
//primary -> REALNUM
void Parser::parse_primary()
{
    if(testParse)
        cout << "\nParsing: " << "primary" << endl;

    // TODO
    Token t = lexer.GetToken();
    if(testParse)
    {
        cout << "Token: " << endl;
        t.Print();
    }
    if((t.token_type == ID) || (t.token_type == NUM) || (t.token_type == REALNUM))
    {
        //primary -> ID
        //primary -> NUM
        //primary -> REALNUM
    }
    else
        syntax_error(1);

    if(testParse)
        cout << "Done Parsing: " << "primary" << endl;
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

    // TODO
    Token t = lexer.GetToken();
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
        syntax_error(0);

    if(testParse)
        cout << "Done Parsing: " << "relop" << endl;
}

/************************************
 * Functions I created from scratch *
 ************************************/

//cat = category 1 or 2, spec = specific error
void Parser::errorCode(int cat, int spec, std::string symbol)
{
    cout << "ERROR CODE " << cat << "." << spec << " " << symbol << endl;
    exit(1);
}

/*
int Parser::declCheck(Symbol sym)
{
    int pos = -1;

    for(int iter = 0; iter < symTable.size(); iter++)
    {
        //Remember, string comparison returns 0 if strings are equal
        if((sym.name).compare((symTable[iter]).name) == 0)
        {
            pos = iter;
            break;
        }
    }

    return pos;
}

int Parser::declCheck(string lexeme)
{
    Symbol temp;
    temp.name = lexeme;
    return declCheck(temp);
}
*/


//Teacher's function
void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
}
/*
void print()
{
    cout << "\nList of IDs:" << endl;
    for(int i = 0; i < symTable.size(); i++)
    {
        cout << "Var: " << symTable[i].name << endl;
        if(symTable[i].type.empty())
            cout << "Type: UNKNOWN" << endl;
        else
            cout << "Type: " << symTable[i].type << endl;
    }
    cout << "End of list" << endl;
}
*/
int main()
{
    tester();

    Parser parser;

    parser.ParseInput();
}

