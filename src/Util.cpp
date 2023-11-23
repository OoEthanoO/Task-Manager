/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.3
 */

// Contribution: aqariio

#include "Util.h"
#include "Task.h"
#include "Program.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

Program program = Program();
vector<Task> tasks = program.tasks;
string Util::sortBy = "priority";
const int WIDTH = 70;

void Util::printTopRow() {
    cout << "\n";
    cout << "\033[34m";
    for (int i = 0; i < WIDTH; i++) {
        cout << "-";
    }
    cout << "\033[0m";
    cout << "\n";
}

void Util::printBottomRow() {
    cout << "\033[34m";
    for (int i = 0; i < WIDTH; i++) {
        cout << "-";
    }
    cout << "\033[0m";
    cout << "\n\n";
}

void Util::printf(const string &text) {
    int size = text.size();
    int width = WIDTH;
    width -= 3;
    cout << "\033[34m| \033[0m";
    cout << text;
    for (int i = 0; i < (int) width - size; i++) {
        cout << " ";
    }

    cout << "\033[34m|\033[0m";
    cout << "\n";
}

void Util::printf(const string &text, int size) {
    int width = WIDTH;
    width -= 3;
    cout << "\033[34m| \033[0m";
    cout << text;
    for (int i = 0; i < (int) width - size; i++) {
        cout << " ";
    }

    cout << "\033[34m|\033[0m";
    cout << "\n";
}

void Util::checkDate(string date) {
    if (date.size() != 10) {
        cout << "\033[31mDate must be in the format of YYYY-MM-DD.\n\033[0m";
        program.showMenu();
        return;
    }
    if (date[4] != '-' || date[7] != '-') {
        cout << "\033[31mDate must be in the format of YYYY-MM-DD.\n\033[0m";
        program.showMenu();
        return;
    }

    string sYear = date.substr(0, 4);
    if (!all_of(sYear.begin(), sYear.end(), ::isdigit)) {
        cout << "\033[31mYear must be a number.\n\033[0m";
        program.showMenu();
        return;
    }
    string sMonth = date.substr(5, 2);
    if (!all_of(sMonth.begin(), sMonth.end(), ::isdigit)) {
        cout << "\033[31mMonth must be a number.\n\033[0m";
        program.showMenu();
        return;
    }
    string sDay = date.substr(8, 2);
    if (!all_of(sDay.begin(), sDay.end(), ::isdigit)) {
        cout << "\033[31mDay must be a number.\n\033[0m";
        program.showMenu();
        return;
    }

    int year = stoi(sYear);
    int month = stoi(sMonth);
    int day = stoi(sDay);
    if (year < 2023) {
        cout << "\033[31mYear must be greater than or equal to 2023.\n\033[0m";
        program.showMenu();
        return;
    }
    if (month < 1 || month > 12) {
        cout << "\033[31mMonth must be between 1 and 12.\n\033[0m";
        program.showMenu();
        return;
    }
    if (day < 1 || day > 31) {
        cout << "\033[31mDay must be between 1 and 31.\n\033[0m";
        program.showMenu();
        return;
    }
    if (month == 2) {
        if (year % 4 == 0) {
            if (day > 29) {
                cout << "\033[31mDay must be between 1 and 29.\n\033[0m";
                program.showMenu();
                return;
            }
        } else {
            if (day > 28) {
                cout << "\033[31mDay must be between 1 and 28.\n\033[0m";
                program.showMenu();
                return;
            }
        }
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            cout << "\033[31mDay must be between 1 and 30.\n\033[0m";
            program.showMenu();
            return;
        }
    }
}

int Util::checkValidID(string sid) {
    if (!all_of(sid.begin(), sid.end(), ::isdigit)) {
        cout << "\033[31mID must be a number.\n\033[0m";
        printBottomRow();
        program.showMenu();
        return -1;
    }

    if (sid.empty()) {
        cout << "\033[31mID must not be empty.\n\033[0m";
        printBottomRow();
        program.showMenu();
        return -1;
    }

    int id = stoi(sid);
    if (id < 1) {
        cout << "\033[31mID must be greater than or equal to 1.\n\033[0m";
        printBottomRow();
        program.showMenu();
        return -1;
    }
    if (tasks.size() < id) {
        cout << "\033[31mTask not found.\n\033[0m";
        printBottomRow();
        program.showMenu();
        return -1;
    }

    return id;
}

void Util::printWindow(const string &text) {
    printTopRow();
    cout << "| ";
    int chars = 2;
    for (auto c: text) {
        cout << c;
        chars++;
        if (chars == WIDTH - 2) {
            cout << " |\n| ";
            chars = 2;
        }
    }

    while (chars < WIDTH - 1) {
        cout << " ";
        chars++;
    }

    cout << "|\n";
    printBottomRow();
}

void Util::refreshStorage() {
    program.outputSaveStream.close();
    program.outputSaveStream.open("storage.csv", ios::trunc);
    program.outputSaveStream.close();
    program.outputSaveStream.open("storage.csv", ios::app);
    for (const Task& task : tasks) {
        program.outputSaveStream << task.name << "," << task.date << "," << task.description << "," << task.priority
                         << "\n";
    }
    program.outputSaveStream.flush();
}

void Util::sort() {
    if (sortBy == "priority") {
        std::sort(tasks.begin(), tasks.end(), sortByPriority);
    } else {
        std::sort(tasks.begin(), tasks.end(), sortByDate);
    }
}

bool Util::sortByPriority(const Task &t1, const Task &t2) {
    if (t1.priority == "high" && t2.priority == "medium") {
        return true;
    }
    if (t1.priority == "high" && t2.priority == "low") {
        return true;
    }
    if (t1.priority == "medium" && t2.priority == "low") {
        return true;
    }
    return false;
}

bool Util::sortByDate(const Task &t1, const Task &t2) {
    int year1 = stoi(t1.date.substr(0, 4));
    int year2 = stoi(t2.date.substr(0, 4));
    if (year1 < year2) {
        return true;
    }
    if (year1 > year2) {
        return false;
    }
    int month1 = stoi(t1.date.substr(5, 2));
    int month2 = stoi(t2.date.substr(5, 2));
    if (month1 < month2) {
        return true;
    }
    if (month1 > month2) {
        return false;
    }
    int day1 = stoi(t1.date.substr(8, 2));
    int day2 = stoi(t2.date.substr(8, 2));
    if (day1 < day2) {
        return true;
    }
    if (day1 > day2) {
        return false;
    }
    return true;
}