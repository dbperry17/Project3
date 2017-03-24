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
    void print();
    void loadDefaultSyms();

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
    int parse_type_name(TokenType flag);
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
    int parse_expr();
    int parse_term();
    int parse_factor();
    void parse_condition();
    void parse_primary();
    void parse_relop();

    //mine
    void errorCode(int cat, int spec, std::string symbol); //cat = category 1 or 2, spec = specific error
    void typeMismatch(int lineNo, std::string constraint);
    Symbol declCheck(std::string name);
    int unify(int typeNum1, int typeNum2);
};

#endif

