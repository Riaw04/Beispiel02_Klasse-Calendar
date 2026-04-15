#ifndef DATE_H
#define DATE_H

#include <cstddef>
#include <iosfwd>

class Date {
public:
    Date(size_t day, size_t month, size_t year);

    [[nodiscard]] size_t getDay() const noexcept;
    [[nodiscard]] size_t getMonth() const noexcept;
    [[nodiscard]] size_t getYear() const noexcept;

    [[nodiscard]] int Compare(Date const& d) const noexcept;

    void Print(std::ostream& out) const;

private:
    [[nodiscard]] static bool IsLeapYear(size_t year) noexcept;
    [[nodiscard]] static size_t DaysInMonth(size_t month, size_t year) noexcept;
    [[nodiscard]] static bool IsValidDate(size_t day, size_t month, size_t year) noexcept;

    size_t mDay;
    size_t mMonth;
    size_t mYear;
};

std::ostream& operator<<(std::ostream& out, Date const& date);

#endif
