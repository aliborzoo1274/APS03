#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

const char PREREQUISITES_DELIM = ',';

const int MIN_PASSING_GRADE = 10;
const int MIN_REGISTERED_CREDIT = 12;
const int EXCELLENT_MIN_GRADE = 17;
const int EXCELLENT_MAX_REGISTERED_CREDIT = 24;
const int MEDIOCRE_MIN_GRADE = 12;
const int MEDIOCRE_MAX_REGISTERED_CREDIT = 20;
const int WEAK_MAX_REGISTERED_CREDIT = 14;

const std::string OK = "OK!";
const std::string COURSE_NOT_FOUND_ERR = "Course Not Offered!";
const std::string MIN_CREDIT_ERR = "Minimum Units Validation Failed!";
const std::string MAX_CREDIT_ERR = "Maximum Units Validation Failed!";
const std::string REPEATED_COURSE_ERR = "Course Already Taken!";
const std::string PREREQUISITES_ERR = "Prerequisites Not Met!";

struct Course {
    std::string name;
    int credit;
    std::vector<std::string> prerequisites;

    bool operator==(const Course& other) {
        return name == other.name;
    }
};

struct EnrollmentRecord {
    std::string courseName;
    int credit;
    double grade;
};

struct Student {
    std::vector<EnrollmentRecord> enrollmentHistory;
    double gpa;
    std::vector<std::optional<Course>> registeredCourses;
};

std::optional<Course> findCourse(const std::string& courseName, const std::vector<Course>& courses) {
    const auto foundCourse =
        std::find_if(courses.begin(), courses.end(), [&courseName](const Course& course) {
            return course.name == courseName;
        });

    if (foundCourse == courses.end()) {
        return {};
    }
    return *foundCourse;
}

bool isPassed(const EnrollmentRecord& record) {
    return record.grade >= MIN_PASSING_GRADE;
}

bool meetMinRegisteredCreditCount(const std::vector<std::optional<Course>>& registeredCourses) {
    int registeredCreditCount = 0;
    for (const auto& registeredCourse : registeredCourses) {
        if (!registeredCourse.has_value()) {
            continue;
        }
        registeredCreditCount += registeredCourse.value().credit;
    }

    return registeredCreditCount >= MIN_REGISTERED_CREDIT;
}

bool meetMaxRegisteredCreditCount(const std::vector<std::optional<Course>> registeredCourses, int gpa) {
    int registeredCreditCount = 0;
    for (const auto& registeredCourse : registeredCourses) {
        if (!registeredCourse.has_value()) {
            continue;
        }
        registeredCreditCount += registeredCourse.value().credit;
    }

    if (gpa >= EXCELLENT_MIN_GRADE)
        return registeredCreditCount <= EXCELLENT_MAX_REGISTERED_CREDIT;
    if (gpa >= MEDIOCRE_MIN_GRADE)
        return registeredCreditCount <= MEDIOCRE_MAX_REGISTERED_CREDIT;
    return registeredCreditCount <= WEAK_MAX_REGISTERED_CREDIT;
}

bool courseIsPassed(const std::string& courseName,
                    const std::vector<EnrollmentRecord> enrollmentHistory) {
    return std::any_of(enrollmentHistory.begin(),
                       enrollmentHistory.end(),
                       [&courseName](const EnrollmentRecord& record) {
                           return record.courseName == courseName && isPassed(record);
                       });
}

bool meetPrerequisites(
    const Course& registeredCourse,
    const std::vector<EnrollmentRecord>& enrollmentHistory,
    const std::vector<Course>& courses) {
    for (const auto& prerequisite : registeredCourse.prerequisites) {
        if (!courseIsPassed(prerequisite, enrollmentHistory))
            return false;
    }
    return true;
}

double calcGpa(const std::vector<EnrollmentRecord>& enrollmentHistory) {
    int gradeSum = 0, creditSum = 0;

    for (const auto& enrollmentRecord : enrollmentHistory) {
        gradeSum += enrollmentRecord.grade * enrollmentRecord.credit;
        creditSum += enrollmentRecord.credit;
    }

    return gradeSum / creditSum;
}

std::vector<Course> getCourses() {
    auto getPrerequisites = [](std::string prerequisitesLine) {
        std::istringstream lineStream(prerequisitesLine);
        std::vector<std::string> prerequisites;

        std::string currentPrerequisite;
        while (std::getline(lineStream, currentPrerequisite, PREREQUISITES_DELIM))
            prerequisites.push_back(currentPrerequisite);

        return prerequisites;
    };

    int m;
    std::vector<Course> result;

    std::cin >> m;
    result.reserve(m);
    while (m--) {
        std::string name;
        int credit;
        std::string prerequisitesLine;
        std::cin >> name >> credit >> prerequisitesLine;
        std::vector<std::string> prerequisites = getPrerequisites(prerequisitesLine);
        result.push_back({name, credit, prerequisites});
    }

    return result;
}

std::vector<EnrollmentRecord> getEnrollmentHistory(std::vector<Course>& courses) {
    int n;
    std::vector<EnrollmentRecord> result;

    std::cin >> n;
    result.reserve(n);
    while (n--) {
        std::string courseName;
        int credit;
        double grade;
        std::cin >> courseName >> credit >> grade;
        result.push_back({courseName, credit, grade});
    }

    return result;
}

std::vector<std::optional<Course>> getRegisteredCourses(const std::vector<Course>& courses) {
    int k;
    std::vector<std::optional<Course>> result;

    std::cin >> k;
    result.reserve(k);
    while (k--) {
        std::string courseName;
        std::cin >> courseName;
        result.push_back(findCourse(courseName, courses));
    }

    return result;
}

void printValidationResult(const Student& student, const std::vector<Course>& courses) {
    bool errorOccurred = false;

    for (const auto registeredCourse : student.registeredCourses) {
        if (!registeredCourse.has_value()) {
            std::cout << COURSE_NOT_FOUND_ERR << std::endl;
            errorOccurred = true;
        }
        else {
            if (courseIsPassed(registeredCourse.value().name, student.enrollmentHistory)) {
                std::cout << REPEATED_COURSE_ERR << std::endl;
                errorOccurred = true;
            }
            if (!meetPrerequisites(registeredCourse.value(), student.enrollmentHistory, courses)) {
                std::cout << PREREQUISITES_ERR << std::endl;
                errorOccurred = true;
            }
        }
    }

    if (!meetMinRegisteredCreditCount(student.registeredCourses)) {
        std::cout << MIN_CREDIT_ERR << std::endl;
        errorOccurred = true;
    }
    else if (!meetMaxRegisteredCreditCount(student.registeredCourses, student.gpa)) {
        std::cout << MAX_CREDIT_ERR << std::endl;
        errorOccurred = true;
    }

    if (!errorOccurred)
        std::cout << OK << std::endl;
}

int main() {
    std::vector<Course> courses = getCourses();
    std::vector<EnrollmentRecord> enrollmentHistory = getEnrollmentHistory(courses);
    std::vector<std::optional<Course>> registeredCourses = getRegisteredCourses(courses);

    Student student = {enrollmentHistory, calcGpa(enrollmentHistory), registeredCourses};

    printValidationResult(student, courses);

    return 0;
}
