/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Symbol
{
public:
    /*union values
    {
        double realVal;
        int intVal;
        bool boolVal;
        std::string strVal;
        long longVal;
    } value;*/

    std::string name;
    std::string type;
};

class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    void syntax_error(int x);
    Token expect(TokenType expected_type);
    Token peek();

    Symbol tmpSym; //mine
    int symBegin = 0;
    int symEnd;

    void parse_program();
    void parse_decl();
    void parse_type_decl_section();
    void parse_type_decl_list();
    void parse_type_decl();
    void parse_type_name();
    void parse_var_decl_section();
    void parse_var_decl_list();
    void parse_var_decl();
    void parse_id_list();
    void parse_body();
    void parse_stmt_list();
    void parse_stmt();
    void parse_assign_stmt();
    void parse_while_stmt();
    void parse_do_stmt();
    void parse_switch_stmt();
    void parse_case_list();
    void parse_case();
    void parse_expr();
    void parse_term();
    void parse_factor();
    void parse_condition();
    void parse_primary();
    void parse_relop();

    //mine
    void errorCode(int cat, int spec, std::string symbol); //cat = category 1 or 2, spec = specific error
    int declCheck(Symbol sym);
    int declCheck(std::string lexeme);


  public:
    void ParseInput();
};

#endif

