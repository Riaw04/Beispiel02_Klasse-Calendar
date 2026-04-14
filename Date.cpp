#include "Date.h"

#include <iomanip>
#include <ostream>
#include <stdexcept>

Date::Date(size_t day, size_t month, size_t year)
    : mDay(day), mMonth(month), mYear(year) {
    if (!IsValidDate(day, month, year)) {
        throw std::invalid_argument("invalid date");
    }
}

size_t Date::getDay() const {
    return mDay;
}

size_t Date::getMonth() const {
    return mMonth;
}

size_t Date::getYear() const {
    return mYear;
}

int Date::Compare(Date const& d) const {
    if (mYear < d.mYear) {
        return -1;
    }
    if (mYear > d.mYear) {
        return +1;
    }

    if (mMonth < d.mMonth) {
        return -1;
    }
    if (mMonth > d.mMonth) {
        return +1;
    }

    if (mDay < d.mDay) {
        return -1;
    }
    if (mDay > d.mDay) {
        return +1;
    }

    return 0;
}

void Date::Print(std::ostream& out) const {
    out << std::setw(2) << std::setfill('0') << mDay << "."
        << std::setw(2) << std::setfill('0') << mMonth << "."
        << mYear;
}

bool Date::IsLeapYear(size_t year) {
    return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

size_t Date::DaysInMonth(size_t month, size_t year) {
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return IsLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

bool Date::IsValidDate(size_t day, size_t month, size_t year) {
    if (year == 0 || month < 1 || month > 12) {
        return false;
    }
    const size_t maxDays = DaysInMonth(month, year);
    return day >= 1 && day <= maxDays;
}

std::ostream& operator<<(std::ostream& out, Date const& date) {
    date.Print(out);
    return out;
}
