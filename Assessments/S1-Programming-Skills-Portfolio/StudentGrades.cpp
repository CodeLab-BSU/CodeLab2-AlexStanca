#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Student {
private:
    string studentID;
    string name;
    double coursework;
    double exam;

public:
    Student(string id, string studentName, double cw, double ex)
        : studentID(id), name(studentName), coursework(cw), exam(ex) {}

    string getID() const {
        return studentID;
    }

    double calculateFinalMark() const {
        return (coursework * 0.4) + (exam * 0.6);
    }

    string getClassification() const {
        double mark = calculateFinalMark();

        if (mark >= 70) return "First";
        if (mark >= 60) return "Upper Second";
        if (mark >= 50) return "Lower Second";
        if (mark >= 40) return "Third";
        return "Fail";
    }

    void displayDetails() const {
        cout << left << setw(12) << studentID
             << setw(22) << name
             << setw(14) << coursework
             << exam << '\n';
    }

    void displayResult() const {
        cout << "\nStudent: " << name << " (" << studentID << ")\n";
        cout << "Final module mark: " << calculateFinalMark() << "%\n";
        cout << "Classification: " << getClassification() << '\n';
    }
};

bool loadStudents(const string& filename, vector<Student>& students) {
    ifstream file(filename);

    if (!file) {
        cout << "Could not open " << filename << '\n';
        return false;
    }

    string line;
    getline(file, line); // Skip the column headings.
    int lineNumber = 1;

    while (getline(file, line)) {
        lineNumber++;
        if (line.empty() || line == "\r") continue;

        stringstream record(line);
        string id, name, courseworkText, examText;

        if (!getline(record, id, ',') ||
            !getline(record, name, ',') ||
            !getline(record, courseworkText, ',') ||
            !getline(record, examText)) {
            cout << "Skipping invalid record on line " << lineNumber << '\n';
            continue;
        }

        double coursework, exam;
        stringstream courseworkInput(courseworkText);
        stringstream examInput(examText);

        if (!(courseworkInput >> coursework) ||
            !(examInput >> exam)) {
            cout << "Skipping invalid marks on line " << lineNumber << '\n';
            continue;
        }

        courseworkInput >> ws;
        examInput >> ws;

        if (!courseworkInput.eof() || !examInput.eof() ||
            id.empty() || name.empty() ||
            coursework < 0 || coursework > 100 ||
            exam < 0 || exam > 100) {
            cout << "Skipping invalid record on line " << lineNumber << '\n';
            continue;
        }

        students.emplace_back(id, name, coursework, exam);
    }

    if (students.empty()) {
        cout << "No valid student records were found.\n";
        return false;
    }

    return true;
}

void displayHeading() {
    cout << '\n' << left << setw(12) << "Student ID"
         << setw(22) << "Name"
         << setw(14) << "Coursework"
         << "Exam\n";
    cout << string(56, '-') << '\n';
}

const Student* findStudent(const vector<Student>& students,
                           const string& id) {
    for (const Student& student : students) {
        if (student.getID() == id) {
            return &student;
        }
    }
    return nullptr;
}

int main() {
    vector<Student> students;

    if (!loadStudents("Resources/studentMarks.txt", students)) {
        return 1;
    }

    cout << fixed << setprecision(2);
    string choice;

    while (true) {
        cout << "\n--- Student Grade Manager ---\n";
        cout << "1. View all students\n";
        cout << "2. View an individual student\n";
        cout << "3. Calculate a student's final module mark\n";
        cout << "4. Quit\n";
        cout << "Choose an option: ";

        if (!getline(cin, choice) || choice == "4") break;

        if (choice == "1") {
            displayHeading();
            for (const Student& student : students) {
                student.displayDetails();
            }
        } else if (choice == "2" || choice == "3") {
            string id;
            cout << "Enter student ID: ";
            if (!getline(cin, id)) break;

            const Student* student = findStudent(students, id);

            if (student == nullptr) {
                cout << "Student not found.\n";
            } else if (choice == "2") {
                displayHeading();
                student->displayDetails();
            } else {
                student->displayResult();
            }
        } else {
            cout << "Invalid option. Please enter 1, 2, 3 or 4.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}