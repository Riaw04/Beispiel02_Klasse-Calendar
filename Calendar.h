// ==========================
// File: Calendar.h
// Author: Niklas Riepl 
// Date: 13/04/2026
// Description: Header file for the Calendar class.
// ==========================

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Date.h"
#include <string>
#include <vector>

// Binary search tree-based calendar
class Calendar {
public:
    Calendar();
    Calendar(Calendar const& cal);              // deep copy
    Calendar(Calendar&& cal) noexcept;          // move
    ~Calendar();                                // frees tree

    Calendar& operator=(Calendar const& cal);   // deep copy
    Calendar& operator=(Calendar&& cal) noexcept;

    // Insert or replace entry with same date
    void AddEntry(Date const& date, std::string const& text);

    // Delete all entries
    void Clear();

    // Print entries in ascending / descending order
    void PrintAllEntries(bool ascending = true) const;
    std::vector<Date> FindByText(std::string const& searchText) const;
    void swap(Calendar& other) noexcept;

private:
    struct CalendarEntry {
        Date date;                  // key
        std::string text;          // value
        CalendarEntry* left;       // earlier dates
        CalendarEntry* right;      // later dates

        CalendarEntry(Date const& d, std::string const& t)
            : date(d), text(t), left(nullptr), right(nullptr) {
        }
    };

    CalendarEntry* mRoot;          // root of BST

    // Tree utilities
    static CalendarEntry* CloneTree(CalendarEntry const* node);
    static void DeleteTree(CalendarEntry* node);
    static void AddOrReplaceEntry(CalendarEntry*& node, Date const& date, std::string const& text);

    // Traversal helpers
    static void PrintInOrder(CalendarEntry const* node);
    static void PrintReverseOrder(CalendarEntry const* node);
    static void CollectByText(CalendarEntry const* node, std::string const& searchLower, std::vector<Date>& result);
    static std::string ToLower(std::string const& input);
};

#endif