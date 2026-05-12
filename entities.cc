#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "entities.h"

//no duplicate student name, teacher names, course names (id generation for unique identification to be added later)
//students can add courses (which automatically adds them to the teacher's student list) and view what courses and teachers they have added
//maybe course can be something only a teacher owns, they have their list of courses and each list of courses has a list of students
//
//

using namespace std;

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

void Teacher::changeMark (string courseName, int index, int newNum){
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
