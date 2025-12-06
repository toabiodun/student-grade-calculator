#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <deque>
#include <chrono>
#include <random>
#include <fstream>

struct Student {
    std::string name;
    std::string surname;
    double finalGrade;
};

// Generate a random number (1–10)
int randomScore(std::mt19937 &gen) {
    std::uniform_int_distribution<int> dist(1, 10);
    return dist(gen);
}

// Generate a list of random students
template <typename Container>
Container generateStudents(size_t count) {
    Container students;
    std::mt19937 gen(std::random_device{}());

    for (size_t i = 1; i <= count; i++) {
        double hw1 = randomScore(gen);
        double hw2 = randomScore(gen);
        double hw3 = randomScore(gen);
        double exam = randomScore(gen);

        double avg = (hw1 + hw2 + hw3) / 3.0;
        double finalGrade = 0.4 * avg + 0.6 * exam;

        Student s;
        s.name = "Name" + std::to_string(i);
        s.surname = "Surname" + std::to_string(i);
        s.finalGrade = finalGrade;

        students.push_back(s);
    }
    return students;
}

// Split students into passed/failed using list or deque
template <typename Container>
void splitStudents(const Container& students, Container& passed, Container& failed) {
    for (const auto& s : students) {
        if (s.finalGrade >= 5.0)
            passed.push_back(s);
        else
            failed.push_back(s);
    }
}

// Write output to a file
template <typename Container>
void writeToFile(const std::string& filename, const Container& data) {
    std::ofstream out(filename);
    for (const auto& s : data) {
        out << s.name << " " << s.surname << " " << s.finalGrade << "\n";
    }
}

template <typename Container>
void testContainer(const std::string& label, size_t count) {
    using namespace std::chrono;

    std::cout << "\n--- Testing with " << count << " students (" << label << ") ---\n";

    // Generate
    auto start = high_resolution_clock::now();
    Container students = generateStudents<Container>(count);
    auto end = high_resolution_clock::now();
    auto genTime = duration_cast<milliseconds>(end - start).count();

    // Split
    Container passed, failed;
    start = high_resolution_clock::now();
    splitStudents(students, passed, failed);
    end = high_resolution_clock::now();
    auto splitTime = duration_cast<milliseconds>(end - start).count();

    // Write files
    start = high_resolution_clock::now();
    writeToFile("students_" + std::to_string(count) + "_passed_listdeque.txt", passed);
    writeToFile("students_" + std::to_string(count) + "_failed_listdeque.txt", failed);
    end = high_resolution_clock::now();
    auto writeTime = duration_cast<milliseconds>(end - start).count();

    // Results
    std::cout << "Generate: " << genTime << " ms\n";
    std::cout << "Split:    " << splitTime << " ms\n";
    std::cout << "Write:    " << writeTime << " ms\n";
    std::cout << "Passed: " << passed.size() << ", Failed: " << failed.size() << "\n";
}

int main() {
    std::cout << "=== LIST & DEQUE PERFORMANCE TEST (v0.25) ===\n";

    // Test with std::list
    std::cout << "\n### Testing std::list ###\n";
    testContainer<std::list<Student>>("list", 1000);
    testContainer<std::list<Student>>("list", 10000);
    testContainer<std::list<Student>>("list", 100000);
    // 1,000,000 might be too slow for std::list - optional
    // testContainer<std::list<Student>>("list", 1000000);

    // Test with std::deque
    std::cout << "\n### Testing std::deque ###\n";
    testContainer<std::deque<Student>>("deque", 1000);
    testContainer<std::deque<Student>>("deque", 10000);
    testContainer<std::deque<Student>>("deque", 100000);
    testContainer<std::deque<Student>>("deque", 1000000);

    std::cout << "\nPerformance test completed.\n";
    return 0;
}
