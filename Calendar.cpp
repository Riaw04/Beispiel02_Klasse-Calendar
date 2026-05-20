// ==========================
// File: Calendar.cpp
// Author: Niklas Riepl 
// Date: 13/04/2026
// Description: Implementation of the Calendar class.
// ==========================

#include "Calendar.h"

#include <iostream>
#include <cctype>
#include <utility>

Calendar::Calendar() : mRoot(nullptr) {} // Default constructor initializes an empty calendar.
Calendar::Calendar(Calendar const& cal) : mRoot(CloneTree(cal.mRoot)) {} // Copy constructor creates a deep copy of the calendar.
Calendar::Calendar(Calendar&& cal) noexcept : mRoot(cal.mRoot) { cal.mRoot = nullptr; } // Move constructor takes ownership of the resources from the source calendar and leaves it in a valid but empty state.
Calendar::~Calendar() { Clear(); } // Destructor cleans up all allocated memory by deleting the entire tree.

Calendar& Calendar::operator=(Calendar const& cal) {
    if (this == &cal) {
        return *this; // handle self-assignment
    }

    CalendarEntry* newRoot = CloneTree(cal.mRoot); // deep copy source tree
    DeleteTree(mRoot); // free current data
    mRoot = newRoot;   // assign new copy
    return *this;
}

Calendar& Calendar::operator=(Calendar&& cal) noexcept {
    if (this == &cal) {
        return *this; // handle self-move (rare but safe)
    }

    Clear();              // release current resources
    mRoot = cal.mRoot;    // steal pointer
    cal.mRoot = nullptr;  // leave source in valid empty state
    return *this;
}

void Calendar::AddEntry(Date const& date, std::string const& text) {
    AddOrReplaceEntry(mRoot, date, text); // insert or update entry
}

void Calendar::Clear() {
    DeleteTree(mRoot); // delete entire tree
    mRoot = nullptr;   // reset root
}

void Calendar::PrintAllEntries(bool ascending) const {
    if (ascending) {
        PrintInOrder(mRoot);        // smallest ? largest
    }
    else {
        PrintReverseOrder(mRoot);   // largest ? smallest
    }
}

std::vector<Date> Calendar::FindByText(std::string const& searchText) const {
    std::vector<Date> result;
    const std::string searchLower = ToLower(searchText);
    if (searchLower.empty()) {
        return result;
    }

    CollectByText(mRoot, searchLower, result);
    return result;
}

void Calendar::swap(Calendar& other) noexcept {
    std::swap(mRoot, other.mRoot); // constant-time swap
}

Calendar::CalendarEntry* Calendar::CloneTree(CalendarEntry const* node) {
    if (node == nullptr) {
        return nullptr; // base case
    }

    // Create copy of current node
    CalendarEntry* cloned = new CalendarEntry(node->date, node->text);
    try {
        cloned->left = CloneTree(node->left);   // copy left subtree
        cloned->right = CloneTree(node->right); // copy right subtree
        return cloned;
    }
    catch (...) {
        DeleteTree(cloned); // clean up partial copy on failure
        throw;
    }
}

void Calendar::DeleteTree(CalendarEntry* node) {
    if (node == nullptr) {
        return; // base case
    }

    DeleteTree(node->left);   // delete left subtree
    DeleteTree(node->right);  // delete right subtree
    delete node;              // delete current node
}

void Calendar::AddOrReplaceEntry(CalendarEntry*& node, Date const& date, std::string const& text) {
    if (node == nullptr) {
        node = new CalendarEntry(date, text); // insert new node
        return;
    }

    const int cmp = date.Compare(node->date);
    if (cmp == 0) {
        node->text = text; // replace existing entry
    }
    else if (cmp < 0) {
        AddOrReplaceEntry(node->left, date, text); // go left
    }
    else {
        AddOrReplaceEntry(node->right, date, text); // go right
    }
}

void Calendar::PrintInOrder(CalendarEntry const* node) {
    if (node == nullptr) {
        return; // base case
    }

    PrintInOrder(node->left); // left subtree
    std::cout << node->date << " : " << node->text << '\n'; // visit node
    PrintInOrder(node->right); // right subtree
}

void Calendar::PrintReverseOrder(CalendarEntry const* node) {
    if (node == nullptr) {
        return; // base case
    }

    PrintReverseOrder(node->right);
    std::cout << node->date << " : " << node->text << '\n';
    PrintReverseOrder(node->left);
}

void Calendar::CollectByText(CalendarEntry const* node, std::string const& searchLower, std::vector<Date>& result) {
    if (node == nullptr) {
        return;
    }

    CollectByText(node->left, searchLower, result);
    if (ToLower(node->text).find(searchLower) != std::string::npos) {
        result.push_back(node->date);
    }
    CollectByText(node->right, searchLower, result);
}

std::string Calendar::ToLower(std::string const& input) {
    std::string lowered;
    lowered.reserve(input.size());
    for (char ch : input) {
        lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return lowered;
}
