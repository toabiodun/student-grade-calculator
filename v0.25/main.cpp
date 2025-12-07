#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <stdexcept>

#include "Person.h"

using namespace std;

// ------------------------------------------------------
// Helper: faster console I/O
// ------------------------------------------------------
void setupConsole() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

// ------------------------------------------------------
// Random score generator used for all containers
// ------------------------------------------------------
void fillRandomScores(Person &p, int index) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 10);

    // Example names: Name1 Surname1, Name2 Surname2, ...
    p.setFirstName("Name" + to_string(index + 1));
    p.setSurname("Surname" + to_string(index + 1));

    vector<int> hw(15);
    for (int &x : hw) x = dist(gen);

    int exam = dist(gen);

    p.setHomeworkScores(hw);
    p.setExamScore(exam);
    p.calculateFinalGradeAverage();   // finalGrade stored in Person
}

// ------------------------------------------------------
// Generate students for each container type
// ------------------------------------------------------
vector<Person> generateVector(int n) {
    vector<Person> students;
    students.reserve(n);

    for (int i = 0; i < n; ++i) {
        Person p;
        fillRandomScores(p, i);
        students.push_back(p);
    }
    return students;
}

list<Person> generateList(int n) {
    list<Person> students;
    for (int i = 0; i < n; ++i) {
        Person p;
        fillRandomScores(p, i);
        students.push_back(p);
    }
    return students;
}

deque<Person> generateDeque(int n) {
    deque<Person> students;
    students.resize(n);

    for (int i = 0; i < n; ++i) {
        Person p;
        fillRandomScores(p, i);
        students[i] = p;
    }
    return students;
}

// ------------------------------------------------------
// Split passed / failed
// ------------------------------------------------------
vector<Person> splitPassedVector(const vector<Person> &all) {
    vector<Person> passed;
    passed.reserve(all.size());
    for (const auto &s : all)
        if (s.getFinalGrade() >= 5.0) passed.push_back(s);
    return passed;
}

list<Person> splitPassedList(const list<Person> &all) {
    list<Person> passed;
    for (const auto &s : all)
        if (s.getFinalGrade() >= 5.0) passed.push_back(s);
    return passed;
}

deque<Person> splitPassedDeque(const deque<Person> &all) {
    deque<Person> passed;
    for (const auto &s : all)
        if (s.getFinalGrade() >= 5.0) passed.push_back(s);
    return passed;
}

// ------------------------------------------------------
// Write results to files
// ------------------------------------------------------
void writeVectorResults(
    const vector<Person> &passed,
    const vector<Person> &all,
    int n,
    const string &suffix
) {
    ofstream outPassed("students_" + to_string(n) + "_passed_" + suffix + ".txt");
    ofstream outFailed("students_" + to_string(n) + "_failed_" + suffix + ".txt");

    if (!outPassed.is_open() || !outFailed.is_open())
        throw runtime_error("Cannot open output files for vector/" + suffix);

    for (const auto &s : passed)
        outPassed << s.getFirstName() << ' '
                  << s.getSurname()  << ' '
                  << fixed << setprecision(2) << s.getFinalGrade() << '\n';

    for (const auto &s : all)
        if (s.getFinalGrade() < 5.0)
            outFailed << s.getFirstName() << ' '
                      << s.getSurname()  << ' '
                      << fixed << setprecision(2) << s.getFinalGrade() << '\n';
}

void writeListResults(
    const list<Person> &passed,
    const list<Person> &all,
    int n
) {
    ofstream outPassed("students_" + to_string(n) + "_passed_list.txt");
    ofstream outFailed("students_" + to_string(n) + "_failed_list.txt");

    if (!outPassed.is_open() || !outFailed.is_open())
        throw runtime_error("Cannot open output files for list");

    for (const auto &s : passed)
        outPassed << s.getFirstName() << ' '
                  << s.getSurname()  << ' '
                  << fixed << setprecision(2) << s.getFinalGrade() << '\n';

    for (const auto &s : all)
        if (s.getFinalGrade() < 5.0)
            outFailed << s.getFirstName() << ' '
                      << s.getSurname()  << ' '
                      << fixed << setprecision(2) << s.getFinalGrade() << '\n';
}

void writeDequeResults(
    const deque<Person> &passed,
    const deque<Person> &all,
    int n
) {
    ofstream outPassed("students_" + to_string(n) + "_passed_deque.txt");
    ofstream outFailed("students_" + to_string(n) + "_failed_deque.txt");

    if (!outPassed.is_open() || !outFailed.is_open())
        throw runtime_error("Cannot open output files for deque");

    for (const auto &s : passed)
        outPassed << s.getFirstName() << ' '
                  << s.getSurname()  << ' '
                  << fixed << setprecision(2) << s.getFinalGrade() << '\n';

    for (const auto &s : all)
        if (s.getFinalGrade() < 5.0)
            outFailed << s.getFirstName() << ' '
                      << s.getSurname()  << ' '
                      << fixed << setprecision(2) << s.getFinalGrade() << '\n';
}

// ------------------------------------------------------
// Performance tests
// ------------------------------------------------------
void performanceTestList() {
    vector<int> sizes = {1000, 10000, 100000};

    cout << "\n=== LIST PERFORMANCE TEST (v0.25) ===\n";

    for (int n : sizes) {
        cout << "\n--- Testing list with " << n << " students ---\n";

        auto tStartGen = chrono::high_resolution_clock::now();
        list<Person> students = generateList(n);
        auto tEndGen = chrono::high_resolution_clock::now();

        auto tStartSort = chrono::high_resolution_clock::now();
        students.sort();  // list has its own sort()
        auto tEndSort = chrono::high_resolution_clock::now();

        auto tStartSplit = chrono::high_resolution_clock::now();
        list<Person> passed = splitPassedList(students);
        auto tEndSplit = chrono::high_resolution_clock::now();

        auto tStartWrite = chrono::high_resolution_clock::now();
        writeListResults(passed, students, n);
        auto tEndWrite = chrono::high_resolution_clock::now();

        cout << "Generate: "
             << chrono::duration_cast<chrono::milliseconds>(tEndGen - tStartGen).count()
             << " ms\n";
        cout << "Sort:     "
             << chrono::duration_cast<chrono::milliseconds>(tEndSort - tStartSort).count()
             << " ms\n";
        cout << "Split:    "
             << chrono::duration_cast<chrono::milliseconds>(tEndSplit - tStartSplit).count()
             << " ms\n";
        cout << "Write:    "
             << chrono::duration_cast<chrono::milliseconds>(tEndWrite - tStartWrite).count()
             << " ms\n";

        cout << "Passed: " << passed.size()
             << ", Failed: " << (students.size() - passed.size()) << "\n";
    }

    cout << "\nList performance test finished.\n";
}

void performanceTestDeque() {
    vector<int> sizes = {1000, 10000, 100000};

    cout << "\n=== DEQUE PERFORMANCE TEST (v0.25) ===\n";

    for (int n : sizes) {
        cout << "\n--- Testing deque with " << n << " students ---\n";

        auto tStartGen = chrono::high_resolution_clock::now();
        deque<Person> students = generateDeque(n);
        auto tEndGen = chrono::high_resolution_clock::now();

        auto tStartSort = chrono::high_resolution_clock::now();
        sort(students.begin(), students.end());
        auto tEndSort = chrono::high_resolution_clock::now();

        auto tStartSplit = chrono::high_resolution_clock::now();
        deque<Person> passed = splitPassedDeque(students);
        auto tEndSplit = chrono::high_resolution_clock::now();

        auto tStartWrite = chrono::high_resolution_clock::now();
        writeDequeResults(passed, students, n);
        auto tEndWrite = chrono::high_resolution_clock::now();

        cout << "Generate: "
             << chrono::duration_cast<chrono::milliseconds>(tEndGen - tStartGen).count()
             << " ms\n";
        cout << "Sort:     "
             << chrono::duration_cast<chrono::milliseconds>(tEndSort - tStartSort).count()
             << " ms\n";
        cout << "Split:    "
             << chrono::duration_cast<chrono::milliseconds>(tEndSplit - tStartSplit).count()
             << " ms\n";
        cout << "Write:    "
             << chrono::duration_cast<chrono::milliseconds>(tEndWrite - tStartWrite).count()
             << " ms\n";

        cout << "Passed: " << passed.size()
             << ", Failed: " << (students.size() - passed.size()) << "\n";
    }

    cout << "\nDeque performance test finished.\n";
}

// ------------------------------------------------------
// MAIN
// ------------------------------------------------------
int main() {
    setupConsole();

    try {
        cout << "=== STUDENT GRADING SYSTEM – v0.25 ===\n\n";
        cout << "Select option:\n";
        cout << "1. Performance test (list)\n";
        cout << "2. Performance test (deque)\n";
        cout << "3. Run both tests (list + deque)\n";
        cout << "Choice: ";

        int choice;
        if (!(cin >> choice)) {
            cerr << "Invalid input. Exiting.\n";
            return 1;
        }

        if (choice == 1) {
            performanceTestList();
        } else if (choice == 2) {
            performanceTestDeque();
        } else if (choice == 3) {
            performanceTestList();
            performanceTestDeque();
        } else {
            cerr << "Unknown menu option.\n";
            return 1;
        }

        cout << "\nProgram completed. Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    catch (const exception &ex) {
        cerr << "\nERROR: " << ex.what() << "\n";
        cerr << "Program terminated. Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 1;
    }

    return 0;
}
