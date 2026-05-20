# Student Registration System (C++)

A C++ implementation of a student registration system built around a custom hash table. Students and teachers can be registered, courses can be created and linked to both, and marks can be assigned and retrieved. Data persists across sessions through a file-based storage system modeled loosely on how a database loads and saves records.

---

## Background

This project started as a way to ease the transition from C++ to Java and get more comfortable with d the mechanics of many-to-many relationships in data. Much harder in plain C++ than in SQL. I had to start over a lot and resturcture what I was doing often the better I understood different concepts. 

---

## Features

- Register students, teachers, and courses
- Link students to courses and teachers to courses (student-course is many-to-many; teacher-course is one-to-many per course)
- Assign and update marks per student per course
- Update names for students, teachers, and courses across all tables in one call
- Remove students, teachers, or courses (cascades to junction tables)
- Query views: a student's courses, a course's students, a teacher's courses, a course's teachers
- Generate a student report card showing course, marks, and teacher
- Load data from flat files on startup, save back on shutdown — so state carries over between sessions
- Interactive terminal menu with A/B/C navigation for Student, Teacher, and Course
- Duplicate name prevention at the menu level for students, teachers, and courses

---

## Tech Stack

- **C++17**
- **Google Test** for unit testing
- No external libraries beyond the standard library and GTest
- Flat `.txt` files used as a persistence layer (no database)

---

## Architecture

Everything lives in a single class, `StudentRegHashTable`, defined in `entities.h` and implemented in `entities.cc`.

### Data structures

There are three separate 26-bucket hash tables (one each for students, teachers, and courses), each implemented as a `std::vector` of linked list heads. The hash function is simple: take the first letter of the name, lowercase it, subtract `'a'`. So "Alice" goes in bucket 0, "Bob" in bucket 1, and so on. Collisions within a bucket are handled by chaining.

The many-to-many relationships are handled through two junction tables: `studentCourseTable` (student ↔ course) and `teacherCourseTable` (teacher ↔ course). These are `std::map<std::string, SC*>` and `std::map<std::string, TC*>` respectively. The key is a string concatenation (hash function) of the two names, e.g. `"AliceMath"` for Alice enrolled in Math. Marks are stored the same way in a `std::map<std::string, Marks*>`.

### File persistence

On construction, `loadDB()` is called, which reads from flat text files and populates the in-memory tables. On destruction, `printAll()` write everything back out. The intent is to behave like loading a web application's saved state where you process the file, make changes during the session, then overwrite on exit.

The main file is `all.txt`, which stores full records in the format:
```
Student: Alice Course: Math Marks: 95.0 Teacher: Charles
```
There are also separate files for each view: `studentlist.txt`, `courselist.txt`, `teacherlist.txt`, `studentcourses.txt`, `coursestudents.txt`, `teachercourses.txt`, `courseteachers.txt`, and `reportcard.txt`.

---

## Setup and Usage

### Requirements
- A C++17-compatible compiler (tested with `g++`)
- Google Test installed
- PowerShell terminal

### Running the app
```bash
g++ -std=c++17 terminalMenu.cc -o studentreg
./studentreg
```
The application runs as a USSD-style interactive terminal menu. Use A/B/C to navigate between Student, Teacher, and Course sections, and 0 to go back or exit.

<img width="311" height="220" alt="image" src="https://github.com/user-attachments/assets/6339e807-ebfe-4899-99c5-06e21de73249" />

### Running the tests
```bash
g++ -std=c++17 entitiesTest.cc entities.cc entities.h -lgtest -lgtest_main -pthread -o run_tests
./run_tests
```

<img width="573" height="74" alt="image" src="https://github.com/user-attachments/assets/d6efc36b-138a-4ef3-bc0d-5cb167c9fe47" />

---

## Testing

Tests are written using Google Test and live in `entitiesTest.cc`. They cover:

- Default bucket count
- Adding and checking existence for students, teachers, and courses
- Precondition enforcement (junction operations require both entities to exist first)
- Update operations for names and marks
- Remove operations and cascade behavior
- Collision handling (multiple entries in the same bucket)
- Smoke tests for all print/file-write functions (verify no crash)
- Destructor behavior on heap-allocated tables

---

## What I Learned / What I'd Do Differently

The biggest thing that took time was understanding how to model many-to-many relationships without a real database, and that keeping the junction data consistent when you update or delete from the primary tables requires explicit cascade logic. SQL handles that for you; in C++ you have to think through it yourself.

I also spent too long trying to design multiple files to draw from, then one unified file format before landing on the approach of having `all.txt` as the main source to load from and generating the specialized view files on demand.

I overcomplicated things but found that often times the simpler answer was usually the best way to go about it.

---

## Planned Improvements

- Add ID generation so names don't need to be globally unique
- Separate `entities.cc` into more focused files as the codebase grows
