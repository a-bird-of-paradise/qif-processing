#include "date-parsers.hpp"
#include <sstream>

date::year_month_day qif::DateParser_dmy_hyphen::operator()(const std::string& Source) const {

    std::stringstream ss(Source);
    std::string fmt = "%d-%m-%Cy";
    date::sys_time<std::chrono::microseconds> timestamp;
    ss >> date::parse(fmt,timestamp);
    return date::year_month_day(date::floor<date::days>(timestamp));
}

date::year_month_day qif::DateParser_ymd_hyphen::operator()(const std::string& Source) const {

    std::stringstream ss(Source);
    std::string fmt = "%Y-%m-%d";
    date::sys_time<std::chrono::microseconds> timestamp;
    ss >> date::parse(fmt,timestamp);
    return date::year_month_day(date::floor<date::days>(timestamp));
}

date::year_month_day qif::DateParser_dmy_slash::operator()(const std::string& Source) const {

    std::stringstream ss(Source);
    std::string fmt = "%d/%m/%Y";
    date::sys_time<std::chrono::microseconds> timestamp;
    ss >> date::parse(fmt,timestamp);
    return date::year_month_day(date::floor<date::days>(timestamp));
}

date::year_month_day qif::DateParser_ymd_slash::operator()(const std::string& Source) const {

    std::stringstream ss(Source);
    std::string fmt = "%Y/%m/%d";
    date::sys_time<std::chrono::microseconds> timestamp;
    ss >> date::parse(fmt,timestamp);
    return date::year_month_day(date::floor<date::days>(timestamp));
}

std::unique_ptr<qif::DateParser> qif::DateParser_dmy_hyphen::clone() const {
    return std::make_unique<qif::DateParser_dmy_hyphen>(*this);
}
std::unique_ptr<qif::DateParser> qif::DateParser_ymd_hyphen::clone() const {
    return std::make_unique<qif::DateParser_ymd_hyphen>(*this);
}
std::unique_ptr<qif::DateParser> qif::DateParser_dmy_slash::clone() const {
    return std::make_unique<qif::DateParser_dmy_slash>(*this);
}
std::unique_ptr<qif::DateParser> qif::DateParser_ymd_slash::clone() const {
    return std::make_unique<qif::DateParser_ymd_slash>(*this);
}