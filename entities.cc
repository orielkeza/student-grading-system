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
    : table{DefaultBuckets}, studentCourseTable{} {}

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
    
    while(fileStd>>sStd) {
        if(sStd=="Student: ") {
            string key = sStd;
            string teacher;
            string course;
            double marks;
            if (sStd=="     Teacher: ") {
                teacher = sStd;
            } else if (sStd=="     Course: ") {
                course = sStd; 
            } else if (sStd=="     Marks: ") {
                marks = sStd;
            }

        insert(key, teacher, course, marks);
        }
    }
    fileStd.close();
    fileSC.close();
}

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
}

bool StudentRegHashTable::lookup(string key) const {
    const int slot = hash(key);
    Student* curStudent = table.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == key) {
            return true;
        }
        curStudent = curStudent->next;
    }
    return false;
}

void StudentRegHashTable::updateCourse(string oldCourseN, string newCourseN) {
    for(auto & [key, p] : studentCourseTable) {
        if(p->course==oldCourseN) {
            p->course=newCourseN;
        }
    }
}

void StudentRegHashTable::updateStudent(string oldStudentN, string newStudentN) {
    for(auto & [key, p] : studentCourseTable) {
        if(p->name==oldStudentN) {
            p->name=newStudentN;
        }
    }
}

void StudentRegHashTable::remove(string key) {
    if(lookup(key)){
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
    }
}

void StudentRegHashTable::printAll() {
    db.open("studentRegDB.txt", ios::out);
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
    db.close();
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
    const int slot = hash(key);
    Student* curStudent = table.at(slot);
    while(nullptr!=curStudent) {
        if(curStudent->name == key) {
            cout<<"     Student: "<<curStudent->name<<endl;
            cout<<"     Course: "<<curStudent->course<<endl;
            cout<<"     Teacher: "<<curStudent->teacher<<endl;
            cout<<"     Marks: "<<curStudent->marks<<endl;
        }
        curStudent = curStudent->next;
    }
}

int StudentRegHashTable::hash(string key) const {
    char firstLetter = tolower(key[0]);
    return firstLetter - 'a';
}

string StudentRegHashTable::hashStudentCourse (string studentName, string courseName) const {
    string combination = studentName + courseName;
    return combination;
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