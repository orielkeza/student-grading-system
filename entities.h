    #include <vector>
#include <string>

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
    protected:
        virtual void allMarks ();
    private:
        std::string name;
        std::vector<Marks*> marksList;
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
};

class Teacher : public Person {
    public:
        Teacher (std:: string name);
        virtual ~Teacher ();
    private:
        void changeMark (std::string courseName, int index, int newNum);
        std::vector<Student*> studentList;
};

class Student : public Person {
    public:
        Student (std::string name);
        virtual ~Student ();
    private:
        std::vector<Teacher*> teacherList;

};