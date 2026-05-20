#include "Calendar.h"

#include <iostream>
#include <cctype>
#include <utility>

Calendar::Calendar() : mRoot(nullptr) {
}

Calendar::Calendar(Calendar const& cal) : mRoot(CloneTree(cal.mRoot)) {
}

Calendar::Calendar(Calendar&& cal) noexcept : mRoot(cal.mRoot) {
    cal.mRoot = nullptr;
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

Calendar& Calendar::operator=(Calendar&& cal) noexcept {
    if (this == &cal) {
        return *this;
    }

    Clear();
    mRoot = cal.mRoot;
    cal.mRoot = nullptr;
    return *this;
}

void Calendar::AddEntry(Date const& date, std::string const& text) {
    AddOrReplaceEntry(mRoot, date, text);
}

void Calendar::Clear() {
    DeleteTree(mRoot);
    mRoot = nullptr;
}

void Calendar::PrintAllEntries(bool ascending) const {
    if (ascending) {
        PrintInOrder(mRoot);
    } else {
        PrintReverseOrder(mRoot);
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
    std::swap(mRoot, other.mRoot);
}

Calendar::CalendarEntry* Calendar::CloneTree(CalendarEntry const* node) {
    if (node == nullptr) {
        return nullptr;
    }

    // Build a deep copy of the subtree. If cloning one of the children fails,
    // we clean up the already allocated memory and rethrow the exception.
    CalendarEntry* cloned = new CalendarEntry(node->date, node->text);
    try {
        cloned->left = CloneTree(node->left);
        cloned->right = CloneTree(node->right);
        return cloned;
    } catch (...) {
        DeleteTree(cloned);
        throw;
    }
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
        // Create a new node when we reach an empty position in the BST.
        node = new CalendarEntry(date, text);
        return;
    }

    const int cmp = date.Compare(node->date);
    if (cmp == 0) {
        // Same date means we replace the existing description.
        node->text = text;
    } else if (cmp < 0) {
        // Earlier dates are stored in the left subtree.
        AddOrReplaceEntry(node->left, date, text);
    } else {
        // Later dates are stored in the right subtree.
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
