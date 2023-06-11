#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "astnode.hpp"
#include <chrono>
#include <sstream>
#include "date.h" 
#include "date-parsers.hpp"
#include "Scanner.hpp"

namespace qif {
    class QifObject
    {
    public:
        virtual std::ostream& write(std::ostream &dest) const = 0;
        friend std::ostream& operator<<(std::ostream &os, QifObject const &m) { return m.write(os); }
    };

    class QifDatum : public QifObject
    {
    public:
        QifDatum(const AST::tree_node& node, const qif::DateParser& dp);
        QifDatum() {}
        virtual std::ostream& write(std::ostream& dest) const { dest << data_; return dest; }

        auto data() const& -> const std::string& { return data_; }
        auto data() &      -> std::string&       { return data_; }
        auto data() &&     -> std::string&&      { return std::move(data_); }

    private:
        std::string data_;
    };

    class QifHeader : public QifObject
    {
    public:
        QifHeader(const AST::tree_node& node, const qif::DateParser& dp);
        QifHeader() {}
        virtual std::ostream& write(std::ostream& dest) const { dest << data_; return dest; }

        auto data() const& -> const std::string& { return data_; }
        auto data() &      -> std::string&       { return data_; }
        auto data() &&     -> std::string&&      { return std::move(data_); }

    private:
        std::string data_;
    };

    class QifSecurity : public QifObject
    {
    public:
        QifSecurity(const AST::tree_node& node, const qif::DateParser& dp);
        QifSecurity() : completed(false) {}
        virtual std::ostream& write(std::ostream& dest) const;

        auto name() const& -> const std::string& { return name_; }
        auto name() &      -> std::string&       { return name_; }
        auto name() &&     -> std::string&&      { return std::move(name_); }

        auto symbol() const& -> const std::string& { return symbol_; }
        auto symbol() &      -> std::string&       { return symbol_; }
        auto symbol() &&     -> std::string&&      { return std::move(symbol_); }

        auto goal() const& -> const std::string& { return goal_; }
        auto goal() &      -> std::string&       { return goal_; }
        auto goal() &&     -> std::string&&      { return std::move(goal_); }

        auto type() const& -> const std::string& { return type_; }
        auto type() &      -> std::string&       { return type_; }
        auto type() &&     -> std::string&&      { return std::move(type_); }

    private:
        std::string name_, symbol_, goal_, type_;
        bool completed;
    };

    class QifCategory : public QifObject
    {
    public:
        QifCategory(const AST::tree_node& node, const qif::DateParser& dp);
        QifCategory() : completed(false), is_income_category_(false), is_tax_related_(false), has_budget_(false) {}
        virtual std::ostream& write(std::ostream& dest) const;

        auto name() const& -> const std::string& { return name_; }
        auto name() &      -> std::string&       { return name_; }
        auto name() &&     -> std::string&&      { return std::move(name_); }

        auto description() const& -> const std::string& { return description_; }
        auto description() &      -> std::string&       { return description_; }
        auto description() &&     -> std::string&&      { return std::move(description_); }

        auto tax_schedule() const& -> const std::string& { return tax_schedule_; }
        auto tax_schedule() &      -> std::string&       { return tax_schedule_; }
        auto tax_schedule() &&     -> std::string&&      { return std::move(tax_schedule_); }

        auto is_tax_related() const& -> const bool& { return is_tax_related_; }
        auto is_tax_related() &      -> bool&       { return is_tax_related_; }
        auto is_tax_related() &&     -> bool&&      { return std::move(is_tax_related_); }

        auto is_income_category() const& -> const bool& { return is_income_category_; }
        auto is_income_category() &      -> bool&       { return is_income_category_; }
        auto is_income_category() &&     -> bool&&      { return std::move(is_income_category_); }

        auto budget_amount() const& -> const double& { return budget_amount_; }
        auto budget_amount() &      -> double&       { return budget_amount_; }
        auto budget_amount() &&     -> double&&      { return std::move(budget_amount_); }
    private:
        std::string name_, description_, tax_schedule_;
        bool is_tax_related_, is_income_category_, completed, has_budget_;
        double budget_amount_;
    };

    class QifClass : public QifObject
    {
    public:
        QifClass(const AST::tree_node& node, const qif::DateParser& dp);
        QifClass() : completed(false) {}
        virtual std::ostream& write(std::ostream& dest) const;

        auto name() const& -> const std::string& { return name_; }
        auto name() &      -> std::string&       { return name_; }
        auto name() &&     -> std::string&&      { return std::move(name_); }

        auto description() const& -> const std::string& { return description_; }
        auto description() &      -> std::string&       { return description_; }
        auto description() &&     -> std::string&&      { return std::move(description_); }
    private:
        std::string name_, description_;
        bool completed;
    };

    template<class T>
    class QifList : public QifObject
    {
    public:
        QifList(const AST::tree_node& node, const qif::DateParser& dp);
        QifList() {}
        virtual std::ostream& write(std::ostream& dest) const;

        virtual void append(const QifList<T>& toAppend);

        auto collection() const& -> const std::vector<T>& { return collection_; }
        auto collection() &      -> std::vector<T>&       { return collection_; }
        auto collection() &&     -> std::vector<T>&&      { return std::move(collection_); }

    protected:
        std::vector<T> collection_;
    };

    class QifTransaction : public QifObject
    {
    public:
        QifTransaction(const AST::tree_node& node, const qif::DateParser& dp);
        QifTransaction() : completed(false), value_(0) {}
        virtual std::ostream& write(std::ostream& dest) const;

        auto memo() const& -> const std::string& { return memo_; }
        auto memo() &      -> std::string&       { return memo_; }
        auto memo() &&     -> std::string&&      { return std::move(memo_); }

        auto payee() const& -> const std::string& { return payee_; }
        auto payee() &      -> std::string&       { return payee_; }
        auto payee() &&     -> std::string&&      { return std::move(payee_); }

        auto category() const& -> const std::string& { return category_; }
        auto category() &      -> std::string&       { return category_; }
        auto category() &&     -> std::string&&      { return std::move(category_); }

        auto address() const& -> const std::string& { return address_; }
        auto address() &      -> std::string&       { return address_; }
        auto address() &&     -> std::string&&      { return std::move(address_); }

        auto date() const& -> const date::year_month_day& { return date_; }
        auto date() &      -> date::year_month_day&       { return date_; }
        auto date() &&     -> date::year_month_day&&      { return std::move(date_); }

        constexpr std::weak_ordering operator<=>(const QifTransaction& other) const noexcept
        {
            if( date().year() < other.date().year()) {return std::weak_ordering::less;}
            else if( date().year() > other.date().year()) {return std::weak_ordering::greater;}
            else if( date().month() < other.date().month()) {return std::weak_ordering::less;}
            else if( date().month() > other.date().month()) {return std::weak_ordering::greater;}
            else if( date().day() < other.date().day()) {return std::weak_ordering::less;}
            else if( date().day() > other.date().day()) {return std::weak_ordering::greater;}
            else {return std::weak_ordering::equivalent;}
        }
    private:
        double value_;
        date::year_month_day date_;
        std::string memo_, payee_, category_, address_;
        bool completed;
    };

    class QifTransactionList : public QifList<QifTransaction>
    {
    public:
        QifTransactionList(const AST::tree_node& node, const qif::DateParser& dp);
        QifTransactionList() {}
        virtual std::ostream& write(std::ostream& dest) const;

        auto type() const& -> const std::string& { return type_; }
        auto type() &      -> std::string&       { return type_; }
        auto type() &&     -> std::string&&      { return std::move(type_); }

    private:
        std::string type_; 
    };

    class QifAccount : public QifObject
    {
    public:
        QifAccount(const AST::tree_node& node, const qif::DateParser& dp);
        QifAccount() : completed(false), has_balance(false), balance_(0), date_(date::day{1}/1/2000){}
        virtual std::ostream& write(std::ostream& dest) const;
        auto transactions() const& -> const QifTransactionList& { return transactions_; }
        auto transactions() &      -> QifTransactionList&       { return transactions_; }
        auto transactions() &&     -> QifTransactionList&&      { return std::move(transactions_); }

        auto name() const& -> const std::string& { return name_; }
        auto name() &      -> std::string&       { return name_; }
        auto name() &&     -> std::string&&      { return std::move(name_); }

        auto description() const& -> const std::string& { return description_; }
        auto description() &      -> std::string&       { return description_; }
        auto description() &&     -> std::string&&      { return std::move(description_); }

        auto is_completed() const& -> const bool& { return completed; }
        auto is_completed() &      -> bool&       { return completed; }
        auto is_completed() &&     -> bool&&      { return std::move(completed); }

    private:
        std::string name_, description_, type_;
        bool completed, has_balance;
        date::year_month_day date_;
        double balance_;
        QifTransactionList transactions_; 
    };

    class QifRoot : public QifObject
    {
    public:
        QifRoot(const qif::DateParser& dp) : dateparser_(dp.clone()) {};
        QifRoot(const AST::tree_node& node, const qif::DateParser& dp);
        QifRoot() : dateparser_(std::make_unique<DateParser_dmy_slash>()) {}
        QifRoot(const QifRoot& source) : accounts_(source.accounts_),
            securities_(source.securities_),
            categories_(source.categories_),
            classes_(source.classes_),
            transactions_(source.transactions_),
            dateparser_(source.dateparser_->clone()) {}
        QifRoot& operator=(const QifRoot&  other);
        QifRoot& operator=(      QifRoot&& other);
        virtual std::ostream& write(std::ostream& dest) const; 
        friend std::istream& operator>>(std::istream& is, QifRoot& m);

        auto transactions() const& -> const QifTransactionList& { return transactions_; }
        auto transactions() &      -> QifTransactionList&       { return transactions_; }
        auto transactions() &&     -> QifTransactionList&&      { return std::move(transactions_); }

        auto dateparser() const& -> const std::unique_ptr<DateParser>& { return dateparser_; }
        auto dateparser() &      -> std::unique_ptr<DateParser>&       { return dateparser_; }
        auto dateparser() &&     -> std::unique_ptr<DateParser>&&      { return std::move(dateparser_); }

        auto accounts() const& -> const QifList<QifAccount>& { return accounts_; }
        auto accounts() &      -> QifList<QifAccount>&       { return accounts_; }
        auto accounts() &&     -> QifList<QifAccount>&&      { return std::move(accounts_); }

        QifRoot operator+(const QifRoot& rQifRoot) const;
        QifRoot& operator+=(const QifRoot& QifRoot);

        void add(const QifRoot& src);

    private:
        QifList<QifAccount> accounts_;
        QifList<QifSecurity> securities_;
        QifList<QifCategory> categories_;
        QifList<QifClass> classes_;
        QifTransactionList transactions_;
        std::unique_ptr<DateParser> dateparser_;
    }; 
}