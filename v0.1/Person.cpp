#include "Person.h"
#include <algorithm>
#include <numeric>
#include <iomanip>

// Default constructor
Person::Person() : firstName(""), surname(""), examScore(0), finalGrade(0.0) {}

// Parameterized constructor
Person::Person(const std::string& firstName, const std::string& surname)
    : firstName(firstName), surname(surname), examScore(0), finalGrade(0.0) {}

// Copy constructor (Rule of Three)
Person::Person(const Person& other)
    : firstName(other.firstName),
      surname(other.surname),
      homeworkScores(other.homeworkScores),
      examScore(other.examScore),
      finalGrade(other.finalGrade) {}

// Assignment operator (Rule of Three)
Person& Person::operator=(const Person& other) {
    if (this != &other) {
        firstName = other.firstName;
        surname = other.surname;
        homeworkScores = other.homeworkScores;
        examScore = other.examScore;
        finalGrade = other.finalGrade;
    }
    return *this;
}

// Destructor (Rule of Three)
Person::~Person() {
    // Vector automatically cleans up its memory
}

// Calculate final grade using average
void Person::calculateFinalGradeAverage() {
    if (homeworkScores.empty()) {
        finalGrade = 0.6 * examScore;
        return;
    }
    
    double sum = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
    double average = sum / homeworkScores.size();
    finalGrade = 0.4 * average + 0.6 * examScore;
}

// Calculate final grade using median
void Person::calculateFinalGradeMedian() {
    if (homeworkScores.empty()) {
        finalGrade = 0.6 * examScore;
        return;
    }
    
    std::vector<int> sortedScores = homeworkScores;
    std::sort(sortedScores.begin(), sortedScores.end());
    
    double median;
    size_t size = sortedScores.size();
    if (size % 2 == 0) {
        median = (sortedScores[size / 2 - 1] + sortedScores[size / 2]) / 2.0;
    } else {
        median = sortedScores[size / 2];
    }
    
    finalGrade = 0.4 * median + 0.6 * examScore;
}

// Input operator
std::istream& operator>>(std::istream& is, Person& person) {
    std::cout << "Enter first name: ";
    is >> person.firstName;
    std::cout << "Enter surname: ";
    is >> person.surname;
    
    std::cout << "Enter homework scores (enter -1 to finish):\n";
    int score;
    while (true) {
        std::cout << "Homework score: ";
        is >> score;
        if (score == -1) break;
        if (score >= 0 && score <= 10) {
            person.homeworkScores.push_back(score);
        } else {
            std::cout << "Invalid score. Please enter a value between 0 and 10.\n";
        }
    }
    
    std::cout << "Enter exam score (0-10): ";
    is >> person.examScore;
    
    return is;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << std::left << std::setw(15) << person.firstName
       << std::setw(15) << person.surname
       << std::right << std::setw(10) << std::fixed << std::setprecision(2)
       << person.finalGrade;
    return os;
}

// Comparison operator for sorting
bool Person::operator<(const Person& other) const {
    if (firstName != other.firstName) {
        return firstName < other.firstName;
    }
    return surname < other.surname;
}
