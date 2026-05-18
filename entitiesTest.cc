#include <string>
#include "gtest/gtest.h"
#include "entities.h"

using namespace std;

TEST(StudentRegHashTablePub, testDefaultBuckets) {
    EXPECT_EQ(StudentRegHashTable::DefaultBuckets, 26);
}

TEST(addStudentPub, testAddedStudentExists) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    EXPECT_TRUE(srt.student_check("Alice"));
}

TEST(addStudentPub, testMissingStudentDoesNotExist) {
    StudentRegHashTable student;
    EXPECT_FALSE(student.student_check("Bob"));
}

TEST(addTeacherPub, testAddedTeacherExists) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    EXPECT_TRUE(teacher.teacher_check("Charles"));
}

TEST(addTeacherPub, testMissingTeacherDoesNotExist) {
    StudentRegHashTable teacher;
    EXPECT_FALSE(teacher.teacher_check("Charles"));
}

TEST(addCoursePub, testAddedCourseExists) {
    StudentRegHashTable course;
    course.addCourse("Math");
    EXPECT_TRUE(course.course_check("Math"));
}

TEST(addCoursePub, testMissingCourseDoesNotExist) {
    StudentRegHashTable course;
    EXPECT_FALSE(course.course_check("Math"));
}

TEST(studentCourseCheckPub, testBothExist) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    EXPECT_TRUE(student.student_course_check("Alice", "Math"));
}

TEST(studentCourseCheckPub, testStudentMissing) {
    StudentRegHashTable course;
    course.addCourse("Math");
    EXPECT_FALSE(srt.student_course_check("Alice", "Math"));
}

TEST(studentCourseCheckPub, testCourseMissing) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    EXPECT_FALSE(student.student_course_check("Alice", "Math"));
}

TEST(teacherCourseCheckPub, testBothExist) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    EXPECT_TRUE(teacher.teacher_course_check("Charles", "Math"));
}

TEST(teacherCourseCheckPub, testTeacherMissing) {
    StudentRegHashTable course;
    course.addCourse("Math");
    EXPECT_FALSE(course.teacher_course_check("Charles", "Math"));
}

TEST(addStudentCoursePub, testAddStudentCourseRequiresBoth) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addStudent_Course("Alice", "Math"); // Math not added, should do nothing
    EXPECT_FALSE(student.student_course_check("Alice", "Math"));
}

TEST(updateStudentPub, testUpdateStudentName) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.updateStudent("Alice", "Bob");
    EXPECT_TRUE(student.student_check("Bob"));
}

TEST(updateStudentPub, testUpdateNonExistentStudentDoesNothing) {
    StudentRegHashTable student;
    student.updateStudent("Ghost", "NewGhost");
    EXPECT_FALSE(tudent.student_check("NewGhost"));
}

TEST(updateTeacherPub, testUpdateTeacherName) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.updateTeacher("Charles", "Don");
    EXPECT_TRUE(teacher.teacher_check("Don"));
}

TEST(updateCoursePub, testUpdateCourseName) {
    StudentRegHashTable course;
    course.addCourse("Math");
    course.updateCourse("Math", "Science");
    EXPECT_TRUE(course.course_check("Science"));
}

TEST(removeStudentPub, testRemovedStudentNoLongerExists) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.removeStudent("Alice");
    EXPECT_FALSE(student.student_check("Alice"));
}

TEST(removeStudentPub, testRemoveNonExistentStudentDoesNothing) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.removeStudent("Bob");
    EXPECT_TRUE(student.student_check("Alice"));
}

TEST(removeTeacherPub, testRemovedTeacherNoLongerExists) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.removeTeacher("Charles");
    EXPECT_FALSE(teacher.teacher_check("Charles"));
}

TEST(removeCoursePub, testRemovedCourseNoLongerExists) {
    StudentRegHashTable course;
    course.addCourse("Math");
    course.removeCourse("Math");
    EXPECT_FALSE(course.course_check("Math"));
}

TEST(removeCoursePub, testRemoveNonExistentCourseDoesNothing) {
    StudentRegHashTable course;
    course.addCourse("Math");
    course.removeCourse("Science");
    EXPECT_TRUE(course.course_check("Math"));
}

TEST(heapPub, testHeapAllocatedTableDeletesCleanly) {
    StudentRegHashTable* student = new StudentRegHashTable;
    student->addStudent("Alice");
    student->addTeacher("Charles");
    student->addCourse("Math");
    delete student;
}