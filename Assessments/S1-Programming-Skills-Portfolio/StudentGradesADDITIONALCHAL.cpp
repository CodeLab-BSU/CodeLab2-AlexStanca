#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
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

    void writeToFile(ostream& file) const {
        file << studentID << ',' << name << ','
             << coursework << ',' << exam << '\n';
    }
};

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

bool parseMark(const string& text, double& mark) {
    stringstream input(text);

    if (!(input >> mark)) return false;
    input >> ws;

    return input.eof() && isfinite(mark) && mark >= 0 && mark <= 100;
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
        if (trim(line).empty()) continue;

        stringstream record(line);
        string id, name, courseworkText, examText;

        if (!getline(record, id, ',') ||
            !getline(record, name, ',') ||
            !getline(record, courseworkText, ',') ||
            !getline(record, examText)) {
            cout << "Invalid record on line " << lineNumber << '\n';
            return false;
        }

        id = trim(id);
        name = trim(name);
        double coursework, exam;

        if (id.empty() || name.empty() ||
            !parseMark(courseworkText, coursework) ||
            !parseMark(examText, exam) ||
            findStudent(students, id) != nullptr) {
            cout << "Invalid or duplicate record on line "
                 << lineNumber << '\n';
            return false;
        }

        students.emplace_back(id, name, coursework, exam);
    }

    if (file.bad()) {
        cout << "An error occurred while reading the file.\n";
        return false;
    }

    return true;
}

bool saveStudents(const string& filename,
                  const vector<Student>& students) {
    ofstream file(filename);

    if (!file) {
        cout << "Could not open the file for saving.\n";
        return false;
    }

    file << "StudentID,Name,Coursework,Exam\n";
    file << setprecision(17);

    for (const Student& student : students) {
        student.writeToFile(file);
    }

    file.close();

    if (!file) {
        cout << "An error occurred while saving.\n";
        return false;
    }

    cout << "Student records saved successfully.\n";
    return true;
}

void displayHeading() {
    cout << '\n' << left << setw(12) << "Student ID"
         << setw(22) << "Name"
         << setw(14) << "Coursework"
         << "Exam\n";
    cout << string(56, '-') << '\n';
}

bool readMark(const string& prompt, double& mark) {
    string input;

    while (true) {
        cout << prompt;
        if (!getline(cin, input)) return false;

        if (parseMark(input, mark)) return true;
        cout << "Enter a number between 0 and 100.\n";
    }
}

bool addStudent(vector<Student>& students) {
    string id, name;
    double coursework, exam;

    while (true) {
        cout << "Enter new student ID: ";
        if (!getline(cin, id)) return false;
        id = trim(id);

        if (id.empty() || id.find(',') != string::npos) {
            cout << "ID cannot be empty or contain commas.\n";
        } else if (findStudent(students, id) != nullptr) {
            cout << "That student ID already exists.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Enter student name: ";
        if (!getline(cin, name)) return false;
        name = trim(name);

        if (!name.empty() && name.find(',') == string::npos) break;
        cout << "Name cannot be empty or contain commas.\n";
    }

    if (!readMark("Coursework mark (0-100): ", coursework)) return false;
    if (!readMark("Exam mark (0-100): ", exam)) return false;

    students.emplace_back(id, name, coursework, exam);
    cout << "Student added. Choose option 5 to save your changes.\n";
    return true;
}

void displayHighestStudent(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students available.\n";
        return;
    }

    double highest = students.front().calculateFinalMark();

    for (const Student& student : students) {
        if (student.calculateFinalMark() > highest) {
            highest = student.calculateFinalMark();
        }
    }

    cout << "\nHighest-performing student(s):\n";

    for (const Student& student : students) {
        if (abs(student.calculateFinalMark() - highest) < 0.000001) {
            student.displayResult();
        }
    }
}

int main() {
    const string filename = "Resources/studentMarks.txt";
    vector<Student> students;

    if (!loadStudents(filename, students)) return 1;

    cout << fixed << setprecision(2);
    bool unsavedChanges = false;
    string choice;

    while (true) {
        cout << "\n--- Student Grade Manager ---\n";
        cout << "1. View all students\n";
        cout << "2. View an individual student\n";
        cout << "3. Calculate a student's final module mark\n";
        cout << "4. Add a new student\n";
        cout << "5. Save records to file\n";
        cout << "6. View highest-performing student\n";
        cout << "7. Save and quit\n";
        cout << "Choose an option: ";

        if (!getline(cin, choice)) {
            if (unsavedChanges && !saveStudents(filename, students)) {
                return 1;
            }
            break;
        }

        if (choice == "1") {
            if (students.empty()) {
                cout << "No students available.\n";
            } else {
                displayHeading();
                for (const Student& student : students) {
                    student.displayDetails();
                }
            }
        } else if (choice == "2" || choice == "3") {
            string id;
            cout << "Enter student ID: ";
            if (!getline(cin, id)) continue;

            const Student* student = findStudent(students, trim(id));

            if (student == nullptr) {
                cout << "Student not found.\n";
            } else if (choice == "2") {
                displayHeading();
                student->displayDetails();
            } else {
                student->displayResult();
            }
        } else if (choice == "4") {
            if (addStudent(students)) unsavedChanges = true;
        } else if (choice == "5") {
            if (saveStudents(filename, students)) unsavedChanges = false;
        } else if (choice == "6") {
            displayHighestStudent(students);
        } else if (choice == "7") {
            if (!unsavedChanges || saveStudents(filename, students)) break;
            cout << "Changes remain unsaved. Please try saving again.\n";
        } else {
            cout << "Invalid option. Please enter a number from 1 to 7.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}