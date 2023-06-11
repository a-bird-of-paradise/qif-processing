#pragma once
#include "location.hh"
#include "astnode.hpp"

namespace qif {

    class Context 
    {
    public:
        Context() : done(false) { loc.initialize(); }
        bool done;
        std::unique_ptr<AST::tree_node> AST;
        location loc;
    };

}