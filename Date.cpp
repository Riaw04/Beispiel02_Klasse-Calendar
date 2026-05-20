// ==========================
// File: Date.cpp
// Author: Niklas Riepl 
// Date: 13/04/2026
// Description: Implementation of the Date class.
// ==========================

#include "Date.h"

#include <iomanip>
#include <ostream>
#include <stdexcept>

Date::Date(std::size_t day, std::size_t month, std::size_t year)
    : mDay(day), mMonth(month), mYear(year) {
    // Reject impossible calendar values at construction time.
    if (!IsValidDate(day, month, year)) {
        throw std::invalid_argument("Invalid date values provided");
    }
}

std::size_t Date::getDay() const noexcept {
    return mDay;
}

std::size_t Date::getMonth() const noexcept {
    return mMonth;
}

std::size_t Date::getYear() const noexcept {
    return mYear;
}

int Date::Compare(Date const& d) const noexcept {
    // Compare from the largest unit (year) down to the smallest (day).
    if (mYear < d.mYear) {
        return -1;
    }
    if (mYear > d.mYear) {
        return 1;
    }

    if (mMonth < d.mMonth) {
        return -1;
    }
    if (mMonth > d.mMonth) {
        return 1;
    }

    if (mDay < d.mDay) {
        return -1;
    }
    if (mDay > d.mDay) {
        return 1;
    }

    return 0;
}

void Date::Print(std::ostream& out) const {
    // Preserve and restore stream state so callers are not affected.
    char const previousFill = out.fill();

    // Always print with two digits for day and month.
    out << std::setfill('0')
        << std::setw(2) << mDay << '.'
        << std::setw(2) << mMonth << '.'
        << mYear;

    out.fill(previousFill);
}

bool Date::IsLeapYear(std::size_t year) noexcept {
    // Leap year rules of the Gregorian calendar.
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

std::size_t Date::DaysInMonth(std::size_t month, std::size_t year) noexcept {
    // Return the valid number of days for each month.
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

bool Date::IsValidDate(std::size_t day, std::size_t month, std::size_t year) noexcept {
    // Restrict the year to positive values in the Gregorian calendar.
    if (year < 1) {
        return false;
    }

    // Month must be in the closed interval [1, 12].
    if (month < 1 || month > 12) {
        return false;
    }

    // Day value must be at least 1.
    if (day < 1) {
        return false;
    }

    return day <= DaysInMonth(month, year);
}

std::ostream& operator<<(std::ostream& out, Date const& date) {
    date.Print(out);
    return out;
}
