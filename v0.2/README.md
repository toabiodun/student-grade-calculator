# Version 0.2 - Preparing folder

Student Grade Calculator – Version v0.2
Version v0.2 is a refactored and extended version of the Student Grade Calculator. It focuses on cleaner code structure, exception handling, and performance testing using std::vector on large datasets.

What is new in v0.2
Compared to v0.1, this version adds:

Refactored project structure (separate .h and .cpp files)

Exception handling for file errors and invalid input

Automatic generation of large random datasets

Performance testing using std::vector

Splitting students into “passed” and “failed” groups

Writing the results into separate output files for each dataset size

Project Structure (v0.2 folder)
The v0.2 folder contains at minimum:
main.cpp
Person.cpp
Person.h
README.md

During performance tests the program generates additional text files:
students_1000_passed_vector.txt
students_1000_failed_vector.txt
students_10000_passed_vector.txt
students_10000_failed_vector.txt
students_100000_passed_vector.txt
students_100000_failed_vector.txt
students_1000000_passed_vector.txt
students_1000000_failed_vector.txt

These files contain the passed and failed students for each dataset size.

How the Program Works in v0.2

The user starts the program and sees a menu.

Option 4 runs the performance test using std::vector.

The program automatically generates random students for datasets of size 1000, 10000, 100000, and 1000000.

For each dataset size, the program:

Generates names (Name1 Surname1, Name2 Surname2, etc.)

Generates homework and exam scores (1–10)

Calculates the final grade using the average formula: FinalGrade = 0.4 * average(homework) + 0.6 * exam

Sorts all students

Splits them into two groups: passed (final >= 5.0) and failed (final < 5.0)

Writes results into output files named students_N_passed_vector.txt and students_N_failed_vector.txt

Measures the time required for generation, sorting, splitting, and writing

Example performance output:
Testing with 1000 students: Generate 2 ms, Sort 0 ms, Split 0 ms, Write 2 ms
Testing with 10000 students: Generate 20 ms, Sort 5 ms, Split 1 ms, Write 9 ms
Testing with 100000 students: Generate 180 ms, Sort 73 ms, Split 20 ms, Write 78 ms
Testing with 1000000 students: Generate 1797 ms, Sort 1021 ms, Split 305 ms, Write 785 ms

Exception Handling
v0.2 includes exception handling for safer execution.

If an input file cannot be opened, an error message is displayed

Invalid data lines are skipped or handled safely

Vector operations avoid out-of-range errors

User menu input is validated

Compilation (v0.2)
To compile this version:
g++ -std=c++11 -O2 -o student_grading_v02 main.cpp Person.cpp

Running the Program
On Windows PowerShell (inside v0.2 folder):
.\student_grading_v02.exe
Choose option 4 to run the performance test.

Performance Output Used in README
=== VECTOR PERFORMANCE TEST (v0.2) ===
--- Testing with 1000 students ---
Generate: 2 ms
Sort: 0 ms
Split: 0 ms
Write: 2 ms
Passed: 584, Failed: 416

--- Testing with 10000 students ---
Generate: 20 ms
Sort: 5 ms
Split: 1 ms
Write: 9 ms
Passed: 5850, Failed: 4150

--- Testing with 100000 students ---
Generate: 180 ms
Sort: 73 ms
Split: 20 ms
Write: 78 ms
Passed: 58200, Failed: 41800

--- Testing with 1000000 students ---
Generate: 1797 ms
Sort: 1021 ms
Split: 305 ms
Write: 785 ms
Passed: 584838, Failed: 415162

Large Dataset Notice
The file students_1000000_passed_vector.txt exceeds GitHub’s upload limit (25 MB) and cannot be uploaded to the repository.
It is included in the ZIP archive submitted to Moodle as required.
The program successfully generated all datasets, including the largest one.

Relation to Other Versions
v0.1: basic implementation (manual input, file input, average/median, formatting)
v0.2: refactored version + exception handling + vector performance tests
v0.25: same logic as v0.2 but implemented using std::list and std::deque

Files in this Folder
main.cpp
Person.cpp
Person.h
README.md

Author
Abiodun Tomiwa (toabiodun)
