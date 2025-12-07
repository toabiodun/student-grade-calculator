#include <iostream>
#include <fstream>
#include <list>
#include <deque>
#include <vector>
#include <algorithm>
#include <chrono>
#include "Person.h"

using namespace std;

// -------------------- Utility: generate random student --------------------
Person generateRandomStudent(int id) {
    Person p("Name" + to_string(id), "Surname" + to_string(id));

    vector<int> hw(15);
    for (int& x : hw) x = rand() % 10 + 1;
    p.setHomeworkScores(hw);

    int exam = rand() % 10 + 1;
    p.setExamScore(exam);

    p.calculateFinalGradeAverage();
    return p;
}

// -------------------- Split students into passed/failed --------------------
template <typename Container>
void splitStudents(const Container& students, Container& passed, Container& failed) {
    for (const auto& s : students) {
        if (s.getFinalGrade() >= 5.0) passed.push_back(s);
        else failed.push_back(s);
    }
}

// -------------------- Write results to file --------------------
template <typename Container>
void writeToFile(const string& filename, const Container& students) {
    ofstream out(filename);
    for (const auto& s : students) {
        out << s.getFirstName() << " " 
            << s.getSurname() << " Final: " 
            << s.getFinalGrade() << "\n";
    }
    out.close();
}

// -------------------- Performance test for list/deque --------------------
template <typename Container>
void performanceTest(const string& typeName, int N) {
    cout << "\n--- Testing (" << typeName << ") with " << N << " students ---\n";

    Container students;

    // Generate
    auto t1 = chrono::high_resolution_clock::now();
    for (int i = 1; i <= N; i++) students.push_back(generateRandomStudent(i));
    auto t2 = chrono::high_resolution_clock::now();

    // Sort
    auto t3 = chrono::high_resolution_clock::now();
    students.sort([](const Person& a, const Person& b) {
        return a < b;
    });
    auto t4 = chrono::high_resolution_clock::now();

    // Split
    Container passed, failed;
    auto t5 = chrono::high_resolution_clock::now();
    splitStudents(students, passed, failed);
    auto t6 = chrono::high_resolution_clock::now();

    // Write
    auto t7 = chrono::high_resolution_clock::now();
    writeToFile("students_" + to_string(N) + "_passed_" + typeName + ".txt", passed);
    writeToFile("students_" + to_string(N) + "_failed_" + typeName + ".txt", failed);
    auto t8 = chrono::high_resolution_clock::now();

    // Results
    cout << "Generate: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms\n";
    cout << "Sort:     " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << " ms\n";
    cout << "Split:    " << chrono::duration_cast<chrono::milliseconds>(t6 - t5).count() << " ms\n";
    cout << "Write:    " << chrono::duration_cast<chrono::milliseconds>(t8 - t7).count() << " ms\n";
    cout << "Passed: " << passed.size() << ", Failed: " << failed.size() << "\n";
}

// -------------------- MAIN PROGRAM --------------------
int main() {
    srand(time(NULL));

    cout << "=== STUDENT GRADING SYSTEM (v0.25 - list/deque version) ===\n";

    cout << "\nChoose performance test type:\n";
    cout << "1. Test using std::list\n";
    cout << "2. Test using std::deque\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;

    vector<int> testSizes = { 1000, 10000, 100000 };

    if (choice == 1) {
        cout << "\nRunning std::list performance tests...\n";
        for (int N : testSizes)
            performanceTest<list<Person>>("list", N);
    }
    else if (choice == 2) {
        cout << "\nRunning std::deque performance tests...\n";
        for (int N : testSizes)
            performanceTest<deque<Person>>("deque", N);
    }
    else {
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\nPerformance test finished.\n";
    return 0;
}
