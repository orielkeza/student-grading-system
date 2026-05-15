#ifndef ENTITIES_H
#define ENTITIES_H
#include <vector>
#include <string>
#include <fstream>
#include <map>
 
class StudentRegHashTable {
    public:
        StudentRegHashTable();
        virtual ~StudentRegHashTable();
        void addStudent(std::string student);
        void addTeacher(std::string teacher);
        void addCourse(std::string course);
        void StudentRegHashTable::addStudent_Course(std::string student, std::string course);
        void StudentRegHashTable::addTeacher_Course(std::string teacher, std::string course);
        void StudentRegHashTable::addMarks(double marks, std::string student, std::string course);
       // void insert(std::string name, std::string teacher, std::string course, double marks);
        void remove(std::string studentName);
        bool lookup(std::string studentName) const;
        bool student_course_check(std::string student, std::string course) const; //to make sure student and course exist
        bool teacher_course_check(std::string teacher, std::string course) const; //to make sure student and course exist
        bool student_check(std::string student) const; //to make sure student and course exist
        bool teacher_check(std::string teacher) const; //to make sure student and course exist
        bool course_check(std::string course) const; //to make sure student and course exist
        void printAll();
        void printJunctionAll();
        void printOne(std::string studentName);
        void updateCourse(std::string oldCourseN, std::string newCourseN);
        void updateStudent(std::string oldStudentN, std::string newStudentN);
        void printStudentList(std::string course);
        void printCourseList(std::string studentName);
        void loadDB(std::fstream& fileStd, std::fstream& fileSC);
        static const int DefaultBuckets = 26;
    protected:
        /*struct Student {
            std::string name;//student name will be the key for now until i add id generation
            Student* next;
            std::string teacher;
            std::string course;
            double marks;
        };*/
        struct Student {
            std::string name;
            Student* next;
        };
        struct Teacher {
            std::string name;
            Teacher* next;
        };
        struct SC {
            std::string name;//student name will be the key for now until i add id generation
            SC* next;
            std::string course;
        };
        struct TC {
            std::string name;//student name will be the key for now until i add id generation
            TC* next;
            std::string course;
        };
        struct Course {
            std::string courseName;
            Course* next;
        };
        struct Marks {
            double marks;
            std::string studentName;
            std::string courseName;
            Marks* next;
        };
    private:
        std::vector<Student*> table;
        std::vector<Student*> studentlist;
        std::vector<Teacher*> teacherlist;
        std::map<std::string, Course*> courselist;
        std::map<std::string, Marks*> markslist;
        std::map<std::string, SC*> studentCourseTable;
        std::map<std::string, TC*> teacherCourseTable;
        std::fstream db;
        std::fstream junctiondb;
        std::fstream dbCourseList;
        std::fstream dbStudentList;
        int hash(std::string key) const;//this is what i need to define to get alphabetical order
        std::string hashStudentCourse(std::string studentName, std::string courseName) const;//to store the student and course together
        std::string hashMarks (std::string studentName, std::string courseName, double marks) const; //marks_student_course
        int hashCourse (std::string course) const; //courselist
        std::string hashTC (std::string teacher, std::string course) const; //teacher_course
};
/*
class Marks {
    public:
        Marks (std::string name, int mark);
    protected:
        std::string toString () const;
    private:
        std::string name;
        int num;
};

class Course {
    public:
        Course (std::string name);
        std::vector<Marks*> marksList;

    protected:
        virtual void allMarks ();
        std::string name;
};

class Person {
    public:
        virtual ~Person ();
    protected:
        Person (std::string name);
        virtual std::string getName () const;
        virtual void addCourse (std::string course);
        virtual void allCourses();
    private:
        std::string name;
        std::string course;
        std::vector<Course*> courseList;
        std::fstream courseFile;
};

class Teacher : public Person {
    public:
        Teacher (std:: string name);
        virtual ~Teacher ();
    private:
        void allStudents ();
        void addMark (std::string courseName, string markName, int mark);
        void changeMark (std::string courseName, int index, int newMark);
        std::vector<Student*> studentList;
        std::fstream studentFile;
};

class Student : public Person {
    public:
        Student (std::string name);
        virtual ~Student ();
    private:
        void allTeachers();
        std::vector<Teacher*> teacherList;
        std::fstream marksFile;

};
*/


#endif