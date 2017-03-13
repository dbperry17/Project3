/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
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
    // program -> decl body
    parse_decl();
    parse_body();
}

void Parser::parse_decl()
{
    // decl -> type_decl_section var_decl_section
    parse_type_decl_section();
    parse_var_decl_section();
}

void Parser::parse_type_decl_section()
{
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
}

void Parser::parse_type_decl_list()
{
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
}

void Parser::parse_type_decl()
{
    // type_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
}

void Parser::parse_type_name()
{
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
}

void Parser::parse_var_decl_section()
{
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
}

void Parser::parse_var_decl_list()
{
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
}

void Parser::parse_var_decl()
{
    // var_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
}

void Parser::parse_id_list()
{
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
}


void Parser::parse_body()
{
    // body -> LBRACE stmt_list RBRACE
    expect(LBRACE);
    parse_stmt_list();
    expect(RBRACE);
}

void Parser::parse_stmt_list()
{
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
}

void Parser::parse_stmt()
{
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
}

//assign_stmt -> ID EQUAL expr SEMICOLON
void Parser::parse_assign_stmt()
{
    // TODO

}

//while_stmt -> WHILE condition body
void Parser::parse_while_stmt()
{
    // TODO
}

//do_stmt -> DO body WHILE condition SEMICOLON
void Parser::parse_do_stmt()
{
    // TODO
}

//switch_stmt -> SWITCH ID LBRACE case_list RBRACE
void Parser::parse_switch_stmt()
{
    // TODO
}

//case_list -> case case_list
//case_list -> case
void Parser::parse_case_list()
{
    // TODO
}

//case -> CASE NUM COLON body
void Parser::parse_case()
{
    // TODO
}

//expr -> term PLUS expr
//expr -> term
void Parser::parse_expr()
{
    // TODO
}

//term -> factor MULT term
//term -> factor DIV term
//term -> factor
void Parser::parse_term()
{
    // TODO
}

//factor -> LPAREN expr RPAREN
//factor -> NUM
//factor -> REALNUM
//factor -> ID
void Parser::parse_factor()
{
    // TODO
}

//condition -> ID
//condition -> primary relop primary
void Parser::parse_condition()
{
    // TODO
}

//primary -> ID
//primary -> NUM
//primary -> REALNUM
void Parser::parse_primary()
{
    // TODO
}

//relop-> GREATER
//relop-> GTEQ
//relop-> LESS
//relop-> NOTEQUAL
//relop-> LTEQ
void Parser::parse_relop()
{
    // TODO
}

void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
}

int main()
{
    Parser parser;

    parser.ParseInput();
}

