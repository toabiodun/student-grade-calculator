#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <string>
#include "Person.h"

using namespace std;

// --------------------------------------------------
// Helper: faster console I/O but still flush before input
// --------------------------------------------------
void setupConsole() {
    ios::sync_with_stdio(false);
    cin.tie(&cout);   // <— IMPORTANT: ensures menu text appears before cin
}

// --------------------------------------------------
// Random score generator used by all containers
// --------------------------------------------------
void fillRandomScores(Person& p, int index) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(1, 10);

    // Example names: Name1 Surname1, Name2 Surname2, ...
    p.setFirstName("Name" + to_string(index + 1));
    p.setSurname("Surname" + to_string(index + 1));

    vector<int> hw(15);
    for (int& x : hw) {
        x = dist(gen);
    }

    for (int x : hw) {
        p.addHomeworkScore(x);
    }
    p.setExamScore(dist(gen));
    p.calculateFinalGradeAverage(); // we use average for splitting
}

// --------------------------------------------------
// Sorting helpers for different containers
// --------------------------------------------------
void sortStudents(list<Person>& students) {
    students.sort();              // list has its own sort()
}

template <typename Seq>
void sortStudents(Seq& students) {
    sort(students.begin(), students.end());  // vector, deque, etc.
}

// --------------------------------------------------
// Writing helpers
// --------------------------------------------------
template <typename Container>
void writeStudentsToFile(const Container& students, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        throw runtime_error("Could not open file for writing: " + filename);
    }

    out << "FirstName Surname Final\n";
    out << "-----------------------------\n";

    for (const auto& s : students) {
        out << s.getFirstName() << " "
            << s.getSurname() << " "
            << s.getFinalGrade() << "\n";
    }
}

// --------------------------------------------------
// Generic performance test for any container type
// --------------------------------------------------
template <typename Container>
void runContainerTest(const string& label, const string& suffix) {
    cout << "\n=== " << label << " PERFORMANCE TEST ===\n";

    vector<size_t> sizes = {1000, 10000, 100000, 1000000};

    for (size_t n : sizes) {
        cout << "\n--- Testing with " << n << " students ---\n";

        // 1) Generate students
        auto t_start_gen = chrono::high_resolution_clock::now();

        Container students;

        for (size_t i = 0; i < n; ++i) {
            Person p;
            fillRandomScores(p, static_cast<int>(i));
            students.push_back(p);
        }

        auto t_end_gen = chrono::high_resolution_clock::now();

        // 2) Sort students
        auto t_start_sort = chrono::high_resolution_clock::now();
        sortStudents(students);
        auto t_end_sort = chrono::high_resolution_clock::now();

        // 3) Split into passed / failed
        auto t_start_split = chrono::high_resolution_clock::now();

        Container passed;
        Container failed;

        for (const auto& s : students) {
            if (s.getFinalGrade() >= 5.0) {
                passed.push_back(s);
            } else {
                failed.push_back(s);
            }
        }

        auto t_end_split = chrono::high_resolution_clock::now();

        // 4) Write to files
        auto t_start_write = chrono::high_resolution_clock::now();

        string base = to_string(n);
        string filePassed = "students_" + base + "_passed_" + suffix + ".txt";
        string fileFailed = "students_" + base + "_failed_" + suffix + ".txt";

        writeStudentsToFile(passed, filePassed);
        writeStudentsToFile(failed, fileFailed);

        auto t_end_write = chrono::high_resolution_clock::now();

        // Timing (ms)
        auto gen_ms   = chrono::duration_cast<chrono::milliseconds>(t_end_gen   - t_start_gen).count();
        auto sort_ms  = chrono::duration_cast<chrono::milliseconds>(t_end_sort  - t_start_sort).count();
        auto split_ms = chrono::duration_cast<chrono::milliseconds>(t_end_split - t_start_split).count();
        auto write_ms = chrono::duration_cast<chrono::milliseconds>(t_end_write - t_start_write).count();

        cout << "Generate: " << gen_ms  << " ms\n";
        cout << "Sort:     " << sort_ms << " ms\n";
        cout << "Split:    " << split_ms << " ms\n";
        cout << "Write:    " << write_ms << " ms\n";
        cout << "Passed:   " << passed.size()
             << ", Failed: " << failed.size() << "\n";
    }

    cout << "\n" << label << " performance test finished.\n";
}

// --------------------------------------------------
// main()
// --------------------------------------------------
int main() {
    setupConsole();

    cout << "=== STUDENT GRADING SYSTEM – v0.25 ===\n\n";
    cout << "Select option:\n";
    cout << "1. Performance test (list)\n";
    cout << "2. Performance test (deque)\n";
    cout << "3. Run both tests (list + deque)\n";
    cout << "Choice: ";

    int choice;
    if (!(cin >> choice)) {
        cout << "Invalid input. Exiting.\n";
        return 0;
    }

    try {
        if (choice == 1) {
            // LIST ONLY
            runContainerTest<list<Person>>("LIST", "list");
        } else if (choice == 2) {
            // DEQUE ONLY
            runContainerTest<deque<Person>>("DEQUE", "deque");
        } else if (choice == 3) {
            // BOTH
            runContainerTest<list<Person>>("LIST", "list");
            runContainerTest<deque<Person>>("DEQUE", "deque");
        } else {
            cout << "Unknown option. Exiting.\n";
            return 0;
        }
    } catch (const exception& ex) {
        cerr << "\nERROR: " << ex.what() << "\n";
    }

    cout << "\nProgram completed. Press Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
