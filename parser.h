/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Parser {
  public:
    void ParseInput();
    struct idListNode;
    struct Symbol;
    void print(idListNode *head);
    void loadDefaultSyms();

  private:

    LexicalAnalyzer lexer;

    void syntax_error();
    void syntax_error(int x);
    Token expect(TokenType expected_type);
    Token peek();

    void parse_program();
    void parse_decl();
    void parse_type_decl_section();
    void parse_type_decl_list();
    void parse_type_decl();
    void parse_type_name(idListNode *nodes, TokenType flag);
    void parse_var_decl_section();
    void parse_var_decl_list();
    void parse_var_decl();
    idListNode* parse_id_list();
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
    Symbol declCheck(std::string name);
};

#endif

