%{
#include "Scanner-Internal.hpp"
#include "Parser.hpp"
#include "astnode.hpp"
#include <string>
#undef  YY_DECL
#define YY_DECL qif::Parser::symbol_type qif::Scanner::lex(qif::Context& context)
#define YY_USER_ACTION context.loc.step(); context.loc.columns(yyleng);
%}
%option c++ noyywrap nodefault noyylineno 8bit align
%option outfile="Scanner.cpp"
%option batch
%option yyclass="qif::Scanner" 
%option prefix="qif"

%s HEADER_STATE

ANY     [^\r\n]*
HEADER  [^!:\r\n]*

%%
(\n|\r\n?)  {   context.loc.lines();   return qif::Parser::make_NEWLINE(context.loc);   }

!Account    {   return qif::Parser::make_ACCOUNT_HEADER(context.loc);                   }  

^\^{ANY}    {   return qif::Parser::make_SEPARATOR(context.loc);                        }

!{HEADER}:  { BEGIN(HEADER_STATE); }

<HEADER_STATE>Class |
Cat                 |
Memorized           |
Prices              |
Budget              |
InvItem             |
Security            |
Template {   
    BEGIN(INITIAL); 
    return qif::Parser::make_LIST_HEADER(std::string(yytext), context.loc);
}

<HEADER_STATE>Bank  |
Cash                |
CCard               |
Invst               |
Oth\ A              |
Oth\ L              |
Invoice             |
Tax                 |
Bill {   
    BEGIN(INITIAL); 
    return qif::Parser::make_TRANSACTION_HEADER(std::string(yytext), context.loc);
}

^[^!]{ANY}  {   return qif::Parser::make_DATUM(std::string(yytext), context.loc);   }

<<EOF>> {   context.done = true; return qif::Parser::make_YYEOF(context.loc);       }
