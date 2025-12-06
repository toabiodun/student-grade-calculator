#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iomanip>
#include <random>
#include <chrono>
#include <stdexcept>
#include <limits>
#include "Person.h"

using namespace std;

// --------------------------------------------------
// Console setup
// --------------------------------------------------
void setupConsole() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

// --------------------------------------------------
// Random score generator for one student
// --------------------------------------------------
void generateRandomScores(Person& person, int numHomework) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);

    for (int i = 0; i < numHomework; ++i) {
        person.addHomeworkScore(dis(gen));
    }
    person.setExamScore(dis(gen));

    // we use average-based final grade for splitting
    person.calculateFinalGradeAverage();
}

// --------------------------------------------------
// Save any container of Person to file
// (works for vector, list, deque, ...)
// --------------------------------------------------
template <typename Container>
void saveStudentsToFile(const Container& students, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        throw runtime_error("Could not open output file: " + filename);
    }

    out << left << setw(20) << "FirstName"
        << setw(20) << "Surname"
        << right << setw(10) << "Final\n";
    out << string(50, '-') << "\n";

    for (const auto& s : students) {
        out << left << setw(20) << s.getFirstName()
            << setw(20) << s.getSurname()
            << right << setw(10) << fixed << setprecision(2)
            << s.getFinalGrade() << "\n";
    }
}

// ==================================================
// VECTOR PERFORMANCE TEST
// ==================================================
void performanceTestVector(const vector<size_t>& sizes) {
    cout << "\n=== VECTOR PERFORMANCE TEST (v0.25) ===\n";

    for (size_t n : sizes) {
        cout << "\n--- Testing with " << n << " students ---\n";

        // 1) Generate
        auto t_start_gen = chrono::high_resolution_clock::now();
        vector<Person> students;
        students.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            Person p("Name" + to_string(i + 1),
                     "Surname" + to_string(i + 1));
            generateRandomScores(p, 15);
            students.push_back(p);
        }
        auto t_end_gen = chrono::high_resolution_clock::now();

        // 2) Sort
        auto t_start_sort = chrono::high_resolution_clock::now();
        sort(students.begin(), students.end());
        auto t_end_sort = chrono::high_resolution_clock::now();

        // 3) Split
        auto t_start_split = chrono::high_resolution_clock::now();
        vector<Person> passed;
        vector<Person> failed;
        passed.reserve(n);
        failed.reserve(n);

        for (const auto& s : students) {
            if (s.getFinalGrade() >= 5.0)
                passed.push_back(s);
            else
                failed.push_back(s);
        }
        auto t_end_split = chrono::high_resolution_clock::now();

        // 4) Write
        auto t_start_write = chrono::high_resolution_clock::now();
        string base = "students_" + to_string(n);
        saveStudentsToFile(passed, base + "_passed_vector.txt");
        saveStudentsToFile(failed, base + "_failed_vector.txt");
        auto t_end_write = chrono::high_resolution_clock::now();

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
}

// ==================================================
// LIST PERFORMANCE TEST
// ==================================================
void performanceTestList(const vector<size_t>& sizes) {
    cout << "\n=== LIST PERFORMANCE TEST (v0.25) ===\n";

    for (size_t n : sizes) {
        cout << "\n--- Testing with " << n << " students ---\n";

        // 1) Generate
        auto t_start_gen = chrono::high_resolution_clock::now();
        list<Person> students;

        for (size_t i = 0; i < n; ++i) {
            Person p("Name" + to_string(i + 1),
                     "Surname" + to_string(i + 1));
            generateRandomScores(p, 15);
            students.push_back(p);
        }
        auto t_end_gen = chrono::high_resolution_clock::now();

        // 2) Sort (list has its own sort())
        auto t_start_sort = chrono::high_resolution_clock::now();
        students.sort();
        auto t_end_sort = chrono::high_resolution_clock::now();

        // 3) Split
        auto t_start_split = chrono::high_resolution_clock::now();
        list<Person> passed;
        list<Person> failed;

        for (const auto& s : students) {
            if (s.getFinalGrade() >= 5.0)
                passed.push_back(s);
            else
                failed.push_back(s);
        }
        auto t_end_split = chrono::high_resolution_clock::now();

        // 4) Write
        auto t_start_write = chrono::high_resolution_clock::now();
        string base = "students_" + to_string(n);
        saveStudentsToFile(passed, base + "_passed_list.txt");
        saveStudentsToFile(failed, base + "_failed_list.txt");
        auto t_end_write = chrono::high_resolution_clock::now();

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
}

// ==================================================
// DEQUE PERFORMANCE TEST
// ==================================================
void performanceTestDeque(const vector<size_t>& sizes) {
    cout << "\n=== DEQUE PERFORMANCE TEST (v0.25) ===\n";

    for (size_t n : sizes) {
        cout << "\n--- Testing with " << n << " students ---\n";

        // 1) Generate
        auto t_start_gen = chrono::high_resolution_clock::now();
        deque<Person> students;

        for (size_t i = 0; i < n; ++i) {
            Person p("Name" + to_string(i + 1),
                     "Surname" + to_string(i + 1));
            generateRandomScores(p, 15);
            students.push_back(p);
        }
        auto t_end_gen = chrono::high_resolution_clock::now();

        // 2) Sort
        auto t_start_sort = chrono::high_resolution_clock::now();
        sort(students.begin(), students.end());
        auto t_end_sort = chrono::high_resolution_clock::now();

        // 3) Split
        auto t_start_split = chrono::high_resolution_clock::now();
        deque<Person> passed;
        deque<Person> failed;

        for (const auto& s : students) {
            if (s.getFinalGrade() >= 5.0)
                passed.push_back(s);
            else
                failed.push_back(s);
        }
        auto t_end_split = chrono::high_resolution_clock::now();

        // 4) Write
        auto t_start_write = chrono::high_resolution_clock::now();
        string base = "students_" + to_string(n);
        saveStudentsToFile(passed, base + "_passed_deque.txt");
        saveStudentsToFile(failed, base + "_failed_deque.txt");
        auto t_end_write = chrono::high_resolution_clock::now();

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
}

// ==================================================
// MAIN
// ==================================================
int main() {
    setupConsole();

    // Dataset sizes used in all three tests
    vector<size_t> sizes = {1000, 10000, 100000, 1000000};

    cout << "=== STUDENT GRADING SYSTEM (v0.25) ===\n\n";
    cout << "Compare container performance:\n";
    cout << " 1. std::vector\n";
    cout << " 2. std::list\n";
    cout << " 3. std::deque\n";
    cout << " 4. Run ALL (vector + list + deque)\n";
    cout << "Choice: ";

    int choice;
    if (!(cin >> choice)) {
        cerr << "Invalid input.\n";
        return 0;
    }

    try {
        switch (choice) {
            case 1:
                performanceTestVector(sizes);
                break;
            case 2:
                performanceTestList(sizes);
                break;
            case 3:
                performanceTestDeque(sizes);
                break;
            case 4:
                performanceTestVector(sizes);
                performanceTestList(sizes);
                performanceTestDeque(sizes);
                break;
            default:
                cout << "Unknown option.\n";
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
