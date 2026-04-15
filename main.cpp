#include "Calendar.h"

#include <iostream>
#include <stdexcept>

int main() {
    try {
        std::cout << "=== Date Tests ===\n";

        // Construct a few valid sample dates and verify comparison behavior.
        Date d1(1, 1, 2025);
        Date d2(2, 1, 2025);
        Date d3(2, 2, 2024);

        std::cout << "d1 = " << d1 << '\n';
        std::cout << "d2 = " << d2 << '\n';
        std::cout << "d3 = " << d3 << '\n';

        std::cout << "Compare d1 with d2: " << d1.Compare(d2) << " (expected -1)\n";
        std::cout << "Compare d2 with d1: " << d2.Compare(d1) << " (expected +1)\n";
        std::cout << "Compare d2 with d2: " << d2.Compare(d2) << " (expected 0)\n";

        // Demonstrate validation: this invalid date must throw an exception.
        try {
            Date invalid(31, 2, 2025);
            std::cout << "ERROR: invalid date accepted: " << invalid << '\n';
        } catch (std::invalid_argument const&) {
            std::cout << "Invalid date check passed.\n";
        }

        std::cout << "\n=== Calendar Tests ===\n";

        // Fill a calendar and print it in both traversal orders.
        Calendar cal;
        cal.AddEntry(Date(15, 5, 2026), "Arzttermin");
        cal.AddEntry(Date(1, 5, 2026), "Maifeiertag");
        cal.AddEntry(Date(24, 12, 2025), "Heiligabend");
        cal.AddEntry(Date(1, 1, 2026), "Neujahr");

        std::cout << "\nAscending:\n";
        cal.PrintAllEntries(true);

        std::cout << "\nDescending:\n";
        cal.PrintAllEntries(false);

        // Inserting the same date again replaces the old text.
        std::cout << "\nReplace existing entry (01.01.2026)...\n";
        cal.AddEntry(Date(1, 1, 2026), "Neujahr (Brunch mit Familie)");
        cal.PrintAllEntries(true);

        // Verify deep copy semantics of the copy constructor.
        std::cout << "\nCopy constructor test...\n";
        Calendar copied(cal);
        copied.AddEntry(Date(31, 12, 2026), "Silvesterparty");

        std::cout << "Original calendar:\n";
        cal.PrintAllEntries(true);

        std::cout << "Copied calendar:\n";
        copied.PrintAllEntries(true);

        // Verify assignment performs a full replacement of existing content.
        std::cout << "\nAssignment operator test...\n";
        Calendar assigned;
        assigned.AddEntry(Date(10, 10, 2030), "Should disappear");
        assigned = cal;
        assigned.PrintAllEntries(true);

        // Clear all entries and confirm the calendar is empty.
        std::cout << "\nClear test...\n";
        assigned.Clear();
        std::cout << "After Clear (should be empty):\n";
        assigned.PrintAllEntries(true);

        return 0;
    } catch (std::exception const& ex) {
        std::cerr << "Unhandled exception: " << ex.what() << '\n';
    } catch (...) {
        std::cerr << "Unhandled unknown exception.\n";
    }

    return 1;
}
