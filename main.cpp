// ==========================
// File: main.cpp
// Author: Niklas Riepl 
// Date: 13/04/2026
// Description: Main program for testing and benchmarking the Calendar class.
// ==========================

#include "Calendar.h"

#include <chrono>
#include <iostream>
#include <sstream>

using Clock = std::chrono::high_resolution_clock;

// ==========================
// Benchmarks
// ==========================

#include <numeric>

volatile int sink = 0;

template <typename Func>
long long Measure(Func f, int repeats = 5) {
    using namespace std::chrono;

    long long total = 0;

    for (int i = 0; i < repeats; ++i) {
        auto start = high_resolution_clock::now();
        f();
        auto end = high_resolution_clock::now();

        total += duration_cast<microseconds>(end - start).count();
    }

    return total / repeats;
}

// Insert n elements
void BenchmarkInsert(int n) {
    auto time = Measure([&]() {
        Calendar cal;
        for (int i = 1; i <= n; ++i) {
            cal.AddEntry(Date((i % 28) + 1, (i % 12) + 1, 2026), "X");
        }
        sink += n;
        });

    std::cout << "Insert " << n << ": " << time << " us\n";
}

// Insert in reverse order (tests worst-case behavior)
void BenchmarkReverseInsert(int n) {
    auto time = Measure([&]() {
        Calendar cal;
        for (int i = n; i >= 1; --i) {
            cal.AddEntry(Date((i % 28) + 1, (i % 12) + 1, 2026), "X");
        }
        sink += n;
        });

    std::cout << "Reverse insert " << n << ": " << time << " us\n";
}

// Traverse and format all entries
void BenchmarkPrint(int n) {
    Calendar cal;

    for (int i = 1; i <= n; ++i) {
        cal.AddEntry(Date((i % 28) + 1, (i % 12) + 1, 2026), "X");
    }

    auto time = Measure([&]() {
        std::ostringstream out;
        cal.PrintAllEntries(true);
        sink += out.str().size();
        });

    std::cout << "Print " << n << ": " << time << " us\n";
}

// Copy constructor (should scale with n)
void BenchmarkCopy(int n) {
    Calendar cal;

    for (int i = 1; i <= n; ++i) {
        cal.AddEntry(Date((i % 28) + 1, (i % 12) + 1, 2026), "X");
    }

    auto time = Measure([&]() {
        Calendar copy(cal);
        copy.PrintAllEntries(true);
        sink += n;
        });

    std::cout << "Copy " << n << ": " << time << " us\n";
}

// Assignment operator (replace existing data)
void BenchmarkAssignment(int n) {
    Calendar cal;

    for (int i = 1; i <= n; ++i) {
        cal.AddEntry(Date((i % 28) + 1, (i % 12) + 1, 2026), "X");
    }

    auto time = Measure([&]() {
        Calendar other;
        other = cal;
        other.PrintAllEntries(true);
        sink += n;
        });

    std::cout << "Assign " << n << ": " << time << " us\n";
}

// ==========================
// Main
// ==========================

int main() {
    try {

        Calendar cal;

        // ------------------------------------------------------------
        // 1. Empty calendar test (should handle gracefully)
        // ------------------------------------------------------------
        std::cout << "\n[1] Empty calendar:\n";
        cal.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 2. Basic insertion + automatic ordering test
        // ------------------------------------------------------------
        std::cout << "\n[2] Basic insertion (unordered input -> sorted output):\n";
        cal.AddEntry(Date(15, 5, 2026), "A");
        cal.AddEntry(Date(1, 5, 2026), "B");
        cal.AddEntry(Date(24, 12, 2025), "C");
        cal.AddEntry(Date(1, 1, 2026), "D");
        cal.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 3. Reverse order printing test
        // ------------------------------------------------------------
        std::cout << "\n[3] Descending order view:\n";
        cal.PrintAllEntries(false);

        // ------------------------------------------------------------
        // 4. Update existing entry (same date overwrite behavior)
        // ------------------------------------------------------------
        std::cout << "\n[4] Replace existing entry (same date update):\n";
        cal.AddEntry(Date(1, 1, 2026), "Updated");
        cal.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 5. Overwrite same date multiple times
        // ------------------------------------------------------------
        std::cout << "\n[5] Multiple overwrites on same date:\n";
        cal.AddEntry(Date(10, 10, 2026), "First");
        cal.AddEntry(Date(10, 10, 2026), "Final");
        cal.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 6. Boundary date test (end of year / leap year context)
        // ------------------------------------------------------------
        std::cout << "\n[6] Boundary dates test:\n";
        cal.AddEntry(Date(31, 12, 2026), "Year End");
        cal.AddEntry(Date(1, 1, 2025), "Year Start");
        cal.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 7. Copy constructor deep copy test
        // ------------------------------------------------------------
        std::cout << "\n[7] Copy constructor test (independence check):\n";
        Calendar copy(cal);
        copy.AddEntry(Date(30, 6, 2027), "Only in copy");

        std::cout << "Original calendar:\n";
        cal.PrintAllEntries(true);

        std::cout << "Copied calendar:\n";
        copy.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 8. Assignment operator test
        // ------------------------------------------------------------
        std::cout << "\n[8] Assignment operator test:\n";
        Calendar assigned;
        assigned.AddEntry(Date(1, 1, 2030), "Temporary data");

        assigned = cal;
        assigned.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 9. Self-assignment safety test
        // ------------------------------------------------------------
        std::cout << "\n[9] Self-assignment test:\n";
        assigned = assigned;
        assigned.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 10. Clear functionality test
        // ------------------------------------------------------------
        std::cout << "\n[10] Clear calendar:\n";
        assigned.Clear();
        assigned.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 11. Reuse after clear
        // ------------------------------------------------------------
        std::cout << "\n[11] Insert after clear:\n";
        assigned.AddEntry(Date(5, 5, 2027), "New Event");
        assigned.PrintAllEntries(true);

        // ------------------------------------------------------------
        // 12. Double clear safety test (should not crash)
        // ------------------------------------------------------------
        std::cout << "\n[12] Double clear test:\n";
        assigned.Clear();
        assigned.Clear();
        assigned.PrintAllEntries(true);

        std::cout << "\n=== All tests completed successfully ===\n";

        // ==========================
        // Benchmarks
        // ==========================

        /*std::cout << "\n=== Benchmarks ===\n";

        int sizes[] = { 100, 1000, 5000 };

        for (int n : sizes) {
            std::cout << "\n--- n = " << n << " ---\n";
            BenchmarkInsert(n);
            BenchmarkReverseInsert(n);
            BenchmarkPrint(n);
            BenchmarkCopy(n);
            BenchmarkAssignment(n);
        }*/

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