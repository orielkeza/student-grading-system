#include <iostream>
#include <string>
#include "entities.h"
#include "entities.cc"

using namespace std;

void resetScreen() {
    system("cls");
}

//formatting

void pressEnter(){
    cout<<"\n0. Go Back\n";
    string input;
    getline(cin, input);
}

void title(string title){
    resetScreen();
    cout<<"\n"<<title<<"\n";
}

//student

void addStudentMenu(StudentRegHashTable& s) {
    title("Add Student");
    s.printStudentList();
    cout<<"Enter student name (no duplicates): ";
    string name;
    getline(cin, name);
    if((s.student_check(name))){
        cout<<"\n Student already exists, try again\n";
        pressEnter();
        return;
    }
    s.addStudent(name);
    cout<<"\nStudent "<<name<<" added\n";
    pressEnter();
    return;
}

void removeStudentMenu(StudentRegHashTable& s) {
    title("Remove Student");
    s.printStudentList();
    cout<<"Enter student name to remove: ";
    string name;
    getline(cin, name);
    if(!(s.student_check(name))){
        cout<<"Student "<<name<<" not found\n";
        pressEnter();
        return;
    }else {
        s.removeStudent(name);
        cout<<"\nStudent "<<name<<" removed\n";
    }
    pressEnter();
    return;
}

void updateStudentMenu(StudentRegHashTable& s) {
    title("Update Student");
    s.printStudentList();
    cout<<"Enter student name to change: ";
    string oldname;
    getline(cin, oldname);
    if(!(s.student_check(oldname))){
        cout<<"Student "<<oldname<<" not found\n";
        pressEnter();
        return;
    }else {
        cout<<"Enter new student name: ";
        string newname;
        getline(cin, newname);
        s.updateStudent(oldname, newname);
        cout<<"\nStudent name updated from "<<oldname<<" to "<<newname<<"\n";
    }
    pressEnter();
    return;
}

void enrollStudentMenu(StudentRegHashTable& s) {
    title("Enroll Student in Course");

    s.printStudentList();
    cout<<"Enter student name to enroll: ";
    string name;
    getline(cin, name);
    if(!(s.student_check(name))){
        cout<<"Student "<<name<<" not found\n";
        pressEnter();
        return;
    }

    s.printCourseList();
    cout<<"Enter course to enroll student in: ";
    string course;
    getline(cin, course);
    if(!(s.course_check(course))){
        cout<<"Course "<<name<<" not found\n";
        pressEnter();
        return;
    }

    s.addStudent_Course(name, course);
    cout<<"\nStudent "<<name<<" enrolled in course "<<course<<"\n";
    pressEnter();
    return;
}

void viewStudentCoursesMenu(StudentRegHashTable& s) {
    title("View Student Courses");
    s.printStudentList();
    cout<<"Enter student name to view courses: ";
    string name;
    getline(cin, name);
    if(!(s.student_check(name))){
        cout<<"Student "<<name<<" not found\n";
        pressEnter();
        return;
    }
    s.studentCourses(name);
    pressEnter();
    return;
}

void studentRCMenu(StudentRegHashTable& s) {
    title("Student Report Card");
    s.printStudentList();
    cout<<"Enter student name to view report card: ";
    string name;
    getline(cin, name);
    if(!(s.student_check(name))){
        cout<<"Student "<<name<<" not found\n";
        pressEnter();
        return;
    }
    s.studentRC(name);
    pressEnter();
    return;
}

void studentMenu(StudentRegHashTable& s){
    while(true){
        title("Student Menu");
        cout<<"\n0. Go Back\n";
        cout<<"1. Add student\n";
        cout<<"2. Remove student\n";
        cout<<"3. Update student name\n";
        cout<<"4. Enroll student in course\n";
        cout<<"5. View student courses\n";
        cout<<"6. View student report card\n";
        string pick;
        getline(cin, pick);
        if(pick=="0"){
            return;
        } else if(pick=="1") {
            addStudentMenu(s);
        } else if(pick=="2"){
            removeStudentMenu(s);
        } else if(pick=="3"){
            updateStudentMenu(s);
        } else if(pick=="4"){
            enrollStudentMenu(s);
        } else if(pick=="5"){
            viewStudentCoursesMenu(s);
        } else if(pick=="6"){
            studentRCMenu(s);
        } else if(pick=="7") {
            s.printStudentList();
        } else {
            cout<<"\nInvalid option, try again\n";
            pressEnter();
        }
    }
    return;
}

//teachers

void addTeacherMenu(StudentRegHashTable& s) {
    title("Add Teacher");
    s.printTeacherList();
    cout<<"Enter teacher name (no duplicates): ";
    string name;
    getline(cin, name);
    if((s.teacher_check(name))){
        cout<<"\n Teacher already exists, try again\n";
        pressEnter();
        return;
    }
    s.addTeacher(name);
    cout<<"\nTeacher "<<name<<" added\n";
    pressEnter();
    return;
}

void removeTeacherMenu(StudentRegHashTable& s) {
    title("Remove Teacher");
    s.printTeacherList();
    cout<<"Enter teacher name to remove: ";
    string name;
    getline(cin, name);
    if(!(s.teacher_check(name))){
        cout<<"Teacher "<<name<<" not found\n";
        pressEnter();
        return;
    }else {
        s.removeTeacher(name);
        cout<<"\nTeacher "<<name<<" removed\n";
    }
    pressEnter();
    return;
}

void updateTeacherMenu(StudentRegHashTable& s) {
    title("Update Teacher");
    s.printTeacherList();
    cout<<"Enter teacher name to change: ";
    string oldname;
    getline(cin, oldname);
    if(!(s.teacher_check(oldname))){
        cout<<"Teacher "<<oldname<<" not found\n";
        pressEnter();
        return;
    }else {
        cout<<"Enter new teacher name: ";
        string newname;
        getline(cin, newname);
        s.updateTeacher(oldname, newname);
        cout<<"\nTeacher name updated from "<<oldname<<" to "<<newname<<"\n";
    }
    pressEnter();
    return;
}

void enrollTeacherMenu(StudentRegHashTable& s) {
    title("Add Teacher to Course");

    s.printTeacherList();
    cout<<"Enter teacher name to enroll: ";
    string name;
    getline(cin, name);
    if(!(s.teacher_check(name))){
        cout<<"Teacher "<<name<<" not found\n";
        pressEnter();
        return;
    }

    s.printCourseList();
    cout<<"Enter course to add teacher to: ";
    string course;
    getline(cin, course);
    if(!(s.course_check(course))){
        cout<<"Course "<<name<<" not found\n";
        pressEnter();
        return;
    }

    s.addTeacher_Course(name, course);
    cout<<"\nTeacher "<<name<<" enrolled in course "<<course<<"\n";
    pressEnter();
    return;
}

void viewTeacherCoursesMenu(StudentRegHashTable& s) {
    title("View Teacher Courses");
    s.printTeacherList();
    cout<<"Enter teacher name to view courses: ";
    string name;
    getline(cin, name);
    if(!(s.teacher_check(name))){
        cout<<"Teacher "<<name<<" not found\n";
        pressEnter();
        return;
    }
    s.teacherCourses(name);
    pressEnter();
    return;
}

void assignMarksMenu(StudentRegHashTable& s) {
    title("Assign Marks");
    s.printTeacherList();
    cout<<"Enter teacher name: ";
    string name;
    getline(cin, name);
    s.printCourseList();
    cout<<"Enter course name: ";
    string course;
    getline(cin, course);
    s.printStudentList();
    cout<<"Enter student name: ";
    string student;
    getline(cin, student);
    if(!(s.teacher_course_check(name,course))){
        cout<<"Teacher "<<name<<" not assigned to"<<course<<"\n";
        pressEnter();
        return;
    }
    if(!(s.student_course_check(student,course))){
        cout<<"Student "<<name<<" not enrolled in"<<course<<"\n";
        pressEnter();
        return;
    }
    cout<<"Add marks for student "<<student<<" in course "<<course<<" as teacher "<<name<<"\n";
    string marksString;
    double marks;
    getline(cin, marksString);
    marks=stod(marksString);
    s.addMarks(marks, student, course);
    pressEnter();
    return;
}

void changeMarksMenu(StudentRegHashTable& s) {
    title("Change Marks");
    s.printTeacherList();
    cout<<"Enter teacher name: ";
    string name;
    getline(cin, name);
    s.printCourseList();
    cout<<"Enter course name: ";
    string course;
    getline(cin, course);
    s.printStudentList();
    cout<<"Enter student name: ";
    string student;
    getline(cin, student);
    if(!(s.teacher_course_check(name,course))){
        cout<<"Teacher "<<name<<" not assigned to"<<course<<"\n";
        pressEnter();
        return;
    }
    if(!(s.student_course_check(student,course))){
        cout<<"Student "<<name<<" not enrolled in"<<course<<"\n";
        pressEnter();
        return;
    }
    cout<<"Change marks for student "<<student<<" in course "<<course<<" as teacher "<<name<<"\n";
    string marksString;
    double newmarks;
    getline(cin, marksString);
    newmarks=stod(marksString);
    s.updateMarks(student, course, newmarks);
    pressEnter();
    return;
}

void teacherMenu(StudentRegHashTable& s){
    while(true){
        title("Teacher Menu");
        cout<<"\n0. Go Back\n";
        cout<<"1. Add teacher\n";
        cout<<"2. Remove teacher\n";
        cout<<"3. Update teacher name\n";
        cout<<"4. Add teacher to course\n";
        cout<<"5. View teacher courses\n";
        cout<<"6. View student report card\n";
        cout<<"7. Assign marks\n";
        cout<<"8. Change Marks\n";
        cout<<"9. View all teachers\n";
        string pick;
        getline(cin, pick);
        if(pick=="0"){
            return;
        } else if(pick=="1") {
            addTeacherMenu(s);
        } else if(pick=="2"){
            removeTeacherMenu(s);
        } else if(pick=="3"){
            updateTeacherMenu(s);
        } else if(pick=="4"){
            enrollTeacherMenu(s);
        } else if(pick=="5"){
            viewTeacherCoursesMenu(s);
        } else if(pick=="6"){
            studentRCMenu(s);
        } else if(pick=="7"){
            assignMarksMenu(s);
        } else if(pick=="8"){
            changeMarksMenu(s);
        } else if(pick=="9"){
            s.printTeacherList();
        } else {
            cout<<"\nInvalid option, try again\n";
            pressEnter();
        }
    }
    return;
}

//course

void addCourseMenu(StudentRegHashTable& s) {
    title("Add Course");
    s.printCourseList();
    cout<<"Enter course name (no duplicates): ";
    string name;
    getline(cin, name);
    if((s.course_check(name))){
        cout<<"\n Course already exists, try again\n";
        pressEnter();
        return;
    }
    s.addCourse(name);
    cout<<"\nCourse "<<name<<" added\n";
    pressEnter();
    return;
}

void removeCourseMenu(StudentRegHashTable& s) {
    title("Remove Course");
    s.printCourseList();
    cout<<"Enter course name to remove: ";
    string name;
    getline(cin, name);
    if(!(s.course_check(name))){
        cout<<"Course "<<name<<" not found\n";
        pressEnter();
        return;
    }else {
        s.removeCourse(name);
        cout<<"\nCourse "<<name<<" removed\n";
    }
    pressEnter();
    return;
}

void updateCourseMenu(StudentRegHashTable& s) {
    title("Update Course");
    s.printCourseList();
    cout<<"Enter course name to change: ";
    string oldname;
    getline(cin, oldname);
    if(!(s.course_check(oldname))){
        cout<<"Course "<<oldname<<" not found\n";
        pressEnter();
        return;
    }else {
        cout<<"Enter new course name: ";
        string newname;
        getline(cin, newname);
        s.updateCourse(oldname, newname);
        cout<<"\nCourse name updated from "<<oldname<<" to "<<newname<<"\n";
    }
    pressEnter();
    return;
}

void viewCourseStudentsMenu(StudentRegHashTable& s) {
    title("View Course Students");
    s.printCourseList();
    cout<<"Enter course name to view students: ";
    string name;
    getline(cin, name);
    if(!(s.course_check(name))){
        cout<<"Course "<<name<<" not found\n";
        pressEnter();
        return;
    }
    s.courseStudents(name);
    pressEnter();
    return;
}

void viewCourseTeachersMenu(StudentRegHashTable& s) {
    title("View Course Teachers");
    s.printCourseList();
    cout<<"Enter course name to view teachers: ";
    string name;
    getline(cin, name);
    if(!(s.course_check(name))){
        cout<<"Course "<<name<<" not found\n";
        pressEnter();
        return;
    }
    s.courseTeachers(name);
    pressEnter();
    return;
}

void courseMenu(StudentRegHashTable& s){
    while(true){
        title("Course Menu");
        cout<<"\n0. Go Back\n";
        cout<<"1. Add course\n";
        cout<<"2. Remove course\n";
        cout<<"3. Update course name\n";
        cout<<"4. View teachers for course\n";
        cout<<"5. View students in course\n";
        cout<<"6. View all courses \n";
        string pick;
        getline(cin, pick);
        if(pick=="0"){
            return;
        } else if(pick=="1") {
            addCourseMenu(s);
        } else if(pick=="2"){
            removeCourseMenu(s);
        } else if(pick=="3"){
            updateCourseMenu(s);
        } else if(pick=="4"){
            viewCourseTeachersMenu(s);
        } else if(pick=="5"){
            viewCourseStudentsMenu(s);
        } else if(pick=="6"){
            s.printCourseList();
        } else {
            cout<<"\nInvalid option, try again\n";
            pressEnter();
        }
    }
    return;
}

int main() {
    StudentRegHashTable s;
    while (true){
        title("Student Registration System");
        cout<<"A: Student\n";
        cout<<"B:Teacher\n";
        cout<<"C: Course\n";
        cout<<"0: Exit\n";
        string pick;
        getline(cin, pick);
        if(pick=="A" || pick=="a"){
            studentMenu(s);
        } else if(pick=="B" || pick=="b"){
            teacherMenu(s);
        } else if(pick=="C" || pick=="c"){
            courseMenu(s);
        } else if(pick=="0"){
            cout<<"exiting...\n";
            return 0;
        } else {
            cout<<"Invalid option, try again \n";
            pressEnter();
        }
    }
    
    return 0;
}
