// v1.0/main.cpp
//
// Final version – compares two splitting strategies
// for three containers: std::vector, std::list, std::deque.
//
// Strategy 1: copy students into two new containers
//             (passed + failed, original unchanged)
// Strategy 2: move failed students out of the base container
//             so only passed students remain there.
//
// This file uses algorithms from <algorithm> such as:
// std::sort, std::partition_copy, std::stable_partition, std::copy.
//

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <iterator>   // std::back_inserter
#include <limits>     // std::numeric_limits

#include "Person.h"

using namespace std;

// ------------------------------------------------------------
// Helper: faster console I/O (optional, but nice)
// ------------------------------------------------------------
void setupConsole()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

// ------------------------------------------------------------
// Random score generator used for all containers
// ------------------------------------------------------------
void fillRandomScores(Person &p, int index)
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(1, 10);

    // Example names: Name1 Surname1, Name2 Surname2, ...
    p.setFirstName("Name" + to_string(index + 1));
    p.setSurname("Surname" + to_string(index + 1));

    // 15 homework scores
    vector<int> hw(15);
    for (int &x : hw)
        x = dist(gen);

    p.setHomeworkScores(hw);

    // exam
    p.setExamScore(dist(gen));

    // calculate final grade using average method
    p.calculateFinalGradeAverage();
}

// ------------------------------------------------------------
// Generate N random students into any container
// ------------------------------------------------------------
template<typename Container>
void generateStudents(Container &students, size_t count)
{
    students.clear();
    students.reserve(count);   // works for vector/deque; list just ignores

    for (size_t i = 0; i < count; ++i)
    {
        Person p;
        fillRandomScores(p, static_cast<int>(i));
        students.push_back(p);
    }
}

// ------------------------------------------------------------
// Sorting helpers
//  - generic version uses std::sort
//  - specialization for std::list uses list::sort()
// ------------------------------------------------------------
template<typename Container>
void sortStudents(Container &students)
{
    std::sort(students.begin(), students.end());
}

// specialization for std::list<Person>
template<>
void sortStudents<std::list<Person>>(std::list<Person> &students)
{
    students.sort();
}

// ------------------------------------------------------------
// Strategy 1:
//   Split into TWO new containers (passed + failed).
//   Original "students" container is not modified.
//   Uses std::partition_copy from <algorithm>.
// ------------------------------------------------------------
template<typename Container>
void splitStrategyOne(const Container &students,
                      Container &passed,
                      Container &failed)
{
    passed.clear();
    failed.clear();

    std::partition_copy(
        students.begin(), students.end(),
        std::back_inserter(passed),
        std::back_inserter(failed),
        [](const Person &p)
        {
            return p.getFinalGrade() >= 5.0;   // true -> passed
        }
    );
}

// ------------------------------------------------------------
// Strategy 2:
//   Only ONE new container is created (failed).
//   Failed students are moved into this container and removed
//   from the base "students" container. After this step,
//   "students" keeps only passed students.
//
//   Uses std::stable_partition + std::copy from <algorithm>.
// ------------------------------------------------------------
template<typename Container>
void splitStrategyTwo(Container &students,
                      Container &failed)
{
    failed.clear();

    // Partition in-place: passed first, failed at the end.
    typename Container::iterator it = std::stable_partition(
        students.begin(), students.end(),
        [](const Person &p)
        {
            return p.getFinalGrade() >= 5.0;   // keep passed at the front
        }
    );

    // Copy failed students into "failed" container
    std::copy(it, students.end(), std::back_inserter(failed));

    // Remove failed students from base container
    students.erase(it, students.end());
}

// ------------------------------------------------------------
// Run tests for one container type and one strategy.
// strategy == 1  -> Strategy 1 (copy to two containers)
// strategy == 2  -> Strategy 2 (move failed out)
// ------------------------------------------------------------
template<typename Container>
void runContainerTests(const std::string &containerName,
                       int strategy)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;

    const size_t sizes[] = { 1000, 10000, 100000 };

    std::cout << "\n=== " << containerName
              << " – Strategy " << strategy << " ===\n";

    for (size_t n : sizes)
    {
        Container students;
        Container passed;  // used only for strategy 1 (for clarity)
        Container failed;

        std::cout << "\n--- Testing with " << n << " students ---\n";

        // 1. Generate
        auto t1 = high_resolution_clock::now();
        generateStudents(students, n);
        auto t2 = high_resolution_clock::now();
        long genMs = duration_cast<milliseconds>(t2 - t1).count();

        // 2. Sort
        t1 = high_resolution_clock::now();
        sortStudents(students);
        t2 = high_resolution_clock::now();
        long sortMs = duration_cast<milliseconds>(t2 - t1).count();

        // 3. Split
        long splitMs = 0;
        size_t passedCount = 0;
        size_t failedCount = 0;

        if (strategy == 1)
        {
            // Strategy 1: two new containers
            t1 = high_resolution_clock::now();
            splitStrategyOne(students, passed, failed);
            t2 = high_resolution_clock::now();
            splitMs = duration_cast<milliseconds>(t2 - t1).count();

            passedCount = passed.size();
            failedCount = failed.size();
        }
        else
        {
            // Strategy 2: move failed students out of base container
            t1 = high_resolution_clock::now();
            splitStrategyTwo(students, failed);
            t2 = high_resolution_clock::now();
            splitMs = duration_cast<milliseconds>(t2 - t1).count();

            passedCount = students.size();   // only passed remain
            failedCount = failed.size();
        }

        // Print results
        std::cout << "Generate: " << genMs   << " ms\n";
        std::cout << "Sort:     " << sortMs  << " ms\n";
        std::cout << "Split:    " << splitMs << " ms\n";
        std::cout << "Passed:   " << passedCount
                  << ", Failed: " << failedCount << "\n";
    }
}

// ------------------------------------------------------------
// Main menu
// ------------------------------------------------------------
int main()
{
    setupConsole();

    std::cout << "=== STUDENT GRADING SYSTEM – v1.0 ===\n\n";
    std::cout << "This version compares two strategies for splitting\n";
    std::cout << "students into PASSED and FAILED groups using:\n";
    std::cout << "  * std::vector\n";
    std::cout << "  * std::list\n";
    std::cout << "  * std::deque\n\n";

    std::cout << "Choose splitting strategy:\n";
    std::cout << "1. Strategy 1 – copy to two new containers\n";
    std::cout << "2. Strategy 2 – move FAILED out of base container\n";
    std::cout << "3. Run BOTH strategies\n";
    std::cout << "Choice: ";

    int choice = 0;
    if (!(cin >> choice))
    {
        std::cerr << "Invalid input. Exiting.\n";
        return 0;
    }

    try
    {
        if (choice == 1 || choice == 3)
        {
            std::cout << "\n\n=== RUNNING STRATEGY 1 (copy two containers) ===\n";
            runContainerTests<std::vector<Person>>("std::vector", 1);
            runContainerTests<std::list<Person>>  ("std::list",    1);
            runContainerTests<std::deque<Person>> ("std::deque",   1);
        }

        if (choice == 2 || choice == 3)
        {
            std::cout << "\n\n=== RUNNING STRATEGY 2 (move FAILED out) ===\n";
            runContainerTests<std::vector<Person>>("std::vector", 2);
            runContainerTests<std::list<Person>>  ("std::list",    2);
            runContainerTests<std::deque<Person>> ("std::deque",   2);
        }

        if (choice != 1 && choice != 2 && choice != 3)
        {
            std::cout << "Unknown option. Exiting.\n";
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "\nERROR: " << ex.what() << "\n";
    }

    std::cout << "\nProgram completed. Press Enter to exit...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();

    return 0;
}
