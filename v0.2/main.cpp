#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <limits>      // for numeric_limits
#include "Person.h"

using namespace std;

// -----------------------------
// Helper: make I/O a bit faster
// -----------------------------
void setupConsole() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

// -----------------------------
// Random score generator
// -----------------------------
void generateRandomScores(Person& person, int numHomework) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);

    for (int i = 0; i < numHomework; ++i) {
        person.addHomeworkScore(dis(gen));
    }
    person.setExamScore(dis(gen));
}

// -----------------------------
// Read students from file
// -----------------------------
vector<Person> readFromFile(const string& filename) {
    vector<Person> students;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'\n";
        return students; // empty
    }

    string line;
    // Skip header
    if (!getline(file, line)) {
        cout << "Error: File '" << filename << "' is empty or invalid.\n";
        return students;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string firstName, surname;
        int score;
        vector<int> scores;

        iss >> firstName >> surname;
        if (firstName.empty() || surname.empty()) {
            continue; // skip invalid lines
        }

        while (iss >> score) {
            scores.push_back(score);
        }

        if (scores.empty()) {
            continue;
        }

        Person p(firstName, surname);

        int examScore = scores.back();
        scores.pop_back();

        p.setHomeworkScores(scores);
        p.setExamScore(examScore);

        students.push_back(p);
    }

    return students;
}

// -----------------------------
// Display students in a table
// -----------------------------
void displayStudents(vector<Person>& students, bool useAverage, bool useMedian) {
    if (students.empty()) {
        cout << "No students to display.\n";
        return;
    }

    // Sort alphabetically by name (uses operator< in Person)
    sort(students.begin(), students.end());

    cout << "\n" << string(80, '=') << "\n";
    cout << "STUDENT GRADE REPORT\n";
    cout << string(80, '=') << "\n";

    cout << left << setw(20) << "First Name"
         << setw(20) << "Last Name";

    if (useAverage && useMedian) {
        cout << right << setw(20) << "Final (Avg.)"
             << setw(20) << "Final (Med.)";
    } else if (useAverage) {
        cout << right << setw(20) << "Final (Avg.)";
    } else {
        cout << right << setw(20) << "Final (Med.)";
    }
    cout << "\n" << string(80, '-') << "\n";

    for (auto& s : students) {
        cout << left << setw(20) << s.getFirstName()
             << setw(20) << s.getSurname();

        if (useAverage && useMedian) {
            s.calculateFinalGradeAverage();
            double avg = s.getFinalGrade();
            s.calculateFinalGradeMedian();
            double med = s.getFinalGrade();

            cout << right << setw(20) << fixed << setprecision(2) << avg
                 << setw(20) << fixed << setprecision(2) << med;
        } else if (useAverage) {
            s.calculateFinalGradeAverage();
            cout << right << setw(20) << fixed << setprecision(2)
                 << s.getFinalGrade();
        } else {
            s.calculateFinalGradeMedian();
            cout << right << setw(20) << fixed << setprecision(2)
                 << s.getFinalGrade();
        }
        cout << "\n";
    }

    cout << string(80, '=') << "\n";
}

// -----------------------------
// Split into passed / failed
// -----------------------------
void splitPassedFailed(
    const vector<Person>& students,
    vector<Person>& passed,
    vector<Person>& failed,
    double threshold = 5.0
) {
    passed.clear();
    failed.clear();

    for (const auto& s : students) {
        if (s.getFinalGrade() >= threshold) {
            passed.push_back(s);
        } else {
            failed.push_back(s);
        }
    }
}

// -----------------------------
// Save students to file
// -----------------------------
void saveStudentsToFile(const vector<Person>& students, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Error: Could not write to file '" << filename << "'\n";
        return;
    }

    out << left << setw(20) << "FirstName"
        << setw(20) << "Surname"
        << right << setw(10) << "Final\n";
    out << string(50, '-') << "\n";

    for (const auto& s : students) {
        out << left << setw(20) << s.getFirstName()
            << setw(20) << s.getSurname()
            << right << setw(10) << fixed << setprecision(2)
            << s.getFinalGrade()
            << "\n";
    }
}

// -----------------------------
// Performance test (vector)
// -----------------------------
void runVectorPerformanceTest() {
    cout << "\n=== VECTOR PERFORMANCE TEST (v0.2) ===\n";

    // required sizes (you can uncomment 10 000 000 if your PC can handle it)
    vector<size_t> sizes = {1000, 10000, 100000, 1000000 /*, 10000000 */};

    for (size_t n : sizes) {
        cout << "\n--- Testing with " << n << " students ---\n";

        // 1) Generate random students
        auto t_start_gen = chrono::high_resolution_clock::now();

        vector<Person> students;
        students.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            Person p("Name" + to_string(i + 1),
                     "Surname" + to_string(i + 1));
            generateRandomScores(p, 15);
            p.calculateFinalGradeAverage(); // use average
            students.push_back(p);
        }

        auto t_end_gen = chrono::high_resolution_clock::now();

        // 2) Sort
        auto t_start_sort = chrono::high_resolution_clock::now();
        sort(students.begin(), students.end());
        auto t_end_sort = chrono::high_resolution_clock::now();

        // 3) Split into passed / failed
        auto t_start_split = chrono::high_resolution_clock::now();
        vector<Person> passed, failed;
        splitPassedFailed(students, passed, failed);
        auto t_end_split = chrono::high_resolution_clock::now();

        // 4) Write to files
        auto t_start_write = chrono::high_resolution_clock::now();
        string base = "students_" + to_string(n);
        saveStudentsToFile(passed, base + "_passed_vector.txt");
        saveStudentsToFile(failed, base + "_failed_vector.txt");
        auto t_end_write = chrono::high_resolution_clock::now();

        // Calculate times in milliseconds
        auto gen_ms   = chrono::duration_cast<chrono::milliseconds>(t_end_gen   - t_start_gen).count();
        auto sort_ms  = chrono::duration_cast<chrono::milliseconds>(t_end_sort  - t_start_sort).count();
        auto split_ms = chrono::duration_cast<chrono::milliseconds>(t_end_split - t_start_split).count();
        auto write_ms = chrono::duration_cast<chrono::milliseconds>(t_end_write - t_start_write).count();

        cout << "Generate: " << gen_ms  << " ms\n";
        cout << "Sort:     " << sort_ms << " ms\n";
        cout << "Split:    " << split_ms << " ms\n";
        cout << "Write:    " << write_ms << " ms\n";
        cout << "Passed: " << passed.size()
             << ", Failed: " << failed.size() << "\n";
    }

    cout << "\nVector performance test finished.\n";
}

// -----------------------------
// MAIN
// -----------------------------
int main() {
    setupConsole();

    vector<Person> students;

    cout << "=== STUDENT GRADING SYSTEM (v0.2) ===\n\n";
    cout << "Select option:\n";
    cout << "1. Manual input\n";
    cout << "2. Read from file\n";
    cout << "3. Generate random data (in memory)\n";
    cout << "4. Performance test (vector: generate + sort + split + write)\n";
    cout << "Choice: ";

    int choice;
    if (!(cin >> choice)) {
        cout << "\nERROR: Invalid input (not a number).\n";
        cout << "Program terminated. Press Enter to exit...";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 0;
    }

    if (choice == 1) {
        int n;
        cout << "How many students? ";
        cin >> n;

        for (int i = 0; i < n; ++i) {
            cout << "\n--- Student " << (i + 1) << " ---\n";
            Person p;
            cin >> p;          // uses your operator>> from Person
            students.push_back(p);
        }
    }
    else if (choice == 2) {
        cout << "\nAvailable files:\n";
        cout << "1. students10000.txt\n";
        cout << "2. Custom filename\n";
        cout << "Choice: ";
        int fileChoice;
        cin >> fileChoice;

        string filename;
        if (fileChoice == 1) {
            filename = "students10000.txt";
        } else {
            cout << "Enter filename: ";
            cin >> filename;
        }

        students = readFromFile(filename);
        cout << "Read " << students.size() << " students from file.\n";
    }
    else if (choice == 3) {
        int n, hw;
        cout << "How many students? ";
        cin >> n;
        cout << "How many homework assignments? ";
        cin >> hw;

        for (int i = 0; i < n; ++i) {
            Person p("Student" + to_string(i + 1),
                     "Surname" + to_string(i + 1));
            generateRandomScores(p, hw);
            students.push_back(p);
        }
    }
    else if (choice == 4) {
        // NEW: full performance test (v0.2)
        runVectorPerformanceTest();
        cout << "\nProgram completed. Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 0;
    }
    else {
        cout << "\nERROR: Invalid menu choice.\n";
        cout << "Program terminated. Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 0;
    }

    if (students.empty()) {
        cout << "No students to process.\n";
        cout << "Program completed. Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 0;
    }

    cout << "\nSelect calculation method:\n";
    cout << "1. Average\n";
    cout << "2. Median\n";
    cout << "3. Both\n";
    cout << "Choice: ";
    cin >> choice;

    bool useAverage = (choice == 1 || choice == 3);
    bool useMedian  = (choice == 2 || choice == 3);

    displayStudents(students, useAverage, useMedian);

    cout << "\nProgram completed. Press Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
