# Student Grading System

A C++ application for calculating and managing student final grades based on homework assignments and exam scores.

## Basic Formula

The final grade is calculated using one of two methods:

**Average Method:**
\`\`\`
Final Grade = 0.4 × Average(Homework) + 0.6 × Exam
\`\`\`

**Median Method:**
\`\`\`
Final Grade = 0.4 × Median(Homework) + 0.6 × Exam
\`\`\`

## Features

- Read student data from files or manual input
- Calculate final grades using average or median
- Generate random homework and exam scores
- Sort students by name/surname
- Formatted output display
- Support for large datasets (10,000+, 100,000+, 1,000,000+ students)

## Compilation

\`\`\`bash
g++ -std=c++11 -o student_grading main.cpp Person.cpp
\`\`\`

## Usage

\`\`\`bash
./student_grading
\`\`\`

Follow the on-screen prompts to:
1. Choose input method (manual, file, or random generation)
2. Select calculation method (average or median)
3. View formatted results

## File Format

Input files should follow this format:

\`\`\`
Vardas      Pavarde    ND1   ND2   ND3   ...  ND15  Egz.
Vardas1     Pavarde1     8     5     8   ...     5     3
Vardas2     Pavarde2     6     6     5   ...     5     7
\`\`\`


The program expects:
- Header row with column names (automatically skipped)
- Student name and surname
- 15 homework scores (ND1-ND15)
- 1 exam score (Egz.)
- All values space-separated

## Available Data Files

- **students10000.txt** - 10,000 student records (included)
- **students100000.txt** - 100,000 student records (add if needed)
- **students1000000.txt** - 1,000,000 student records (add if needed)


## Version History

  - Initial release with core functionality
  - Person class implementation with rule of three
  - File reading and manual input
  - Average and median calculations
  - Random score generation
  - Formatted output with sorting
