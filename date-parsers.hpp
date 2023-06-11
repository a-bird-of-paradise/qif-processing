#pragma once
#include<string>
#include "date.h"

namespace qif
{

    class DateParser
    {
    public:
        virtual date::year_month_day operator()(const std::string& Source) const = 0;
        virtual ~DateParser() = default;
        virtual std::unique_ptr<DateParser> clone() const = 0;
    };

    class DateParser_dmy_hyphen : public DateParser
    {
    public:
        virtual date::year_month_day operator()(const std::string& Source) const override;
        virtual std::unique_ptr<DateParser> clone() const override;
    };

    class DateParser_ymd_hyphen : public DateParser
    {
    public:
        virtual date::year_month_day operator()(const std::string& Source) const override;
        virtual std::unique_ptr<DateParser> clone() const override;
    };

    class DateParser_dmy_slash : public DateParser
    {
    public:
        virtual date::year_month_day operator()(const std::string& Source) const override;
        virtual std::unique_ptr<DateParser> clone() const override;
    };

    class DateParser_ymd_slash : public DateParser
    {
    public:
        virtual date::year_month_day operator()(const std::string& Source) const override;
        virtual std::unique_ptr<DateParser> clone() const override;
    };
}