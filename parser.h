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
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

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

  public:
    void ParseInput();
};

#endif

