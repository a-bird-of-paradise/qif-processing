#include "QifObjects.hpp"
#include <iomanip>

#pragma region QifDatum

qif::QifDatum::QifDatum(const AST::tree_node& node, const qif::DateParser& dp)
{
    if(node.type() == AST::tree_node_type::Field)
    {
        data_ = node.data();
    }
}

#pragma endregion

#pragma region QifHeader

qif::QifHeader::QifHeader(const AST::tree_node& node, const qif::DateParser& dp)
{
    if(node.type() == AST::tree_node_type::Header)
    {
        data_ = node.data();
    }
}

#pragma endregion

#pragma region QifSecurity

qif::QifSecurity::QifSecurity(const AST::tree_node& node, const qif::DateParser& dp) : completed(false)
{
    if(node.type() == AST::tree_node_type::Record)
    {
        for(const auto& s : node.children())
        {
            if(s->type() == AST::tree_node_type::Field) {
                if(s->data().front()=='N') { 
                    name_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'T') {
                    type_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'G') {
                    goal_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'S') {
                    symbol_ = s->data().substr(1,std::string::npos);}
                else { 
                    std::cout << "Warning: don't know how to use datum " << s->data() << " in Security" << std::endl; }
                
                if(completed) {
                    std::cout << "Warning: incorporating datum after record terminator in Security" << std::endl; }
                }
            else if(s->type() == AST::tree_node_type::Separator)
            {
                if(completed) std::cout << "Warning: multiple separators in Security record" << std::endl;
                completed = true;
            }
            else
            {
                std::cout << "Warning: ignoring unknown element in Security record" << std::endl;
            }
        }
    }
}

std::ostream& qif::QifSecurity::write(std::ostream& dest) const
{
    dest << 'N' << name_  << "\n"
        << 'S' << symbol_ << "\n"
        << 'T' << type_   << "\n"
        << 'G' << goal_   << "\n"
        << '^' << '\n';

    return dest; 
}

#pragma endregion

#pragma region QifCategory

qif::QifCategory::QifCategory(const AST::tree_node& node, const qif::DateParser& dp) : completed(false), 
    is_income_category_(false), is_tax_related_(false), has_budget_(false)
{
    if(node.type() == AST::tree_node_type::Record)
    {
        for(const auto& s : node.children())
        {
            if(s->type() == AST::tree_node_type::Field) {
                if(s->data().front()=='N') { 
                    name_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'D') {
                    description_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'T') {
                    is_tax_related_ = true; }
                else if(s->data().front() == 'I') {
                    is_income_category_ = true; }
                else if(s->data().front() == 'E') {
                    is_income_category_ = false; }
                else if(s->data().front() == 'B') {
                    has_budget_ = true;
                    std::string input = s->data().substr(1,std::string::npos);
                    std::istringstream buffer(input);
                    buffer.imbue(std::locale(""));
                    buffer >> budget_amount_;}
                else if(s->data().front() == 'R') {
                    tax_schedule_ = s->data().substr(1,std::string::npos);}
                else { 
                    std::cout << "Warning: don't know how to use datum " << s->data() << " in Category" << std::endl; }
                
                if(completed) {
                    std::cout << "Warning: incorporating datum after record terminator in Category" << std::endl; }
                }
            else if(s->type() == AST::tree_node_type::Separator)
            {
                if(completed) std::cout << "Warning: multiple separators in Category record" << std::endl;
                completed = true;
            }
            else
            {
                std::cout << "Warning: ignoring unknown element in Category record" << std::endl;
            }
        }
    }
}

std::ostream& qif::QifCategory::write(std::ostream& dest) const
{
    dest << "N" << name_ << '\n'; 
    if(description_ != "") dest << "D" << description_ << '\n';
    if(is_tax_related_) dest << ", Tax related: schedule = " << tax_schedule_<< '\n';
    if(is_income_category_) dest << ", Income"; else dest << ", Expense";
    if(has_budget_) dest << ", Budget amount = " << budget_amount_;
    dest << "}\n";

    return dest; 
}

#pragma endregion

#pragma region QifClass

qif::QifClass::QifClass(const AST::tree_node& node, const qif::DateParser& dp) : completed(false)
{
    if(node.type() == AST::tree_node_type::Record)
    {
        for(const auto& s : node.children())
        {
            if(s->type() == AST::tree_node_type::Field) {
                if(s->data().front()=='N') { 
                    name_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front() == 'D') {
                    description_ = s->data().substr(1,std::string::npos);}
                else { 
                    std::cout << "Warning: don't know how to use datum " << s->data() << " in Class" << std::endl; }
                
                if(completed) {
                    std::cout << "Warning: incorporating datum after record terminator in Class" << std::endl; }
                }
            else if(s->type() == AST::tree_node_type::Separator)
            {
                if(completed) std::cout << "Warning: multiple separators in Class record" << std::endl;
                completed = true;
            }
            else
            {
                std::cout << "Warning: ignoring unknown element in Class record" << std::endl;
            }
        }
    }
}

std::ostream& qif::QifClass::write(std::ostream& dest) const
{
    dest << 'N' << name_ << '\n';
    if(description_ != "") dest << "D" << description_ << '\n';
    dest << '^' << '\n';

    return dest; 
}

#pragma endregion

#pragma region QifList

template<class T>
qif::QifList<T>::QifList(const AST::tree_node& node, const qif::DateParser& dp)
{

    for(const auto& i : node.children())
    {
        T item(*i, dp);
        collection_.push_back(item);
    }
}

template<class T>
std::ostream& qif::QifList<T>::write(std::ostream& dest) const
{
    for(const auto& i: collection()) { dest << i; }

    return dest; 
}

template<class T>
void qif::QifList<T>::append(const qif::QifList<T>& toAppend)
{
    collection_.insert(collection_.end(), toAppend.collection().begin(), toAppend.collection().end());
}

#pragma endregion

#pragma region QifTransaction

qif::QifTransaction::QifTransaction(const AST::tree_node& node, const qif::DateParser& dp) : completed(false)
{
    if(node.type() == AST::tree_node_type::Record)
    {
        std::cout << "Making transaction from [" << &node << "]" << std::endl;
        for(const auto& s : node.children())
        {
            if(s->type() == AST::tree_node_type::Field) {
                if(s->data().front()=='P') { 
                    payee_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front()=='A') {
                    address_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front()=='L') {
                    category_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front()=='M') {
                    memo_ = s->data().substr(1,std::string::npos);}
                else if(s->data().front()=='T' || s->data().front()=='U') {
                    std::string input = s->data().substr(1,std::string::npos);
                    std::istringstream buffer(input);
                    buffer.imbue(std::locale(""));
                    buffer >> value_;}
                else if(s->data().front()=='D') {
                    date_ = dp(s->data().substr(1,std::string::npos)); }
                else { 
                    std::cout << "Warning: don't know how to use datum " << s->data() << " in Transaction" << std::endl; }

                if(completed) {
                    std::cout << "Warning: incorporating datum after record terminator in Transaction" << std::endl; }
                }
            else if(s->type() == AST::tree_node_type::Separator)
            {
                if(completed) std::cout << "Warning: multiple separators in Transaction record" << std::endl;
                completed = true;
            }
            else
            {
                std::cout << "Warning: ignoring unknown element in Transaction record" << std::endl;
            }
        }
    }
}

std::ostream& qif::QifTransaction::write(std::ostream& dest) const
{
    dest << "D" << std::setw(2) << std::setfill('0')    << (unsigned)date_.day()
            << "/" << std::setw(2) << std::setfill('0') << (unsigned)date_.month()
            << "/" <<  (signed)date_.year() << '\n'
        << "T" << std::fixed << std::setprecision(2) << value_ << '\n';
        if(memo_ != "") dest << "M" << memo_<< '\n';
        if(payee_ != "") dest << "P" << payee_ << '\n';
        if(address_ != "") dest << "A" << address_<< '\n';
        dest << "L" << category_<< '\n';

    dest << '^' << '\n';

    return dest;
}

#pragma endregion

#pragma region QifTransactionList

qif::QifTransactionList::QifTransactionList(const AST::tree_node& node, const qif::DateParser& dp) 
{
    if(node.type() == AST::tree_node_type::TransactionList)
    {
        type_ = node.data();

        std::cout << "Making transaction list [" << type_ << "] from [" << &node << "]" << std::endl;

        if(node.children().size() == 1) // check there's a single child of a list type
            if(node.children()[0]->type() == AST::tree_node_type::List)
                for(const auto& i: node.children()[0]->children())
                {
                    collection_.emplace_back(qif::QifTransaction(*i,dp));
                }

    }
}

std::ostream& qif::QifTransactionList::write(std::ostream& dest) const
{
    dest << "!Type:" << type() << "\n";

    for(const auto& i: collection()) { dest << i; }

    return dest;
}

#pragma endregion

#pragma region QifAccount

qif::QifAccount::QifAccount(const AST::tree_node& node, const qif::DateParser& dp) 
    : completed(false), has_balance(false), balance_(0), date_(date::day{1}/1/2000)
{
    if(node.type() == AST::tree_node_type::Record)
    {
        for(const auto& i : node.children())
        {
            if(i->type() == AST::tree_node_type::Field)
            {
                if(i->data().front() == 'N'){
                    name_ = i->data().substr(1,std::string::npos);
                }
                else if(i->data().front() == 'D'){
                    description_ = i->data().substr(1,std::string::npos);
                }
                else if(i->data().front() == 'T'){
                    type_ = i->data().substr(1,std::string::npos);
                }
                else if(i->data().front() == '$'){
                    has_balance = true;
                    std::string input = i->data().substr(1,std::string::npos);
                    std::istringstream buffer(input);
                    buffer.imbue(std::locale(""));
                    buffer >> balance_;
                }
                else if(i->data().front()=='/') {
                    has_balance = true;
                    date_ = dp(i->data().substr(1,std::string::npos));}
                else { 
                    std::cout << "Warning: don't know how to use datum " << i->data() << " in Account" << std::endl; }
                
                if(completed) {
                    std::cout << "Warning: incorporating datum after record terminator in Account" << std::endl; }
                }
            else if(i->type() == AST::tree_node_type::Separator)
            {
                if(completed) std::cout << "Warning: multiple separators in Account record" << std::endl;
                completed = true;
            }
            else
            {
                std::cout << "Warning: ignoring unknown element in Account record" << std::endl;
            }
        }
    }

    std::cout << "Constructed account from " << &node << std::endl << *this;
}


std::ostream& qif::QifAccount::write(std::ostream& dest) const
{
    dest << "!Account\n" << 'N' << name_ << '\n'
        << "D" << description_ << '\n';
        
    if(type_ != "") dest << "!Type:" << type_ << '\n';

    if(has_balance) dest << "$" << balance_ << '\n'
        << "/" << std::setw(2) << std::setfill('0') << (unsigned)date_.day()
        << "/" << std::setw(2) << std::setfill('0') << (unsigned)date_.month()
        << "/" << (signed)date_.year() << '\n';

    dest << '^' << '\n';

    if(transactions().collection().size() > 0) dest << transactions_ ;
;
    return dest;
}


#pragma endregion

#pragma region QifRoot

qif::QifRoot::QifRoot(const AST::tree_node& node, const qif::DateParser& dp)
{
    dateparser_ = dp.clone();
    if(node.type() == AST::tree_node_type::Root)
    {
        for(const auto& i : node.children())
        {
            if(i->type() == AST::tree_node_type::BareList)
            {
                std::string ListType = i->data();

                if(ListType == "Security"){
                    securities_.append(QifList<QifSecurity>( *i->children()[0], *dateparser_));
                }
                else if(ListType == "Cat"){
                    categories_.append(QifList<QifCategory>( *i->children()[0], *dateparser_));
                }
                else if(ListType == "Class"){
                    classes_.append(QifList<QifClass>( *i->children()[0], *dateparser_));
                }
            }
            else if(i->type() == AST::tree_node_type::TransactionList)
            {
                transactions_ = QifTransactionList(*i, *dateparser_);
            }
            else if(i->type() == AST::tree_node_type::AccountList)
            {
                accounts_.append(QifList<QifAccount>(*(i->children()[0]), *dateparser_));
            }
            else if(i->type() == AST::tree_node_type::AccountTransactionList)
            {
                std::size_t AccountIndex, TransactionListIndex;
                if(i->children()[0]->type() == AST::tree_node_type::Record)
                {
                    AccountIndex = 0;
                }
                else
                {
                    AccountIndex = 1;
                }
                TransactionListIndex = 1 - AccountIndex; 
                QifAccount acct(*(i->children()[AccountIndex]), *dateparser_);
                QifTransactionList lst(*(i->children()[TransactionListIndex]), *dateparser_);
                acct.transactions() = lst; 
                accounts_.collection().emplace_back(acct);
            }
        }
    }
}

std::ostream& qif::QifRoot::write(std::ostream& dest) const
{
    if(securities_.collection().size() > 0)   dest << "!Type:Security\n"   << securities_  ;
    if(transactions_.collection().size() > 0) dest <<                         transactions_;
    if(accounts_.collection().size() > 0)     dest <<                         accounts_    ;
    if(categories_.collection().size() > 0)   dest << "!Type:Cat\n"        << categories_  ;
    if(classes_.collection().size() > 0)      dest << "!Type:Class\n"      << classes_     ;
    return dest;
}

std::istream& qif::operator>>(std::istream& is, qif::QifRoot& m)
{
    qif::Context ctx;
    qif::Scanner scanner(std::cin,std::cerr);
    qif::Parser parser(scanner,ctx);

    int result = parser.parse();

    AST::tree_print(*ctx.AST);

    if(result == 0) m = std::move(qif::QifRoot(*ctx.AST,*m.dateparser_));

    return is;
}

void qif::QifRoot::add(const qif::QifRoot& src)
{
    accounts_.collection().insert(std::end(accounts_.collection()), 
        std::begin(src.accounts_.collection()), std::end(src.accounts_.collection()));

    securities_.collection().insert(std::end(securities_.collection()), 
        std::begin(src.securities_.collection()), std::end(src.securities_.collection()));

    categories_.collection().insert(std::end(categories_.collection()), 
        std::begin(src.categories_.collection()), std::end(src.categories_.collection()));

    classes_.collection().insert(std::end(classes_.collection()), 
        std::begin(src.classes_.collection()), std::end(src.classes_.collection()));

    transactions_.collection().insert(std::end(transactions_.collection()), 
        std::begin(src.transactions_.collection()), std::end(src.transactions_.collection()));

    if(transactions_.type() != src.transactions_.type()) {
        std::cerr << "Warning: merging transaction lists of types ["
            << transactions_.type() << "] and ["
            << src.transactions_.type() << "]\n";
    }
    if(transactions_.type() == "") transactions_.type() = src.transactions_.type();
}

qif::QifRoot qif::QifRoot::operator+(const qif::QifRoot& rQifRoot) const
{
    qif::QifRoot answer = qif::QifRoot(*this);
    answer.add(rQifRoot);
    return answer;
}

qif::QifRoot& qif::QifRoot::operator+=(const qif::QifRoot& rQifRoot)
{
    this->add(rQifRoot);
    return *this;
}

qif::QifRoot& qif::QifRoot::operator=(const qif::QifRoot& other)
{
    if(this != &other)
    {
        accounts_       = qif::QifList<qif::QifAccount>(other.accounts_)    ;
        securities_     = qif::QifList<qif::QifSecurity>(other.securities_) ;
        categories_     = qif::QifList<qif::QifCategory>(other.categories_) ;
        classes_        = qif::QifList<qif::QifClass>(other.classes_)       ;
        transactions_   = qif::QifTransactionList(other.transactions_)      ;
        dateparser_     = other.dateparser_->clone()                        ;
    }
    return *this;
}

qif::QifRoot& qif::QifRoot::operator=(qif::QifRoot&& other)
{
    accounts_       = std::move(other.accounts_)    ;
    securities_     = std::move(other.securities_)  ;
    categories_     = std::move(other.categories_)  ;
    classes_        = std::move(other.classes_)     ;
    transactions_   = std::move(other.transactions_);
    dateparser_     = std::move(other.dateparser_)  ;

    return *this;
}

#pragma endregion
