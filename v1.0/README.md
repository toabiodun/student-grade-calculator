Student Grade Calculator – Version v1.0

Version v1.0 is the final optimized release of the Student Grade Calculator project.

It builds on:

v0.1 – basic version (manual/file input, median/average, formatted table)

v0.2 – refactored version, exception handling, performance tests using std::vector

v0.25 – additional performance tests using std::list and std::deque

In v1.0 we compare three container types:

std::vector

std::list

std::deque

and two strategies for splitting students into passed and failed groups.

The aim is to see how container choice and splitting strategy affect performance
when we work with large numbers of students.

What is new in v1.0

Compared to v0.25, this version adds:

Strategy 1: copy passed and failed students into two new containers

Strategy 2: move failed students into one container and shrink the base container

Use of standard algorithms from the <algorithm> library, such as:
std::copy_if, std::stable_partition, std::move

Performance comparison of all three containers (vector, list, deque)
under both strategies

Clean Makefile and CMakeLists.txt for easy building on different systems

This fulfills the Task 3 requirements.

Splitting strategies – idea
Strategy 1 – Copy to two new containers

We start with a base container called students.

We create two new containers of the same type:
passed and failed.

We copy elements into these containers based on a predicate:

isPassed(person) – returns true if final grade >= 5.0

isFailed(person) – returns true if final grade < 5.0

The original students container remains unchanged.

Advantages:

Simple to understand and implement.

No deletion from the base container.

Disadvantages:

Higher memory usage, because we keep three containers
(students, passed, failed).

Strategy 2 – Move failed and shrink the base container

We keep only one additional container: failed.

We reorder the base container using std::stable_partition,
so that all passed students are at the beginning and
all failed students are grouped at the end.

We move the failed students into the failed container.

We erase the moved elements from the base container.
After this step, students contains only passed students.

Advantages:

Much better memory usage, because we store data only in:
students (passed) and failed.

We do not keep three full copies of the data.

Disadvantages:

Erasing elements can be more expensive for some containers,
especially for std::vector.

What the program measures

For each container type:

std::vector<Person>

std::list<Person>

std::deque<Person>

and for each dataset size:

1000 students

10 000 students

100 000 students

the program measures and prints:

generation time (creating random students)

time for Strategy 1 (copy to passed/failed)

time for Strategy 2 (move failed + shrink base)

number of passed and failed students in each case

Example of text output (numbers are only an illustration):

--- N = 10000 students ---
Generate: 24 ms
Strategy 1: 11 ms
Strategy 2: 7 ms
Sizes after Strategy 1: passed = 5890, failed = 4110
Sizes after Strategy 2: passed = 5890, failed = 4110

These results can be compared with the vector-only
results from v0.2 and with the list/deque results from v0.25.

Project structure (v1.0 folder)

The v1.0 folder contains:

main.cpp – program entry point and performance tests

Person.cpp – implementation of the Person class (same as v0.25)

Person.h – definition of the Person class (same as v0.25)

README.md – this document

Makefile – for Unix-like systems (Linux, macOS, WSL)

CMakeLists.txt – for building with CMake on any OS

The Person class:

stores first name, surname, homework scores, exam score, final grade

implements the rule of three (copy constructor, assignment operator, destructor)

has functions to calculate final grade by average or median

overloads operators for input/output

overloads operator< for sorting by surname then first name

Building the program

Using Make (Linux / macOS / WSL):

Open a terminal in the v1.0 folder.

Run: make

This produces an executable named: student_grading_v10

Run the program:
./student_grading_v10

Using CMake (any OS):

Create a build directory (for example: build).

From inside v1.0, run:
mkdir build
cd build
cmake ..
cmake --build .

Run the executable from the build directory:
./student_grading_v10 (Linux / macOS)
student_grading_v10.exe (Windows)

Using the program

When you run the program, you will see a menu like:

1 – Test std::vector
2 – Test std::list
3 – Test std::deque
4 – Test ALL containers

After you choose an option, the program will:

generate random students for each dataset size

run Strategy 1 and Strategy 2

measure times using std::chrono

print performance results for each container and strategy

You can run multiple times to see how results change.

Relation to previous versions

v0.1 – first version, focused on:

reading data (manual and from file)

calculating final grade by average or median

formatted screen output

v0.2 – refactored version with:

cleaner class design

exception handling for file errors

performance tests using std::vector

v0.25 – extended version with:

performance tests using std::list and std::deque

splitting students into passed/failed (copy strategy)

v1.0 – final version:

adds Strategy 1 and Strategy 2

optimizes splitting using algorithms from <algorithm>

compares vector, list, deque

provides Makefile and CMakeLists.txt

summarized and documented results in this README

Author

Abiodun Tomiwa
GitHub: toabiodun
