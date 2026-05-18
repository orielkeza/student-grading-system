#include <string>
#include <gtest/gtest.h>
#include "entities.h"

using namespace std;

TEST(StudentRegHashTablePub, testDefaultBuckets) {
    EXPECT_EQ(StudentRegHashTable::DefaultBuckets, 26);
}

TEST(addStudentPub, testAddedStudentExists) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    EXPECT_TRUE(student);
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
    EXPECT_FALSE(course.student_course_check("Alice", "Math"));
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
    EXPECT_FALSE(student.student_check("NewGhost"));
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

TEST(studentCheckPub, multipleStudentsDifferentBuckets) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addStudent("Bob");
    EXPECT_TRUE(student.student_check("Alice"));
    EXPECT_TRUE(student.student_check("Bob"));
}

TEST(studentCheckPub, multipleStudentsSameBucket) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addStudent("Alex");
    EXPECT_TRUE(student.student_check("Alice"));
    EXPECT_TRUE(student.student_check("Alex"));
}

TEST(teacherCheckPub, multipleTeachersSameBucket) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addTeacher("Claude");
    EXPECT_TRUE(teacher.teacher_check("Charles"));
    EXPECT_TRUE(teacher.teacher_check("Claude"));
}

TEST(courseCheckPub, multipleCoursesSameBucket) {
    StudentRegHashTable course;
    course.addCourse("Math");
    course.addCourse("Music");
    EXPECT_TRUE(course.course_check("Math"));
    EXPECT_TRUE(course.course_check("Music"));
}

TEST(studentCourseCheckPub, bothMissing) {
    StudentRegHashTable student;
    EXPECT_FALSE(student.student_course_check("Alice", "Math"));
}

TEST(teacherCourseCheckPub, courseMissing) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    EXPECT_FALSE(teacher.teacher_course_check("Charles", "Math"));
}

TEST(teacherCourseCheckPub, bothMissing) {
    StudentRegHashTable teacher;
    EXPECT_FALSE(teacher.teacher_course_check("Charles", "Math"));
}

TEST(addStudentCoursePub, succeedsWhenBothExist) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    EXPECT_TRUE(student.student_course_check("Alice", "Math"));
}

TEST(addTeacherCoursePub, requiresBothToExist) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addTeacher_Course("Charles", "Math");
    EXPECT_FALSE(teacher.teacher_course_check("Charles", "Math"));
}

TEST(addTeacherCoursePub, doesNothingIfTeacherMissing) {
    StudentRegHashTable course;
    course.addCourse("Math");
    course.addTeacher_Course("Charles", "Math");
    EXPECT_FALSE(course.teacher_course_check("Charles", "Math"));
}

TEST(addTeacherCoursePub, succeedsWhenBothExist) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    teacher.addTeacher_Course("Charles", "Math");
    EXPECT_TRUE(teacher.teacher_course_check("Charles", "Math"));
}


TEST(addMarksPub, succeedsWhenBothExist) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.addMarks(95.0, "Alice", "Math");
    EXPECT_TRUE(student.student_course_check("Alice", "Math"));
}

TEST(updateStudentPub, updatesNameInStudentCourseTable) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.updateStudent("Alice", "Anne");
    EXPECT_TRUE(student.student_check("Anne"));
}

TEST(updateTeacherPub, updatesNameInTeacherCourseTable) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    teacher.addTeacher_Course("Charles", "Math");
    teacher.updateTeacher("Charles", "Don");
    EXPECT_TRUE(teacher.teacher_check("Don"));
}

TEST(updateCoursePub, updatesNameInStudentCourseTable) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.updateCourse("Math", "Calculus");
    EXPECT_TRUE(student.course_check("Calculus"));
}

TEST(updateMarksPub, updatesMarksWhenEntryExists) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.addMarks(70.0, "Alice", "Math");
    student.updateMarks("Alice", "Math", 90.0);
    EXPECT_TRUE(student.student_course_check("Alice", "Math"));
}

TEST(updateMarksPub, doesNothingWhenStudentOrCourseMissing) {
    StudentRegHashTable marks;
    marks.updateMarks("Alice", "Math", 90.0);
    EXPECT_FALSE(marks.student_course_check("Alice", "Math"));
}

TEST(removeStudentPub, removingStudentAlsoRemovesFromStudentCourseTable) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.removeStudent("Alice");
    EXPECT_FALSE(student.student_check("Alice"));
}

TEST(removeTeacherPub, removeNonExistentTeacherDoesNothing) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.removeTeacher("Ghost");
    EXPECT_TRUE(teacher.teacher_check("Charles"));
}

TEST(removeTeacherPub, removingTeacherAlsoRemovesFromTeacherCourseTable) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    teacher.addTeacher_Course("Charles", "Math");
    teacher.removeTeacher("Charles");
    EXPECT_FALSE(teacher.teacher_check("Charles"));
}

TEST(removeCoursePub, removingCourseAlsoRemovesFromJunctionTables) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addTeacher("Charles");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.addTeacher_Course("Charles", "Math");
    student.removeCourse("Math");
    EXPECT_FALSE(student.course_check("Math"));
}

TEST(printStudentListPub, doesNotCrash) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addStudent("Bob");
    student.printStudentList();
}

TEST(printCourseListPub, doesNotCrash) {
    StudentRegHashTable teacher;
    teacher.addCourse("Math");
    teacher.addCourse("Science");
    teacher.printCourseList();
}

TEST(printTeacherListPub, doesNotCrash) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.printTeacherList();
}

TEST(printAllPub, doesNotCrash) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addTeacher("Charles");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.addTeacher_Course("Charles", "Math");
    student.addMarks(88.0, "Alice", "Math");
    student.printAll();
}

TEST(studentCoursesPub, doesNotCrash) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.studentCourses("Alice");
}

TEST(courseStudentsPub, doesNotCrash) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.courseStudents("Math");
}

TEST(teacherCoursesPub, doesNotCrash) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    teacher.addTeacher_Course("Charles", "Math");
    teacher.teacherCourses("Charles");
}

TEST(courseTeachersPub, doesNotCrash) {
    StudentRegHashTable teacher;
    teacher.addTeacher("Charles");
    teacher.addCourse("Math");
    teacher.addTeacher_Course("Charles", "Math");
    teacher.courseTeachers("Math");
}

TEST(studentRCPub, doesNotCrash) {
    StudentRegHashTable student;
    student.addStudent("Alice");
    student.addTeacher("Charles");
    student.addCourse("Math");
    student.addStudent_Course("Alice", "Math");
    student.addTeacher_Course("Charles", "Math");
    student.addMarks(92.0, "Alice", "Math");
    student.studentRC("Alice");
}

TEST(destructorPub, emptyTableDeletesCleanly) {
    StudentRegHashTable* student = new StudentRegHashTable;
    delete student;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}