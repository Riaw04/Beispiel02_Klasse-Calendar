#ifndef DATE_H
#define DATE_H

#include <cstddef>
#include <iosfwd>

class Date {
public:
    Date(size_t day, size_t month, size_t year);

    size_t getDay() const;
    size_t getMonth() const;
    size_t getYear() const;

    int Compare(Date const& d) const;

    void Print(std::ostream& out) const;

private:
    static bool IsLeapYear(size_t year);
    static size_t DaysInMonth(size_t month, size_t year);
    static bool IsValidDate(size_t day, size_t month, size_t year);

    size_t mDay;
    size_t mMonth;
    size_t mYear;
};

std::ostream& operator<<(std::ostream& out, Date const& date);

#endif
