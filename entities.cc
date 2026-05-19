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
        loadDB();
    }

StudentRegHashTable::~StudentRegHashTable() {
    printAll();
    //printJunctionAll();

    for(int i=0; i<studentlist.size(); i++) {
        Student* p = studentlist.at(i);
        while (nullptr!=p) {
            Student* temp =p;
            p = p->next;
            delete temp;
        }
    }

    for(int i=0; i<teacherlist.size(); i++) {
        Teacher* p = teacherlist.at(i);
        while (nullptr!=p) {
            Teacher* temp =p;
            p = p->next;
            delete temp;
        }
    }

    for(int i=0; i<courselist.size(); i++) {
        Course* p = courselist.at(i);
        while (nullptr!=p) {
            Course* temp =p;
            p = p->next;
            delete temp;
        }
    }

    for(auto & [key, p] : studentCourseTable) {
        delete p;
    }

    for(auto & [key, p] : teacherCourseTable) {
        delete p;
    }

    for(auto & [key, p] : markslist) {
        delete p;
    }
}

//the file being ingested must follow the format
//load each of the files seperately and then call all the seperate ones when you call loadDB

        //create a new student using the student name but stop once you reach "     Course: " (stop everytime you land on five spaces)
        //then write course, teacher, marks
        //remember to add to the second hash table as well
        //this is to just add the stuff to the tables and then proceed to add as normal
        //when i close the application (dtor) rewrite the files with the new info from both

void StudentRegHashTable::loadCS(){
    string cs;
    dbcoursestudents.open("coursestudents.txt", ios::in);
    if (!dbcoursestudents) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbcoursestudents>>cs) {
        dbcoursestudents>>cs;
        string course = cs;
        if(cs=="'s Students:") {
            dbcoursestudents>>cs;
            addStudent_Course(cs,course);
        }

    }
    dbcoursestudents.close();
}
void StudentRegHashTable::loadSC(){
    string sc;
    dbstudentcourses.open("studentcourses.txt", ios::in);
    if (!dbstudentcourses) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbstudentcourses>>sc) {
        dbstudentcourses>>sc;
        string student = sc;
        if(sc=="Courses:") {
            dbstudentcourses>>sc;
            //addStudent_Course(student,sc);
        }

    }
    dbstudentcourses.close();
}
void StudentRegHashTable::loadTC(){
    string tc;
    dbteachercourses.open("teachercourses.txt", ios::in);
    if (!dbteachercourses) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbteachercourses>>tc) {
        dbteachercourses>>tc;
        string teacher = tc;
        if(tc=="Courses:") {
            dbteachercourses>>tc;
            addTeacher_Course(teacher, tc);
        }

    }
    dbteachercourses.close();
}
void StudentRegHashTable::loadCT(){
    string ct;
    dbcourseteachers.open("courseteachers.txt", ios::in);
    if (!dbcourseteachers) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbcourseteachers>>ct) {
        dbcourseteachers>>ct;
        string course = ct;
        if(ct=="Teachers:") {
            dbcourseteachers>>ct;
            //addTeacher_Course(ct, course);
        }

    }
    dbcourseteachers.close();
}
void StudentRegHashTable::loadA(){ //come back to this
    string a;
    dball.open("all.txt", ios::in);
    if (!dball) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dball>>a) {
        string student;
        string course;
        string teacher;
        double marks= 0.0;
        if(a=="Student:") {
            dball>>a;
            student=a;
            addStudent(student);
            dball>>a;
            if(a=="Course:") { 
                dball>>a;
                course=a;
                addCourse(course);
                addStudent_Course(student, course);
                dball>>a;
                if(a=="Marks:"){
                    dball>>a;
                    marks=stod(a);
                    addMarks(marks, student, course);
                    dball>>a;
                    if(a=="Teacher:"){
                        dball>>a;
                        teacher=a;
                        addTeacher(teacher);
                        addTeacher_Course(teacher, course);
                    }
                }
            }

        }
    }
    dball.close();
}
void StudentRegHashTable::loadSL(){
    string sl;
    dbstudentlist.open("studentlist.txt", ios::in);
    if (!dbstudentlist) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbstudentlist>>sl) {
        if(sl=="List of all the students:") {
            dbstudentlist>>sl;
            addStudent(sl);
        }

    }
    dbstudentlist.close();
}
void StudentRegHashTable::loadCL(){
    string cl;
    dbcourselist.open("courselist.txt", ios::in);
    if (!dbcourselist) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbcourselist>>cl) {
        if(cl=="List of all the courses:") {
            dbcourselist>>cl;
            addCourse(cl);
        }

    }
    dbcourselist.close();
}
void StudentRegHashTable::loadTL(){
    string tl;
    dbteacherlist.open("teacherlist.txt", ios::in);
    if (!dbteacherlist) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    while(dbteacherlist>>tl) {
        if(tl=="List of all the teachers:") {
            dbteacherlist>>tl;
            addTeacher(tl);
        }

    }
    dbteacherlist.close();
}
void StudentRegHashTable::loadRC(){ //need to come back and fix this
    string rc;
    dbreportcard.open("reportcard.txt", ios::in);
    if (!dbreportcard) {
        cerr<< "Cannot open file."<<endl;
        return;
    }

    string student;
    string course;
    string teacher;
    double marks = 0.0;
    while(dbreportcard>>rc) {
        dbreportcard>>rc;
        student=rc;
        addStudent(student);
        dbreportcard>>rc;
        if(rc=="Course:") {
            dbreportcard>>rc;
            course=rc;
            addCourse(course);
            addStudent_Course(student, course);
            dbreportcard>>rc;
            if(rc=="Marks:"){
                dbreportcard>>rc;
                marks=stod(rc);
                addMarks(marks, student, course);
                dbreportcard>>rc;
                if(rc=="Teacher:"){
                    dbreportcard>>rc;
                    teacher=rc;
                    addTeacher(teacher);
                    addTeacher_Course(teacher, course);
                }
            }
        }

    }
    dbreportcard.close();
}

void StudentRegHashTable::loadDB() { 
    
    //loading order matters, you add bfr you link
                
     loadSL();
     loadCL();
     loadTL();
     //loadCS();
     //loadSC();
     //loadTC();
     //loadCT();
     //loadRC();
     loadA();
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

    const string tcSlot = hashTC(teacher, course);
    TC* tcNewTeacher = new TC;
    tcNewTeacher->name = teacher;
    tcNewTeacher->course = course;
    tcNewTeacher->next = teacherCourseTable[tcSlot]; //not at. bc it wasn't alr initiallized
    teacherCourseTable[tcSlot] = tcNewTeacher;
}

void StudentRegHashTable::addMarks(double marks, string student, string course) {
    if(!(student_course_check(student,course))) return;

    const string MSlot = hashStudentCourse(student, course);
    Marks* newMarks = new Marks;
    newMarks->marks = marks;
    newMarks->studentName = student;
    newMarks->courseName = course;
    newMarks->next = markslist[MSlot]; //not at. bc it wasn't alr initiallized
    markslist[MSlot] = newMarks;  
}

void StudentRegHashTable::updateCourse(string oldCourseN, string newCourseN) {
    if(course_check(oldCourseN)){
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

        const int slot = hash(oldCourseN);
        Course* curCourse = courselist.at(slot);
        while(nullptr!=curCourse) {
            if(curCourse->courseName == oldCourseN) {
               curCourse->courseName=newCourseN;
            }
            curCourse = curCourse->next;
        }
        for(auto & [key, p] : markslist) {
            if(p->courseName==oldCourseN) {
                p->courseName=newCourseN;
            }
        }  
    }   
}

void StudentRegHashTable::updateStudent(string oldStudentN, string newStudentN) {
    if(student_check(oldStudentN)){
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

        for(auto & [key, p] : markslist) {
            if(p->studentName==oldStudentN) {
                p->studentName=newStudentN;
            }
        }  
    } 
}

void StudentRegHashTable::updateTeacher(string oldTeacherN, string newTeacherN){
    if(teacher_check(oldTeacherN)){    
        for(auto & [key, p] : teacherCourseTable) {
            if(p->name==oldTeacherN) {
                p->name=newTeacherN;
            }
        }
        const int slot = hash(oldTeacherN);
        Teacher* curTeacher = teacherlist.at(slot);
        while(nullptr!=curTeacher) {
            if(curTeacher->name == oldTeacherN) {
                curTeacher->name = newTeacherN;
            }
            curTeacher = curTeacher->next;
        }
    }
}

void StudentRegHashTable::updateMarks(string student, string course, double newMarks){
    if ((student_course_check(student, course))){
        const string MSlot = hashStudentCourse(student, course);
        if ((markslist.count(MSlot))>0){
            markslist[MSlot]->marks = newMarks;
        }
    }
}

void StudentRegHashTable::removeCourse(string course){
    if(course_check(course)){
        /*for(auto & [key, p] : studentCourseTable) {
            if(p->course==course) {
                delete p;
                studentCourseTable.erase(key);
            }
        }*/
        for(auto it = studentCourseTable.begin(); it != studentCourseTable.end();){
            if(it->second->course == course){
                delete it->second;
                it = studentCourseTable.erase(it);
            } else {
                ++it;
            }
        }
        /*for(auto & [key, p] : teacherCourseTable) {
            if(p->course==course) {
                delete p;
            }
        } */
        for(auto it = teacherCourseTable.begin(); it != teacherCourseTable.end();){
            if(it->second->course == course){
                delete it->second;
                it = teacherCourseTable.erase(it);
            } else {
                ++it;
            }
        }  

        const int slot = hash(course);
        Course* curCourse = courselist.at(slot);
        while(nullptr!=curCourse) {
            if(curCourse->courseName == course) {
                Course* temp = curCourse;
                curCourse=curCourse->next;
                delete temp;
            }
        }
        /*for(auto & [key, p] : markslist) {
            if(p->courseName==course) {
                delete p;
            }
        } */
       for(auto it = markslist.begin(); it != markslist.end();){
            if(it->second->courseName == course){
                delete it->second;
                it = markslist.erase(it);
            } else {
                ++it;
            }
        } 
    }
}

void StudentRegHashTable::removeStudent(string student){
    if(student_check(student)){
        /*for(auto & [key, p] : studentCourseTable) {
            if(p->name==student) {
                delete p;
            }
        }*/
       for(auto it = studentCourseTable.begin(); it != studentCourseTable.end();){
            if(it->second->name == student){
                delete it->second;
                it = studentCourseTable.erase(it);
            } else {
                ++it;
            }
        }

        const int slot = hash(student);
        Student* curStudent = studentlist.at(slot);
        while(nullptr!=curStudent) {
            if(curStudent->name == student) {
                Student* temp = curStudent;
                curStudent = curStudent->next;
                delete temp;
            }
        }

        /*for(auto & [key, p] : markslist) {
            if(p->studentName==student) {
                delete p;
            }
        }*/
       for(auto it = markslist.begin(); it != markslist.end();){
            if(it->second->studentName == student){
                delete it->second;
                it = markslist.erase(it);
            } else {
                ++it;
            }
        }  
    } 
}

void StudentRegHashTable::removeTeacher(string teacher){
    if(teacher_check(teacher)){    
        /*for(auto & [key, p] : teacherCourseTable) {
            if(p->name==teacher) {
                delete p;
            }
        }*/
       for(auto it = teacherCourseTable.begin(); it != teacherCourseTable.end();){
            if(it->second->name == teacher){
                delete it->second;
                it = teacherCourseTable.erase(it);
            } else {
                ++it;
            }
        }

        const int slot = hash(teacher);
        Teacher* curTeacher = teacherlist.at(slot);
        while(nullptr!=curTeacher) {
            if(curTeacher->name == teacher) {
                Teacher* temp = curTeacher;
                curTeacher=curTeacher->next;
                delete temp;
            }
        }
    }
}

void StudentRegHashTable::printStudentList() {
    dbstudentlist.open("studentlist.txt", ios::out);
    dbstudentlist<<"List of all the students:"<<endl<<endl;
    for(int i=0; i<studentlist.size(); i++) {
        Student* p = studentlist.at(i);
        while (nullptr!=p) {
            dbstudentlist<<p->name<<endl;
            p=p->next;
        }
    } 
    dbstudentlist.close();
}

void StudentRegHashTable::printCourseList() {
    dbcourselist.open("courselist.txt", ios::out);
    dbcourselist<<"List of all the courses:"<<endl<<endl;
    for(int i=0; i<courselist.size(); i++) {
        Course* p = courselist.at(i);
        while (nullptr!=p) {
            dbcourselist<<p->courseName<<endl;
            p=p->next;
        }
    } 
    dbcourselist.close();
}

void StudentRegHashTable::printTeacherList() {
    dbteacherlist.open("teacherlist.txt", ios::out);
    dbteacherlist<<"List of all the teachers:"<<endl<<endl;
    for(int i=0; i<teacherlist.size(); i++) {
        Teacher* p = teacherlist.at(i);
        while (nullptr!=p) {
            dbteacherlist<<p->name<<endl;
            p=p->next;
        }
    } 
    dbteacherlist.close();
}

void StudentRegHashTable::studentCourses(string student){
    dbstudentcourses.open("studentcourses.txt", ios::out);
    dbstudentcourses<<student<<"'s Courses:"<<endl<<endl;

    for(auto & [key, p] : studentCourseTable) {
        if(p->name==student) {
            dbstudentcourses<<p->course<<endl;
        }
    }
    dbstudentcourses.close();
}

void StudentRegHashTable::courseStudents(string course){
    dbcoursestudents.open("coursestudents.txt", ios::out);
    dbcoursestudents<<course<<"'s Students:"<<endl<<endl;

    for(auto & [key, p] : studentCourseTable) {
        if(p->course==course) {
            dbcoursestudents<<p->name<<endl;
        }
    }
    dbcoursestudents.close();
}

void StudentRegHashTable::teacherCourses(string teacher){
    dbteachercourses.open("teachercourses.txt", ios::out);
    dbteachercourses<<teacher<<"'s Courses:"<<endl<<endl;

    for(auto & [key, p] : teacherCourseTable) {
        if(p->name==teacher) {
            dbteachercourses<<p->course<<endl;
        }
    }
    dbteachercourses.close();
}

void StudentRegHashTable::courseTeachers(string course){
    dbcourseteachers.open("courseteachers.txt", ios::out);
    dbcourseteachers<<course<<"'s Teachers:"<<endl<<endl;

    for(auto & [key, p] : teacherCourseTable) {
        if(p->course==course) {
            dbcourseteachers<<p->name<<endl;
        }
    }
    dbcourseteachers.close();
}

void StudentRegHashTable::studentRC(string student){
    dbreportcard.open("reportcard.txt", ios::out);
    dbreportcard<<student<<"'s Report Card:"<<endl<<endl;

    for(auto & [key, p] : studentCourseTable) {
        if(p->name==student) {
            dbreportcard<<"Course: "<<p->course<<endl;
            if ((student_course_check(student, p->course))){
                const string MSlot = hashStudentCourse(student, p->course);
                if ((markslist.count(MSlot))>0){
                    dbreportcard<<"     Marks: "<<markslist[MSlot]->marks<<endl;
                    //what will i use instead of student?
                    //koresha p->course kureba muri tc ukoreshe the teacher ubona mo
                    string teacherTemp;
                    for(auto & [key, r] : teacherCourseTable) {
                        if(r->course==p->course) {
                            teacherTemp = r->name;
                        }
                    }
                    if ((teacher_course_check(teacherTemp, p->course))){
                        for(auto & [key, q] : teacherCourseTable) {
                            if(q->course==p->course) {
                                dbreportcard<<"     Teacher: "<<q->name<<endl;
                            }
                        }          
                    }
                }            
            }
        }
    }
    dbreportcard.close();
}

void StudentRegHashTable::printAll(){
    dball.open("all.txt", ios::out);

    //loop through all the student and do each thing for each student
    for(int i=0; i<studentlist.size(); i++) {
        Student* s = studentlist.at(i);
        while (nullptr!=s) {
            dball<<"Student: "<<s->name;
            for(auto & [key, p] : studentCourseTable) {
            if(p->name==s->name) {
                dball<<"     Course: "<<p->course<<endl;
                if ((student_course_check(s->name, p->course))){
                    const string MSlot = hashStudentCourse(s->name, p->course);
                    if ((markslist.count(MSlot))>0){
                        dball<<"     Marks: "<<markslist[MSlot]->marks<<endl;
                        //what will i use instead of student?
                        //koresha p->course kureba muri tc ukoreshe the teacher ubona mo
                        string teacherTemp;
                        for(auto & [key, r] : teacherCourseTable) {
                            if(r->course==p->course) {
                                teacherTemp = r->name;
                            }
                        }
                        if ((teacher_course_check(teacherTemp, p->course))){
                            for(auto & [key, q] : teacherCourseTable) {
                                if(q->course==p->course) {
                                    dball<<"     Teacher: "<<q->name<<endl;
                                }
                            }          
                        }
                    }            
                }
            }
            }
        s=s->next;
        }
    }
    dball.close();
}

int StudentRegHashTable::hash(string key) const { //needs to be updated but this is my studentlist
    char firstLetter = tolower(key[0]);
    return firstLetter - 'a';
}

string StudentRegHashTable::hashStudentCourse (string studentName, string courseName) const { //student_course
    string combination = studentName + courseName;
    return combination;
}

string StudentRegHashTable::hashMarks (string studentName, string courseName) const {//marks_student_course
    return hashStudentCourse(studentName, courseName);
}

int StudentRegHashTable::hashCourse (string course) const{ //courselist
    return (hash(course));
}

string StudentRegHashTable::hashTC (string teacher, string course) const { //teacher_course
    return (hashStudentCourse(teacher, course));
}

/*void StudentRegHashTable::insert(string key, string teacher, string course, double marks) {
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
} */

/* void StudentRegHashTable::remove(string key) {
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
}*/

/*void StudentRegHashTable::printOne(string key) {
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
}*/

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