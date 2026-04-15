#ifndef CALENDAR_H
#define CALENDAR_H

#include "Date.h"

#include <string>

class Calendar {
public:
    Calendar();
    Calendar(Calendar const& cal);
    Calendar(Calendar&& cal) noexcept;
    ~Calendar();

    Calendar& operator=(Calendar const& cal);
    Calendar& operator=(Calendar&& cal) noexcept;

    void AddEntry(Date const& date, std::string const& text);
    void Clear();
    void PrintAllEntries(bool ascending = true) const;
    void swap(Calendar& other) noexcept;

private:
    struct CalendarEntry {
        Date date;
        std::string text;
        CalendarEntry* left;
        CalendarEntry* right;

        CalendarEntry(Date const& d, std::string const& t)
            : date(d), text(t), left(nullptr), right(nullptr) {
        }
    };

    CalendarEntry* mRoot;

    static CalendarEntry* CloneTree(CalendarEntry const* node);
    static void DeleteTree(CalendarEntry* node);
    static void AddOrReplaceEntry(CalendarEntry*& node, Date const& date, std::string const& text);
    static void PrintInOrder(CalendarEntry const* node);
    static void PrintReverseOrder(CalendarEntry const* node);
};

#endif
