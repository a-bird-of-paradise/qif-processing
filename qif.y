%language "C++"
%require "3.8"
%defines "Parser.hpp"
%output "Parser.cpp"
%define api.token.constructor
%define api.value.type variant
%define api.value.automove
%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full
%locations
%define api.token.prefix {TOK_}
%define api.parser.class {Parser}
%define api.namespace {qif}
%parse-param { Scanner& scanner } { Context& ctx }
%lex-param { Context& ctx }
%code requires 
{
namespace qif {class Scanner; class Context; } 
#include "astnode.hpp"
} 
%code
{
#include "Scanner.hpp"
#define yylex scanner.lex
}

%token SEPARATOR ACCOUNT_HEADER NEWLINE
%token <std::string> LIST_HEADER TRANSACTION_HEADER DATUM

%nonassoc LOWER_THAN_TRANSACTION_HEADER
%nonassoc TRANSACTION_HEADER

%type <std::unique_ptr<AST::tree_node> > qif sections account_transaction_list bare_list
%type <std::unique_ptr<AST::tree_node> > section list lists account_list
%type <std::unique_ptr<AST::tree_node> > transaction_list 
%type <std::unique_ptr<AST::tree_node> > data

%type <std::unique_ptr<AST::tree_node> > data_row

%type <std::string> bare_list_header_row transaction_list_header_row

%type separator_row

%start qif

%%

qif: 
    lists
    {
        ctx.AST = $1;
    }
    ;

lists: 
    list
    { 
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::Root);
        $$->add_child($1); 
    }   
    | lists list        
    { 
        $$ = $1; 
        $$->add_child($2); 
    };

list:   
    account_transaction_list 
    | account_list 
    | bare_list 
    | transaction_list
    ;

account_transaction_list:   
    account_header_row section transaction_list
    {
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::AccountTransactionList);
        $$->add_child($2);
        $$->add_child($3);
    }
    ;

account_list:   
    account_header_row sections
    {
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::AccountList);
        $$->add_child($2);
    }
    ;

bare_list: 
    bare_list_header_row sections
    {
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::BareList);
        $$->data() = $1;
        $$->add_child($2);
    }
    ;

transaction_list: 
    transaction_list_header_row sections
    {
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::TransactionList);
        $$->data() = $1;
        $$->add_child($2);
    }
    ;

sections: 
    section %prec LOWER_THAN_TRANSACTION_HEADER
    {
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::List);
        $$->add_child($1);   
    }            
    | sections section  
    {   
        $$ = $1; 
        $$->add_child($2);  
    }
    ;

data_row:
    DATUM NEWLINES  
    {   
        $$ = std::make_unique<AST::tree_node>($1, AST::tree_node_type::Field);      
    }
    ;

data: 
    data_row            
    {   
        $$ = std::make_unique<AST::tree_node>(AST::tree_node_type::Record);
        $$->add_child($1);   
    }
    | data data_row     
    {   
        $$ = $1; 
        $$->add_child($2);
    }
    ;

account_header_row: 
    ACCOUNT_HEADER NEWLINES 
    ;

bare_list_header_row: 
    LIST_HEADER NEWLINES
    ;

transaction_list_header_row: 
    TRANSACTION_HEADER NEWLINES 
    ;

separator_row: 
    SEPARATOR NEWLINES
    | SEPARATOR YYEOF 
    ;

section: 
    data separator_row       
    ;

NEWLINES: 
    NEWLINES NEWLINE 
    | NEWLINE
    ;

%%

void qif::Parser::error(qif::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}