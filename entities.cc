#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cctype>
#include <algorithm>
#include <map>
#include "entities.h"

//no duplicate student name, teacher names, course names (id generation for unique identification to be added later)
//students can add courses (which automatically adds them to the teacher's student list) and view what courses and teachers they have added
//maybe course can be something only a teacher owns, they have their list of courses and each list of courses has a list of students

using namespace std;

StudentRegHashTable::StudentRegHashTable()
    : studentCourseTable{}, teacherCourseTable{}, studentlist{DefaultBuckets}, courselist{DefaultBuckets}, markslist{}, teacherlist{DefaultBuckets} {
        loadDB(db, junctiondb);
    }

StudentRegHashTable::~StudentRegHashTable() {
    printAll();
    printJunctionAll();
    
    for(int i=0; i<table.size(); i++) {
        Student* p = table.at(i);
        while (nullptr!=p) {
            Student* temp =p;
            p = p->next;
            delete temp;
        }
    }
    for(auto & [key, p] : studentCourseTable) {
        delete p;
    }
}
//the file being ingested must follow the format
void StudentRegHashTable::loadDB(fstream& fileStd, fstream& fileSC) { //aka db and junctiondb
    string sStd;
    string sSC;
    fileStd.open("studentRegDB.txt", ios::out);
    fileSC.open("junctionDB.txt", ios::out);
    if (!fileStd) {
        cerr<< "Cannot open standard file."<<endl;
        return;
    }
    if (!fileSC) {
        cerr<<"Cannot open student course junction file."<<endl;
        return;
    }
        //create a new student using the student name but stop once you reach "     Course: " (stop everytime you land on five spaces)
        //then write course, teacher, marks
        //remember to add to the second hash table as well
        //this is to just add the stuff to the tables and then proceed to add as normal
        //when i close the application (dtor) rewrite the files with the new info from both
        string key = sStd;
        string teacher;
        string course;
        double marks=0.0;
    while(fileStd>>sStd) {
        if(sStd=="Student:") {
            fileStd>>key;
        } else if (sStd=="Teacher:") {
            fileStd>>teacher;
        } else if (sStd=="Course:") {
            fileStd>>course; 
        } else if (sStd=="Marks:") {
            string marksString;
            fileStd>>marksString;
            marks=stod(marksString);
            insert(key, teacher, course, marks);
        }
    }

    fileStd.close();
    fileSC.close();
}

void StudentRegHashTable::addStudent(string student) {
    const int slot = hash(student);
    Student* newStudent = new Student;
    newStudent->name = student;
    newStudent->next =studentlist.at(slot);
    studentlist.at(slot)=newStudent;
}

void StudentRegHashTable::addTeacher(string teacher) {
    const int slot = hash(teacher);
    Teacher* newTeacher = new Teacher;
    newTeacher->name = teacher;
    newTeacher->next = teacherlist.at(slot);
    teacherlist.at(slot)=newTeacher;
}

void StudentRegHashTable::addCourse(string course) {
    const int slot = hashCourse(course);
    Course* newCourse = new Course;
    newCourse->courseName = course;
    newCourse->next =courselist.at(slot);
    courselist.at(slot)=newCourse;
}

bool StudentRegHashTable::student_check(string student) const { //to make sure student and course exist
//look through studentlist
    const int slot = hash(student);
    Student* curStudent = studentlist.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == student) {
            return true;
        }
        curStudent = curStudent->next;
    }
    return false;
}

bool StudentRegHashTable::teacher_check(string teacher) const { //to make sure student and course exist
//look through teacherlist
    const int slot = hash(teacher);
    Teacher* curTeacher = teacherlist.at(slot);
    while(nullptr!=curTeacher) {
        if(curTeacher->name == teacher) {
            return true;
        }
        curTeacher = curTeacher->next;
    }
    return false;
}

bool StudentRegHashTable::course_check(string course) const { //to make sure student and course exist
//look through courselist
    const int slot = hash(course);
    Course* curCourse = courselist.at(slot);
    while(nullptr!=curCourse) {
        if(curCourse->courseName == course) {
            return true;
        }
        curCourse = curCourse->next;
    }
    return false;
}

bool StudentRegHashTable::student_course_check(string student, string course) const { //to make sure student and course exist
//look through studentlist and courselist
    if((true==student_check(student))&&(true==course_check(course))){
        return true;
    }
    return false;
}

bool StudentRegHashTable::teacher_course_check(string teacher, string course) const { //to make sure student and course exist
//look through studentlist and courselist
    if((true==teacher_check(teacher))&&(true==course_check(course))){
        return true;
    }
    return false;
}

void StudentRegHashTable::addStudent_Course(string student, string course){
    if(!(student_course_check(student,course))) return;

    const string scSlot = hashStudentCourse(student, course);
    SC* scNewStudent = new SC;
    scNewStudent->name = student;
    scNewStudent->course = course;
    scNewStudent->next = studentCourseTable[scSlot]; //not at. bc it wasn't alr initiallized
    studentCourseTable[scSlot] = scNewStudent;
}
//needs to be updated for the teacher course connection
void StudentRegHashTable::addTeacher_Course(string teacher, string course){
    if(!(teacher_course_check(teacher,course))) return;

    const string tcSlot = hashStudentCourse(teacher, course);
    TC* tcNewTeacher = new TC;
    tcNewTeacher->name = teacher;
    tcNewTeacher->course = course;
    tcNewTeacher->next = teacherCourseTable[tcSlot]; //not at. bc it wasn't alr initiallized
    teacherCourseTable[tcSlot] = tcNewTeacher;
}

void StudentRegHashTable::addMarks(double marks, string student, string course) {
    if(!(student_course_check(student,course))) return;

    const string MSlot = hashMarks(student, course, marks);
    Marks* newMarks = new Marks;
    newMarks->marks = marks;
    newMarks->studentName = student;
    newMarks->courseName = course;
    newMarks->next = markslist[MSlot]; //not at. bc it wasn't alr initiallized
    markslist[MSlot] = newMarks;  
}
/*
void StudentRegHashTable::insert(string key, string teacher, string course, double marks) {
    const int slot = hash(key);
    Student* newStudent = new Student;
    newStudent->name = key;
    newStudent->teacher = teacher;
    newStudent->course = course;
    newStudent->marks = marks;
    newStudent->next = table.at(slot);
    table.at(slot) = newStudent;

    const string scSlot = hashStudentCourse(key, course);
    SC* scNewStudent = new SC;
    scNewStudent->name = key;
    scNewStudent->course = course;
    scNewStudent->next = studentCourseTable[scSlot]; //not at. bc it wasn't alr initiallized
    studentCourseTable[scSlot] = scNewStudent;
} */

bool StudentRegHashTable::lookup(string key) const {
    /*const int slot = hash(key);
    Student* curStudent = table.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == key) {
            return true;
        }
        curStudent = curStudent->next;
    }
    return false; */
}

void StudentRegHashTable::updateCourse(string oldCourseN, string newCourseN) {
    for(auto & [key, p] : studentCourseTable) {
        if(p->course==oldCourseN) {
            p->course=newCourseN;
        }
    }

    for(auto & [key, p] : teacherCourseTable) {
        if(p->course==oldCourseN) {
            p->course=newCourseN;
        }
    }   

    if(course_check){
        const int slot = hash(oldCourseN);
        Course* curCourse = courselist.at(slot);
        while(nullptr!=curCourse) {
            if(curCourse->courseName == oldCourseN) {
               curCourse->courseName=newCourseN;
            }
            curCourse = curCourse->next;
        }
    }
}

void StudentRegHashTable::updateStudent(string oldStudentN, string newStudentN) {
    for(auto & [key, p] : studentCourseTable) {
        if(p->name==oldStudentN) {
            p->name=newStudentN;
        }
    }
    const int slot = hash(oldStudentN);
    Student* curStudent = studentlist.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == oldStudentN) {
            curStudent->name = newStudentN;
        }
        curStudent = curStudent->next;
    }
}

void StudentRegHashTable::remove(string key) {
    /*if(lookup(key)){
        int i = hash(key);
        Student* p = table[i];
        Student* prev = nullptr;

        while(p!=nullptr) {
            if(p->name==key) {
                if(prev==0){
                    table[i]=p->next;
                } else {
                    prev->next=p->next;
                }
            delete p;
            return;
            }
        prev=p;
        p=p->next;
        }

        for(auto & [key, p] : studentCourseTable) {
        if(p->name==key) {
            delete p;
        }
        }
    } */
}

void StudentRegHashTable::printAll() {
    /*db.open("studentRegDB.txt", ios::out);
    if(!db) {
        cerr<<"Error: Could not open file."<<endl;
        return;
    }
    for(int i=0; i<table.size(); i++) {
        Student* curStudent = table.at(i);
        while(nullptr!=curStudent) {
            db<<"Student: "<<curStudent->name<<endl;
            db<<"     Course: "<<curStudent->course<<endl;
            db<<"     Teacher: "<<curStudent->teacher<<endl;
            db<<"     Marks: "<<curStudent->marks<<endl;
            curStudent = curStudent->next;
        }
    }
    db.close();*/
}

void StudentRegHashTable::printJunctionAll() {
    junctiondb.open("junctionDB.txt", ios::out);
    if(!junctiondb) {
        cerr<<"Error: Could not open file."<<endl;
        return;
    }
    for(auto & [key, p] : studentCourseTable) {  
        junctiondb<<"Student: "<<p->name<<endl;
        junctiondb<<"     Course: "<<p->course<<endl;
    }
    junctiondb.close();
}

void StudentRegHashTable::printStudentList(string courseName) {
    dbStudentList.open("studentList.txt", ios::out);
    dbStudentList<<courseName<<"'s Students:"<<endl;
    for(auto & [key, p] : studentCourseTable) {
        if(p->name==courseName) {
            dbStudentList<<p->course<<endl;      
        }
    }    
}

void StudentRegHashTable::printCourseList(string studentName) {
    dbCourseList.open("courseList.txt", ios::out);
    dbCourseList<<studentName<<"'s Courses:"<<endl;
    for(auto & [key, p] : studentCourseTable) {
        if(p->name==studentName) {
            dbCourseList<<p->course<<endl;      
        }
    }
}

void StudentRegHashTable::printOne(string key) {
    /*const int slot = hash(key);
    Student* curStudent = table.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == key) {
            cout<<"     Student: "<<curStudent->name<<endl;
            cout<<"     Course: "<<curStudent->course<<endl;
            cout<<"     Teacher: "<<curStudent->teacher<<endl;
            cout<<"     Marks: "<<curStudent->marks<<endl;
        }
        curStudent = curStudent->next;
    } */
}

int StudentRegHashTable::hash(string key) const { //needs to be updated but this is my studentlist
    char firstLetter = tolower(key[0]);
    return firstLetter - 'a';
}

string StudentRegHashTable::hashStudentCourse (string studentName, string courseName) const { //student_course
    string combination = studentName + courseName;
    return combination;
}

string StudentRegHashTable::hashMarks (string studentName, string courseName, double marks) const {//marks_student_course
    string combination = studentName + courseName;
    string marksString=to_string(marks);
    return combination+marksString;
}

int StudentRegHashTable::hashCourse (string course) const{ //courselist
    return (hash(course));
}

string StudentRegHashTable::hashTC (string teacher, string course) const { //teacher_course
    return (hashStudentCourse(teacher, course));
}


/*
 // Person Class
Person::Person (string name)
    : name {name} {}

Person::~Person () {}

string Person::getName() const {
    return name;
}

void Person::addCourse (string course) {
    Course* c = new Course {course};
    courseList.push_back(c);
    if(!courseFile){
        cerr<<"Error: Could not open file."<<endl;
    }
    courseFile<<course<<endl;
}

void Person::allCourses() {
    if(!courseFile){
        cerr<<"Error: Could not open file."<<endl;
    }
    string line;
    while(getline(courseFile, line)){
        cout<<line<<endl;
    }
}

//Teacher Class
Teacher::Teacher (string name)
    :Person {name} {}

Teacher::~Teacher () {}

//to be fixed later
/*void Teacher::addMarks (string courseName, string markName, int mark){
    Marks* c = new Marks {markName, mark};
    marksList.open(marks.txt);
    marksList.push_back(c);
    if(!marksFile){
        cerr<<"Error: Could not open file."<<endl;
    }
    marksFile<<mark<<endl;
    marksList.close();
}*/

/*void Teacher::changeMark (string courseName, int index, int newMark){
    for(int i=0; i<index; i++) {

    }

}

void Teacher::allStudents (){
    for(const auto & s: studentList){
        cout << s << endl;
    }
}

//Student Class
Student::Student (string name)
    :Person {name} {}

Student::~Student () {}

void Student::allTeachers(){
    for(const auto & s : teacherList){
        cout << s << endl;
    }
}
*/