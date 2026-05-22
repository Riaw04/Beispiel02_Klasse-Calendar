// ==========================
// File: main.cpp
// Author: Niklas Riepl
// Date: 13/04/2026
// Description: Main program for testing the Calendar class.
// ==========================

#include "Calendar.h"

#include <cassert>
#include <iostream>
#include <vector>

static void PrintFound(std::vector<Date> const& dates) {
    for (Date const& date : dates) {
        std::cout << "  -> " << date << '\n';
    }
}

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
        assert(d1.Compare(d2) < 0);
        assert(d2.Compare(d1) > 0);
        assert(d2.Compare(d2) == 0);

        // Demonstrate validation: this invalid date must throw an exception.
        try {
            Date invalid(31, 2, 2025);
            std::cout << "ERROR: invalid date accepted: " << invalid << '\n';
        }
        catch (std::invalid_argument const&) {
            std::cout << "Invalid date check passed.\n";
        }

        Calendar cal;

        std::cout << "\n[1] Empty calendar:\n";
        cal.PrintAllEntries(true);

        std::cout << "\n[2] Basic insertion (unordered input -> sorted output):\n";
        cal.AddEntry(Date(15, 5, 2026), "A");
        cal.AddEntry(Date(1, 5, 2026), "B");
        cal.AddEntry(Date(24, 12, 2025), "C");
        cal.AddEntry(Date(1, 1, 2026), "D");
        cal.PrintAllEntries(true);

        std::cout << "\n[3] Descending order view:\n";
        cal.PrintAllEntries(false);

        std::cout << "\n[4] Replace existing entry (same date update):\n";
        cal.AddEntry(Date(1, 1, 2026), "Updated");
        cal.PrintAllEntries(true);

        std::cout << "\n[5] Multiple overwrites on same date:\n";
        cal.AddEntry(Date(10, 10, 2026), "First");
        cal.AddEntry(Date(10, 10, 2026), "Final");
        cal.PrintAllEntries(true);

        std::cout << "\n[6] Boundary dates test:\n";
        cal.AddEntry(Date(31, 12, 2026), "Year End");
        cal.AddEntry(Date(1, 1, 2025), "Year Start");
        cal.PrintAllEntries(true);

        std::cout << "\nFindByText tests...\n";
        std::vector<Date> match1 = cal.FindByText("jahr");
        std::cout << "Suche nach 'jahr' (case-insensitive), Treffer: " << match1.size() << '\n';
        PrintFound(match1);

        std::vector<Date> match2 = cal.FindByText("ARZT");
        std::cout << "Suche nach 'ARZT', Treffer: " << match2.size() << '\n';
        PrintFound(match2);

        std::vector<Date> match3 = cal.FindByText("nicht-vorhanden");
        std::cout << "Suche nach 'nicht-vorhanden', Treffer: " << match3.size() << '\n';
        assert(match3.empty());

        std::vector<Date> match4 = cal.FindByText("");
        std::cout << "Suche nach leerem Text, Treffer: " << match4.size() << '\n';
        assert(match4.empty());

        std::cout << "\nCopy constructor test...\n";
        Calendar copied(cal);
        copied.AddEntry(Date(31, 12, 2026), "Silvesterparty");

        std::cout << "Original calendar:\n";
        cal.PrintAllEntries(true);

        std::cout << "Copied calendar:\n";
        copied.PrintAllEntries(true);

        std::cout << "\n[8] Assignment operator test:\n";
        Calendar assigned;
        assigned.AddEntry(Date(1, 1, 2030), "Temporary data");

        assigned = cal;
        assigned.PrintAllEntries(true);

        std::cout << "\nMove constructor test...\n";
        Calendar movedConstructed(std::move(assigned));
        std::cout << "Moved-to calendar:\n";
        movedConstructed.PrintAllEntries(true);
        std::cout << "Moved-from calendar (should be empty):\n";
        assigned.PrintAllEntries(true);

        std::cout << "\nMove assignment and swap test...\n";
        Calendar moveAssigned;
        moveAssigned = std::move(movedConstructed);
        std::cout << "After move assignment:\n";
        moveAssigned.PrintAllEntries(true);

        Calendar other;
        other.AddEntry(Date(11, 11, 2040), "Eintrag in anderem Kalender");
        std::cout << "Before swap - moveAssigned:\n";
        moveAssigned.PrintAllEntries(true);
        std::cout << "Before swap - other:\n";
        other.PrintAllEntries(true);
        moveAssigned.swap(other);
        std::cout << "After swap - moveAssigned:\n";
        moveAssigned.PrintAllEntries(true);
        std::cout << "After swap - other:\n";
        other.PrintAllEntries(true);

        std::cout << "\nClear test...\n";
        other.Clear();
        std::cout << "After Clear (should be empty):\n";
        other.PrintAllEntries(true);

        std::cout << "\n[11] Insert after clear:\n";
        assigned.AddEntry(Date(5, 5, 2027), "New Event");
        assigned.PrintAllEntries(true);

        std::cout << "\n[12] Double clear test:\n";
        assigned.Clear();
        assigned.Clear();
        assigned.PrintAllEntries(true);

        std::cout << "\n=== All tests completed successfully ===\n";
        std::cout << "\nDone.\n";
        return 0;
    }
    catch (std::exception const& ex) {
        std::cerr << "Unhandled exception: " << ex.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unhandled unknown exception.\n";
    }

    return 1;
}
