#pragma once
#include<string>
#include<memory>
#include<vector> 

namespace AST {

    enum class tree_node_type {
        Empty,
        Root, 
        Record, 
        Field,
        List,
        Header,
        Separator,
        AccountList,
        AccountTransactionList,
        TransactionList,
        BareList
    };

    class tree_node
    {
    public:
        tree_node();
        tree_node(const std::string& x, const tree_node_type t);
        tree_node(const tree_node_type t);
        
        auto children() const& -> const std::vector<std::unique_ptr<tree_node>>& { return children_; }
        auto children() &      -> std::vector<std::unique_ptr<tree_node>>&       { return children_; }
        auto children() &&     -> std::vector<std::unique_ptr<tree_node>>&&      { return std::move(children_); }
        
        auto data() const& -> const std::string& { return data_; }
        auto data() &      -> std::string&       { return data_; }
        auto data() &&     -> std::string&&      { return std::move(data_); }
        
        auto type() const& -> const tree_node_type& { return type_; }
        auto type() &      -> tree_node_type&       { return type_; }
        auto type() &&     -> tree_node_type&&      { return std::move(type_); }

        void add_child(std::unique_ptr<tree_node> x);

        tree_node(const tree_node& x);
        tree_node(tree_node&& x);
        tree_node& operator=(const tree_node& x);
        tree_node& operator=(tree_node&& x);
        ~tree_node();

    private:

        std::string data_;
        std::vector<std::unique_ptr<tree_node>> children_;
        tree_node_type type_;
    };

    void tree_print(const tree_node& x, unsigned long long depth = 0, const unsigned long long maxdepth = 999);

}