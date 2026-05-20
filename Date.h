// ==========================
// File: Date.h
// Author: Niklas Riepl 
// Date: 13/04/2026
// Description: Header file for the Date class.
// ==========================

#pragma once

#include <cstddef>
#include <iosfwd>

class Date {
public:
    // Creates a date object.
    // Throws std::invalid_argument when day/month/year do not form a valid calendar date.
    Date(std::size_t day, std::size_t month, std::size_t year);

    // Accessor functions.
    [[nodiscard]] std::size_t getDay() const noexcept;
    [[nodiscard]] std::size_t getMonth() const noexcept;
    [[nodiscard]] std::size_t getYear() const noexcept;

    // Returns -1 if this date is earlier than d, +1 if later, and 0 if equal.
    [[nodiscard]] int Compare(Date const& d) const noexcept;

    // Writes date as dd.mm.yyyy into an output stream.
    void Print(std::ostream& out) const;

private:
    [[nodiscard]] static bool IsLeapYear(std::size_t year) noexcept; // Leap year rules of the Gregorian calendar.
    [[nodiscard]] static std::size_t DaysInMonth(std::size_t month, std::size_t year) noexcept; // Return the valid number of days for each month.
    [[nodiscard]] static bool IsValidDate(std::size_t day, std::size_t month, std::size_t year) noexcept; // Validates the date values.

    std::size_t mDay;
    std::size_t mMonth;
    std::size_t mYear;
};

// Stream output helper for Date.
std::ostream& operator<<(std::ostream& out, Date const& date);
