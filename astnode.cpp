#include "astnode.hpp"
#include <iostream>
#include <string>

std::string text(const AST::tree_node_type& x)
{
    std::string answer = "";

    switch(x) {
        case AST::tree_node_type::Empty:
            answer = "Empty";
        break;
        case AST::tree_node_type::Root:
            answer = "Root";
        break;
        case AST::tree_node_type::Record:
            answer = "Record";
        break;
        case AST::tree_node_type::Field:
            answer = "Field";
        break;
        case AST::tree_node_type::List:
            answer = "List";
        break;
        case AST::tree_node_type::Header:
            answer = "Header";
        break;
        case AST::tree_node_type::Separator:
            answer = "Separator";
        break;
        case AST::tree_node_type::AccountList:
            answer = "Account List";
        break;
        case AST::tree_node_type::AccountTransactionList:
            answer = "Account Transaction List";
        break;
        case AST::tree_node_type::TransactionList:
            answer = "Transaction List";
        break;
        case AST::tree_node_type::BareList:
            answer = "Bare List";
        break;
    }

    return answer;
}

AST::tree_node::tree_node(): type_(AST::tree_node_type::Empty) 
{
    std::cout << "[" << this << "] default constructed" << std::endl;
}

AST::tree_node::tree_node(const tree_node_type t): type_(t) 
{
    std::cout << "[" << this << "] default constructed (type: " << text(t) << ")" << std::endl;
}

AST::tree_node::tree_node(const std::string& x, const tree_node_type t):
data_(x), type_(t) 
{
    std::cout << "[" << this << "] full constructed: data: " << x <<
        ", type: " << text(t) << std::endl;
}

void AST::tree_node::add_child(std::unique_ptr<AST::tree_node> x)
{
    std::cout << "[" << this << "] adding child at " << x.get() << std::endl;
    children_.push_back(std::move(x));
}

AST::tree_node::tree_node(const AST::tree_node& x): data_(x.data()), type_(x.type())
{
    std::cout << "[" << this << "] copy constructed from " << &x << std::endl;
    for(const auto &child_ptr : x.children()){
        children_.push_back(std::make_unique<AST::tree_node>(*child_ptr));
    }
}

AST::tree_node::tree_node(AST::tree_node&& x)
{
    std::cout << "[" << this << "] moved from " << &x << std::endl;
    children_ = std::move(x.children());
    data_ = std::move(x.data());
    type_ = std::move(x.type());
}

AST::tree_node& AST::tree_node::operator=(const AST::tree_node& x)
{
    std::cout << "[" << this << "] set equal to " << &x << std::endl;
    children_.clear();
    data_ = x.data();
    type_ = x.type();

    for(const auto &child_ptr : x.children()){
        children_.push_back(std::make_unique<AST::tree_node>(*child_ptr));
    }

    return *this;
}

AST::tree_node& AST::tree_node::operator=(AST::tree_node&& x)
{
    std::cout << "[" << this << "] set move equal to " << &x << std::endl;
    children_ = std::move(x.children());
    data_ = std::move(x.data());
    type_ = std::move(x.type());

    return *this;
}

AST::tree_node::~tree_node()
{
    std::cout << "[" << this << "] deleted" << std::endl;
}

void AST::tree_print(const AST::tree_node& x, unsigned long long depth, const unsigned long long maxdepth)
{

    // print leading |

    for(unsigned long long i = 0; i < depth; i++) 
    {
        std::cout << "|"; 
    }

    // print out self

    std::cout << "[" << &x << "], data: " << x.data() << ", type: " << text(x.type()) << "\n"; 

    // print out children

    if (depth < maxdepth)
    {
        for(const auto& i: x.children())
        {
            tree_print(*i, depth+1, maxdepth);
        }
    }
}
