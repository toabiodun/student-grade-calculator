#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <iostream>

class Person {
private:
    std::string firstName;
    std::string surname;
    std::vector<int> homeworkScores;
    int examScore;
    double finalGrade;

public:
    // Constructors
    Person();
    Person(const std::string& firstName, const std::string& surname);
    
    // Rule of Three
    Person(const Person& other);                    // Copy constructor
    Person& operator=(const Person& other);         // Assignment operator
    ~Person();                                      // Destructor
    
    // Getters
    std::string getFirstName() const { return firstName; }
    std::string getSurname() const { return surname; }
    double getFinalGrade() const { return finalGrade; }
    const std::vector<int>& getHomeworkScores() const { return homeworkScores; }
    int getExamScore() const { return examScore; }
    
    // Setters
    void setFirstName(const std::string& name) { firstName = name; }
    void setSurname(const std::string& name) { surname = name; }
    void addHomeworkScore(int score) { homeworkScores.push_back(score); }
    void setExamScore(int score) { examScore = score; }
    void setHomeworkScores(const std::vector<int>& scores) { homeworkScores = scores; }
    
    // Calculation methods
    void calculateFinalGradeAverage();
    void calculateFinalGradeMedian();
    
    // I/O operators
    friend std::istream& operator>>(std::istream& is, Person& person);
    friend std::ostream& operator<<(std::ostream& os, const Person& person);
    
    // Comparison for sorting
    bool operator<(const Person& other) const;
};

#endif
