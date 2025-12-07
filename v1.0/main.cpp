#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <limits>
#include <type_traits>

#include "Person.h"

using namespace std;

// -----------------------------------------------
// Helper types for timing
// -----------------------------------------------
using Clock = chrono::high_resolution_clock;
using ms    = chrono::milliseconds;

// -----------------------------------------------
// Faster console I/O (but keep cin tied to cout)
// -----------------------------------------------
void setupConsole()
{
    ios::sync_with_stdio(false);
    // Do not untie cin from cout – we want prompts to be visible.
    // cin.tie(nullptr);
}

// -----------------------------------------------
// Random score generator used for all containers
// -----------------------------------------------
void fillRandomScores(Person& p, int index)
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(1, 10);

    // Example names: Name1 Surname1, Name2 Surname2, ...
    p.setFirstName("Name" + to_string(index + 1));
    p.setSurname("Surname" + to_string(index + 1));

    vector<int> hw(15);
    for (int& x : hw) x = dist(gen);

    p.setHomeworkScores(hw);
    p.setExamScore(dist(gen));

    // For this project we use the average formula
    p.calculateFinalGradeAverage();
}

// -----------------------------------------------
// Predicates for passed / failed
// -----------------------------------------------
inline bool isPassed(const Person& p)
{
    return p.getFinalGrade() >= 5.0;
}

inline bool isFailed(const Person& p)
{
    return !isPassed(p);
}

// -----------------------------------------------
// Helper: reserve capacity only for std::vector<Person>
// -----------------------------------------------
template <typename Container>
void maybeReserve(Container&, size_t)
{
    // default: do nothing
}

template <>
void maybeReserve<std::vector<Person>>(std::vector<Person>& c, size_t count)
{
    c.reserve(count);
}

// -----------------------------------------------
// Generate N students into any container type
// Uses std::vector, std::list or std::deque
// -----------------------------------------------
template <typename Container>
Container generateStudents(size_t count)
{
    Container students;
    maybeReserve(students, count); // only does something for vector<Person>

    for (size_t i = 0; i < count; ++i)
    {
        Person p;
        fillRandomScores(p, static_cast<int>(i));
        students.push_back(std::move(p));
    }

    return students;
}

// -----------------------------------------------
// Strategy 1: copy students to TWO new containers
//   - original students container is NOT changed
//   - passed + failed are created using std::copy_if
// -----------------------------------------------
template <typename Container>
void strategy1_splitCopy(const Container& students,
                         Container& passed,
                         Container& failed)
{
    passed.clear();
    failed.clear();

    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(passed), isPassed);

    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(failed), isFailed);
}

// -----------------------------------------------
// Strategy 2: move failed students OUT of base
//   - after this, "students" contains only PASSED
//   - "failed" contains FAILED students
//   - uses std::stable_partition to split
// -----------------------------------------------
template <typename Container>
void strategy2_moveFailed(Container& students,
                          Container& failed)
{
    failed.clear();

    // Partition: [passed | failed]
    typename Container::iterator partitionPoint =
        std::stable_partition(students.begin(), students.end(), isPassed);

    // Move failed students into separate container
    std::move(partitionPoint, students.end(),
              std::back_inserter(failed));

    // Shrink base container so it holds only passed students
    students.erase(partitionPoint, students.end());
}

// -----------------------------------------------
// Utility: measure execution time of a lambda
// -----------------------------------------------
template <typename Func>
long long measureMs(Func&& f)
{
    Clock::time_point start = Clock::now();
    f();
    Clock::time_point end   = Clock::now();
    return chrono::duration_cast<ms>(end - start).count();
}

// -----------------------------------------------
// Run tests for ONE container type (vector/list/deque)
// -----------------------------------------------
template <typename Container>
void runTestsForContainer(const string& containerName)
{
    cout << "\n======================================\n";
    cout << "  " << containerName << " (Strategy 1 vs Strategy 2)\n";
    cout << "======================================\n";

    const size_t sizesArray[] = {1000, 10000, 100000};
    const size_t numSizes = sizeof(sizesArray) / sizeof(sizesArray[0]);

    for (size_t idx = 0; idx < numSizes; ++idx)
    {
        size_t n = sizesArray[idx];
        Container students;

        // 1) Generate students
        long long genTime = measureMs([&]() {
            students = generateStudents<Container>(n);
        });

        // 2) Strategy 1: copy to passed + failed
        Container passed1, failed1;
        long long strategy1Time = measureMs([&]() {
            strategy1_splitCopy(students, passed1, failed1);
        });

        // 3) Strategy 2: move failed out of base container
        Container students2 = students;   // fresh copy
        Container failed2;
        long long strategy2Time = measureMs([&]() {
            strategy2_moveFailed(students2, failed2);
        });

        // 4) Print results
        cout << "\n--- N = " << n << " students ---\n";
        cout << "Generate:    " << genTime       << " ms\n";
        cout << "Strategy 1:  " << strategy1Time << " ms  (copy -> passed + failed)\n";
        cout << "Strategy 2:  " << strategy2Time << " ms  (move failed, shrink base)\n";

        cout << "Sizes (Strategy 1): passed = " << passed1.size()
             << ", failed = " << failed1.size() << "\n";
        cout << "Sizes (Strategy 2): passed = " << students2.size()
             << ", failed = " << failed2.size() << "\n";
    }
}

// -----------------------------------------------
// Main menu for v1.0
// -----------------------------------------------
int main()
{
    setupConsole();

    cout << "=== STUDENT GRADING SYSTEM - v1.0 ===\n\n";
    cout << "This version compares two splitting strategies\n";
    cout << "for three containers: std::vector, std::list, std::deque.\n\n";
    cout << "1. Test std::vector\n";
    cout << "2. Test std::list\n";
    cout << "3. Test std::deque\n";
    cout << "4. Test ALL containers\n";
    cout << "Choice: ";
    cout.flush();   // make sure the prompt appears

    int choice = 0;
    if (!(cin >> choice))
    {
        cerr << "Invalid input. Exiting.\n";
        return 0;
    }

    try
    {
        if (choice == 1)
        {
            runTestsForContainer<std::vector<Person> >("std::vector<Person>");
        }
        else if (choice == 2)
        {
            runTestsForContainer<std::list<Person> >("std::list<Person>");
        }
        else if (choice == 3)
        {
            runTestsForContainer<std::deque<Person> >("std::deque<Person>");
        }
        else if (choice == 4)
        {
            runTestsForContainer<std::vector<Person> >("std::vector<Person>");
            runTestsForContainer<std::list<Person> >("std::list<Person>");
            runTestsForContainer<std::deque<Person> >("std::deque<Person>");
        }
        else
        {
            cout << "Unknown option. Exiting.\n";
            return 0;
        }
    }
    catch (const std::exception& ex)
    {
        cerr << "\nERROR: " << ex.what() << "\n";
    }

    cout << "\nProgram completed. Press Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
