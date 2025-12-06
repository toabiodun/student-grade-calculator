# Student Grade Calculator – Main Overview

This repository contains a multi-version C++ application for calculating student final grades based on homework results and exam scores.  
Each version of the project (v0.1, v0.2, v0.25) extends the functionality, improves efficiency, and applies different data structures.

---

## 📌 Final Grade Calculation Formula

The program supports two methods for computing the final grade:

### 1️⃣ Average Method

FinalGrade = 0.4 × Average(Homework) + 0.6 × Exam

### 2️⃣ Median Method

FinalGrade = 0.4 × Median(Homework) + 0.6 × Exam

---

## 📌 Project Versions Overview

### v0.1

- First working version  
- `Person` class implemented with Rule of Three  
- Manual input and file input  
- Homework median/average calculation  
- Sorting and formatted output  
- Random score generation added  

### v0.2

- Refactored code into header/source files  
- Added basic exception handling  
- Large dataset generation: 1,000 / 10,000 / 100,000 / 1,000,000 students  
- Performance measurement using `std::vector`  
- Division into “passed” and “failed” students  
- Output results saved to separate text files  

### v0.25

*(To be implemented next)*  

- Same functionality as v0.2 using `std::list` and `std::deque`  
- Performance measurements for all three containers  
- Comparison of results (vector vs list vs deque)

---

## 📌 Repository Structure

main/  → General information and formulas  
v0.1/  → First implementation of the program  
v0.2/  → Refactored version + vector performance tests  
v0.25/ → List & deque performance version  

---

## 📌 Author

Abiodun Tomiwa (`toabiodun`)
