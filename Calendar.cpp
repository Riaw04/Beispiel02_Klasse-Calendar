#include "Calendar.h"

#include <iostream>

Calendar::Calendar() : mRoot(nullptr) {
}

Calendar::Calendar(Calendar const& cal) : mRoot(CloneTree(cal.mRoot)) {
}

Calendar::~Calendar() {
    Clear();
}

Calendar& Calendar::operator=(Calendar const& cal) {
    if (this == &cal) {
        return *this;
    }

    CalendarEntry* newRoot = CloneTree(cal.mRoot);
    DeleteTree(mRoot);
    mRoot = newRoot;
    return *this;
}

void Calendar::AddEntry(Date const& date, std::string const& text) {
    AddOrReplaceEntry(mRoot, date, text);
}

void Calendar::Clear() {
    DeleteTree(mRoot);
    mRoot = nullptr;
}

void Calendar::PrintAllEntries(bool const ascending) const {
    if (ascending) {
        PrintInOrder(mRoot);
    } else {
        PrintReverseOrder(mRoot);
    }
}

Calendar::CalendarEntry* Calendar::CloneTree(CalendarEntry const* node) {
    if (node == nullptr) {
        return nullptr;
    }

    CalendarEntry* cloned = new CalendarEntry(node->date, node->text);
    cloned->left = CloneTree(node->left);
    cloned->right = CloneTree(node->right);
    return cloned;
}

void Calendar::DeleteTree(CalendarEntry* node) {
    if (node == nullptr) {
        return;
    }

    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

void Calendar::AddOrReplaceEntry(CalendarEntry*& node, Date const& date, std::string const& text) {
    if (node == nullptr) {
        node = new CalendarEntry(date, text);
        return;
    }

    const int cmp = date.Compare(node->date);
    if (cmp == 0) {
        node->text = text;
    } else if (cmp < 0) {
        AddOrReplaceEntry(node->left, date, text);
    } else {
        AddOrReplaceEntry(node->right, date, text);
    }
}

void Calendar::PrintInOrder(CalendarEntry const* node) {
    if (node == nullptr) {
        return;
    }

    PrintInOrder(node->left);
    std::cout << node->date << " : " << node->text << '\n';
    PrintInOrder(node->right);
}

void Calendar::PrintReverseOrder(CalendarEntry const* node) {
    if (node == nullptr) {
        return;
    }

    PrintReverseOrder(node->right);
    std::cout << node->date << " : " << node->text << '\n';
    PrintReverseOrder(node->left);
}
