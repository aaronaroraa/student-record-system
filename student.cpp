#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Student {
    int rollNumber;
    string name;
    float marks;

    void input() {
        cout << "Enter Roll Number: ";
        cin >> rollNumber;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Marks: ";
        cin >> marks;
    }

    void display() const {
        cout << left << setw(10) << rollNumber
             << setw(20) << name
             << setw(10) << marks << endl;
    }
};

vector<Student> students;

void loadFromFile() {
    ifstream in("students.dat", ios::binary);
    students.clear();
    Student s;
    while (in.read((char*)&s, sizeof(Student))) {
        students.push_back(s);
    }
    in.close();
}

void saveToFile() {
    ofstream out("students.dat", ios::binary);
    for (const Student& s : students) {
        out.write((char*)&s, sizeof(Student));
    }
    out.close();
}

void addStudent() {
    Student s;
    s.input();
    students.push_back(s);
    saveToFile();
    cout << "Student added successfully!\n";
}

void displayStudents() {
    cout << left << setw(10) << "Roll No" << setw(20) << "Name" << setw(10) << "Marks" << endl;
    for (const Student& s : students) s.display();
}

void searchStudent() {
    int roll;
    cout << "Enter roll number to search: ";
    cin >> roll;
    bool found = false;
    for (const Student& s : students) {
        if (s.rollNumber == roll) {
            s.display();
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found.\n";
}

void editStudent() {
    int roll;
    cout << "Enter roll number to edit: ";
    cin >> roll;
    for (Student& s : students) {
        if (s.rollNumber == roll) {
            cout << "Enter new details:\n";
            s.input();
            saveToFile();
            cout << "Record updated.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void deleteStudent() {
    int roll;
    cout << "Enter roll number to delete: ";
    cin >> roll;
    auto it = remove_if(students.begin(), students.end(), [roll](Student& s) {
        return s.rollNumber == roll;
    });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveToFile();
        cout << "Record deleted.\n";
    } else {
        cout << "Student not found.\n";
    }
}

void sortMenu() {
    int choice;
    cout << "Sort By:\n1. Name\n2. Marks\nChoice: ";
    cin >> choice;
    if (choice == 1) {
        sort(students.begin(), students.end(), [](Student& a, Student& b) {
            return a.name < b.name;
        });
    } else if (choice == 2) {
        sort(students.begin(), students.end(), [](Student& a, Student& b) {
            return a.marks > b.marks;
        });
    }
    displayStudents();
}

void menu() {
    int choice;
    do {
        cout << "\n--- Student Record Management ---\n";
        cout << "1. Add Student\n2. Display All\n3. Search\n4. Edit\n5. Delete\n6. Sort\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: editStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortMenu(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    loadFromFile();
    menu();
    return 0;
}
