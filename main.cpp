#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include "Person.h"

using namespace std;

// Function to ensure output displays immediately
void setupConsole() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    setvbuf(stdout, nullptr, _IONBF, 0); // Disable output buffering
}

// Function to generate random scores
void generateRandomScores(Person& person, int numHomework) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);
    
    for (int i = 0; i < numHomework; ++i) {
        person.addHomeworkScore(dis(gen));
    }
    person.setExamScore(dis(gen));
}

// Function to read students from file
vector<Person> readFromFile(const string& filename) {
    vector<Person> students;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return students;
    }
    
    string line;
    // Skip header line
    getline(file, line);
    
    while (getline(file, line)) {
        istringstream iss(line);
        string firstName, surname;
        int score;
        vector<int> homeworkScores;
        
        iss >> firstName >> surname;
        
        // Read all scores
        while (iss >> score) {
            homeworkScores.push_back(score);
        }
        
        if (!homeworkScores.empty()) {
            Person person(firstName, surname);
            
            // Last score is exam, rest are homework
            int examScore = homeworkScores.back();
            homeworkScores.pop_back();
            
            person.setHomeworkScores(homeworkScores);
            person.setExamScore(examScore);
            
            students.push_back(person);
        }
    }
    
    file.close();
    return students;
}

// Function to display students with both average and median
void displayStudents(vector<Person>& students, bool useAverage, bool useMedian) {
    // Sort students by name
    sort(students.begin(), students.end());
    
    // Calculate grades
    for (auto& student : students) {
        if (useAverage) {
            student.calculateFinalGradeAverage();
        } else if (useMedian) {
            student.calculateFinalGradeMedian();
        }
    }
    
    // IMPROVED HEADERS - CLEAR AND PROFESSIONAL
    cout << "\n" << string(80, '=') << endl;
    cout << "STUDENT GRADE REPORT" << endl;
    cout << string(80, '=') << endl;
    
    cout << left << setw(20) << "First Name" 
         << setw(20) << "Last Name";
    
    if (useAverage && useMedian) {
        cout << right << setw(20) << "Final Grade (Avg)" 
             << setw(20) << "Final Grade (Med)" << endl;
    } else if (useAverage) {
        cout << right << setw(20) << "Final Grade (Avg)" << endl;
    } else {
        cout << right << setw(20) << "Final Grade (Med)" << endl;
    }
    
    cout << string(80, '-') << endl;
    
    // Display students
    for (auto& student : students) {
        cout << left << setw(20) << student.getFirstName()
             << setw(20) << student.getSurname();
        
        if (useAverage && useMedian) {
            // Calculate and display both
            student.calculateFinalGradeAverage();
            double avgGrade = student.getFinalGrade();
            student.calculateFinalGradeMedian();
            double medGrade = student.getFinalGrade();
            
            cout << right << setw(20) << fixed << setprecision(2) << avgGrade
                 << setw(20) << fixed << setprecision(2) << medGrade << endl;
        } else {
            cout << right << setw(20) << fixed << setprecision(2) 
                 << student.getFinalGrade() << endl;
        }
    }
    
    cout << string(80, '=') << endl;
    
    // ADDED STATISTICS SUMMARY
    if (!students.empty()) {
        double totalAvg = 0.0, totalMed = 0.0;
        double highestGrade = 0.0, lowestGrade = 10.0;
        
        for (auto& student : students) {
            double grade;
            if (useAverage && useMedian) {
                student.calculateFinalGradeAverage();
                grade = student.getFinalGrade();
            } else if (useAverage) {
                student.calculateFinalGradeAverage();
                grade = student.getFinalGrade();
            } else {
                student.calculateFinalGradeMedian();
                grade = student.getFinalGrade();
            }
            
            totalAvg += grade;
            if (grade > highestGrade) highestGrade = grade;
            if (grade < lowestGrade) lowestGrade = grade;
        }
        
        double averageGrade = totalAvg / students.size();
        
        cout << "\n=== STATISTICS SUMMARY ===" << endl;
        cout << "Total Students: " << students.size() << endl;
        cout << "Average Grade: " << fixed << setprecision(2) << averageGrade << endl;
        cout << "Highest Grade: " << fixed << setprecision(2) << highestGrade << endl;
        cout << "Lowest Grade: " << fixed << setprecision(2) << lowestGrade << endl;
        cout << "Calculation Method: " << (useAverage && useMedian ? "Both Average & Median" : 
                                          (useAverage ? "Average" : "Median")) << endl;
    }
}

int main() {
    // SETUP CONSOLE FOR IMMEDIATE OUTPUT
    setupConsole();
    
    vector<Person> students;
    int choice;
    
    // FORCE OUTPUT TO APPEAR IMMEDIATELY
    cout << "=== STUDENT GRADING SYSTEM ===" << endl;
    cout.flush(); // Force output to display
    
    cout << "\nSelect input method:" << endl;
    cout << "1. Manual input" << endl;
    cout << "2. Read from file" << endl;
    cout << "3. Generate random data" << endl;
    cout << "Choice: ";
    cout.flush(); // Force output to display
    cin >> choice;
    
    if (choice == 1) {
        // Manual input
        int numStudents;
        cout << "How many students? ";
        cout.flush();
        cin >> numStudents;
        
        for (int i = 0; i < numStudents; ++i) {
            cout << "\n--- Student " << (i + 1) << " ---" << endl;
            cout.flush();
            Person person;
            cin >> person;
            students.push_back(person);
        }
    } else if (choice == 2) {
        // File input
        cout << "\nAvailable files:" << endl;
        cout << "1. students10000.txt" << endl;
        cout << "2. students100000.txt" << endl;
        cout << "3. students1000000.txt" << endl;
        cout << "4. Custom filename" << endl;
        cout << "Choice: ";
        cout.flush();
        
        int fileChoice;
        cin >> fileChoice;
        
        string filename;
        switch (fileChoice) {
            case 1: filename = "students10000.txt"; break;
            case 2: filename = "students100000.txt"; break;
            case 3: filename = "students1000000.txt"; break;
            case 4:
                cout << "Enter filename: ";
                cout.flush();
                cin >> filename;
                break;
            default:
                filename = "students10000.txt";
        }
        
        students = readFromFile(filename);
        cout << "Read " << students.size() << " students from file." << endl;
        cout.flush();
    } else if (choice == 3) {
        // Random generation
        int numStudents, numHomework;
        cout << "How many students? ";
        cout.flush();
        cin >> numStudents;
        cout << "How many homework assignments? ";
        cout.flush();
        cin >> numHomework;
        
        for (int i = 0; i < numStudents; ++i) {
            Person person("Student" + to_string(i + 1), "Surname" + to_string(i + 1));
            generateRandomScores(person, numHomework);
            students.push_back(person);
        }
    }
    
    if (students.empty()) {
        cout << "No students to process." << endl;
        cout.flush();
        return 0;
    }
    
    // Choose calculation method
    cout << "\nSelect calculation method:" << endl;
    cout << "1. Average" << endl;
    cout << "2. Median" << endl;
    cout << "3. Both" << endl;
    cout << "Choice: ";
    cout.flush();
    cin >> choice;
    
    bool useAverage = (choice == 1 || choice == 3);
    bool useMedian = (choice == 2 || choice == 3);
    
    displayStudents(students, useAverage, useMedian);
    
    // Add pause at the end to see results
    cout << "\nProgram completed. Press Enter to exit...";
    cout.flush();
    cin.ignore();
    cin.get();
    
    return 0;
}