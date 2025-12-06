# Student Grade Calculator – Version v0.1

This folder contains the first fully working version of the Student Grade Calculator project. Version v0.1 focuses on implementing the core functionality required by the assignment, including the Person class, Rule of Three, manual and file input, and final grade calculation using both average and median.

## Features Implemented in v0.1

Person Class (with Rule of Three):
- First name and surname
- Vector of homework scores
- Exam score
- Final grade
Methods:
- Constructors
- Copy constructor (Rule of Three)
- Copy assignment operator (Rule of Three)
- Destructor (Rule of Three)
- Overloaded input operator (operator>>)
- Overloaded output operator (operator<<)
- Final grade calculation using average or median

## Input Options

1) Manual Input:
- Name and surname
- Homework scores (unknown amount)
- Exam score

2) File Input:
- Reads a formatted text file
- Automatically skips header row
- Loads each student’s homework and exam scores

## Final Grade Calculation

Average Method:
FinalGrade = 0.4 × Average(Homework) + 0.6 × Exam

Median Method:
FinalGrade = 0.4 × Median(Homework) + 0.6 × Exam

## Output Examples

Example (Average only):

Name        Surname        Final (Avg.)
---------------------------------------
Name1       Surname1           8.45
Name2       Surname2           7.20

Example (Average and Median):

Name        Surname     Final (Avg.) | Final (Med.)
----------------------------------------------------
Name1       Surname1        8.45     |     8.00
Name2       Surname2        7.20     |     6.90

Students are sorted alphabetically and displayed in aligned columns.

## File Format (Input Data)

Input files follow this format:

Vardas Pavarde ND1 ND2 ND3 ... ND15 Egz.
Vardas1 Pavarde1 8 5 8 ... 5 3
Vardas2 Pavarde2 6 6 5 ... 5 7

The program expects:
- A header line (skipped automatically)
- Name and surname
- 15 homework scores (ND1–ND15)
- 1 exam score
- All values separated by spaces

## Available Data Files

- students10000.txt — 10,000 student records (included)
- students100000.txt — 100,000 records (may be provided separately)
- students1000000.txt — 1,000,000 records (too large for GitHub)

## Version History (v0.1)

- Initial release with core functionality
- Person class using Rule of Three
- Manual and file input supported
- Median and average grade calculations
- Random score generator added
- Sorting and formatted output

## Note About Large Files (For Lecturer)

GitHub does not allow files larger than 100 MB.  
students1000000.txt is approximately 120 MB, so it cannot be uploaded to GitHub.

All required datasets (10,000; 100,000; 1,000,000 students) are included in the ZIP file submitted to Moodle. The program supports all datasets through the “Read from file” option.

## Files in This Folder

- main.cpp — v0.1 program logic
- Person.cpp — class implementation
- Person.h — header file
- students10000.txt — sample dataset
- README.md — documentation for version v0.1

## Summary

Version v0.1 satisfies the initial assignment:
- Correct Person class with Rule of Three
- Median and average calculations
- Manual and file input
- Random data generator
- Sorted and formatted output

This prepares the foundation for v0.2 (performance testing with vector) and v0.25 (list and deque versions).

## Author
Abiodun Tomiwa (toabiodun)
