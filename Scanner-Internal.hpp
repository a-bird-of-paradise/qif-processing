#pragma once
#include "Context.hpp"
#include "Parser.hpp"

namespace qif {

    class Scanner : public qifFlexLexer {
    public:
        Scanner(std::istream& arg_yyin, std::ostream& arg_yyout)
            : qifFlexLexer(arg_yyin, arg_yyout) {}
        Scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr)
            : qifFlexLexer(arg_yyin, arg_yyout) {}
        Parser::symbol_type lex(Context& context); 
    };
}