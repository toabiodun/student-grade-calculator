#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include "Person.h"

using namespace std;

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
    
    // Display header
    cout << "\n" << string(70, '=') << endl;
    cout << left << setw(15) << "Name" 
         << setw(15) << "Surname";
    
    if (useAverage && useMedian) {
        cout << right << setw(20) << "Final (Avg.)" 
             << setw(20) << "Final (Med.)" << endl;
    } else if (useAverage) {
        cout << right << setw(20) << "Final (Avg.)" << endl;
    } else {
        cout << right << setw(20) << "Final (Med.)" << endl;
    }
    
    cout << string(70, '-') << endl;
    
    // Display students
    for (auto& student : students) {
        cout << left << setw(15) << student.getFirstName()
             << setw(15) << student.getSurname();
        
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
    
    cout << string(70, '=') << endl;
}

int main() {
    vector<Person> students;
    int choice;
    
    cout << "=== Student Grading System ===" << endl;
    cout << "\nSelect input method:" << endl;
    cout << "1. Manual input" << endl;
    cout << "2. Read from file" << endl;
    cout << "3. Generate random data" << endl;
    cout << "Choice: ";
    cin >> choice;
    
    if (choice == 1) {
        // Manual input
        int numStudents;
        cout << "How many students? ";
        cin >> numStudents;
        
        for (int i = 0; i < numStudents; ++i) {
            cout << "\n--- Student " << (i + 1) << " ---" << endl;
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
        
        int fileChoice;
        cin >> fileChoice;
        
        string filename;
        switch (fileChoice) {
            case 1: filename = "students10000.txt"; break;
            case 2: filename = "students100000.txt"; break;
            case 3: filename = "students1000000.txt"; break;
            case 4:
                cout << "Enter filename: ";
                cin >> filename;
                break;
            default:
                filename = "students10000.txt";
        }
        
        students = readFromFile(filename);
        cout << "Read " << students.size() << " students from file." << endl;
    } else if (choice == 3) {
        // Random generation
        int numStudents, numHomework;
        cout << "How many students? ";
        cin >> numStudents;
        cout << "How many homework assignments? ";
        cin >> numHomework;
        
        for (int i = 0; i < numStudents; ++i) {
            Person person("Student" + to_string(i + 1), "Surname" + to_string(i + 1));
            generateRandomScores(person, numHomework);
            students.push_back(person);
        }
    }
    
    if (students.empty()) {
        cout << "No students to process." << endl;
        return 0;
    }
    
    // Choose calculation method
    cout << "\nSelect calculation method:" << endl;
    cout << "1. Average" << endl;
    cout << "2. Median" << endl;
    cout << "3. Both" << endl;
    cout << "Choice: ";
    cin >> choice;
    
    bool useAverage = (choice == 1 || choice == 3);
    bool useMedian = (choice == 2 || choice == 3);
    
    displayStudents(students, useAverage, useMedian);
    
    return 0;
}
