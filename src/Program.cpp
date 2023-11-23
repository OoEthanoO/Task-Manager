/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.3
 */

// Contribution: aqariio

#include "Program.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Util.h"
#include "Task.h"

using namespace std;

string currentVersion = "1.3";
const vector<string> COMMANDS = {"showCommands", "version", "add", "exit", "clear", "display", "remove",
                                 "showDescription", "edit", "toggleSortBy", "search"};

void Program::showMenu() {
    cout << "Execute a command here (showCommands to show commands): ";
    string command;
    getline(cin, command);
    int index = find(COMMANDS.begin(), COMMANDS.end(), command) - COMMANDS.begin();
    if (index < COMMANDS.size()) {
        switch (index) {
            case 0:
                showCommands();
                break;
            case 1:
                version();
                break;
            case 2:
                add();
                break;
            case 3:
                exitProgram();
                break;
            case 4:
                clear();
                break;
            case 5:
                display();
                break;
            case 6:
                remove();
                break;
            case 7:
                showDescription();
                break;
            case 8:
                edit();
                break;
            case 9:
                toggleSortBy();
                break;
            case 10:
                search();
                break;
        }
    } else {
        cout << "\033[31mCommand not found.\n\033[0m";
        showMenu();
    }
}

void Program::showCommands() {
    Util::printTopRow();
    printf("Here are the list of commands:");
    printf("showCommands -> show all commands available (0.1)");
    printf("version -> show version of program (0.1)");
    printf("add -> add a task (0.2)");
    printf("exit -> exit program (0.2)");
    printf("clear -> clear all tasks (0.3)");
    printf("display -> display all tasks (0.3)");
    printf("remove -> remove a task (0.3)");
    printf("showDescription -> display the description of a task (0.5)");
    printf("edit -> edit a task (0.7)");
    printf("toggleSortBy -> toggle sort mode between priority and date (1.1)");
    printf("search -> search for a task (1.2)");
    Util::printBottomRow();
    showMenu();
}

void Program::version() {
    Util::printTopRow();
    Util::printf("\033[31mT\033[32ma\033[33ms\033[34mk \033[31mM\033[32ma\033[33mn\033[34ma\033[35mg\033[36me\033[37mr\033[0m",
           12);
    Util::printf("Version: " + currentVersion);
    Util::printBottomRow();
    showMenu();
}

void Program::add() {
    Util::printTopRow();
    Task task;

    cout << "Enter the name of the task: ";
    string name;
    getline(cin, name);
    task.name = name;

    cout << "Enter the date of the task (YYYY-MM-DD): ";
    string date;
    getline(cin, date);
    Util::checkDate(date);
    task.date = date;

    cout << "Give a short description of the task (Optional): ";
    string description;
    getline(cin, description);
    task.description = description;

    cout << "Enter the priority of the task (low, medium (default), high): ";
    string priority;
    getline(cin, priority);
    if (priority == "low" || priority == "medium" || priority == "high") {
        task.priority = priority;
    } else {
        task.priority = "medium";
    }

    tasks.push_back(task);
    Util::sort();
    this->outputSaveStream << task.name << "," << task.date << "," << task.description << "," << task.priority << "\n";
    this->outputSaveStream.flush();
    cout << "\033[32mTask added.\n\033[0m";
    Util::printBottomRow();
    display();
    showMenu();
}

void Program::exitProgram() {
    Util::printTopRow();
    printf("Exiting...");
    this->outputSaveStream.close();
    Util::printBottomRow();
    exit(0);
}

void Program::clear() {
    Util::printTopRow();
    cout << "Are you sure you want to clear all tasks? (y/n): ";
    string answer;
    getline(cin, answer);
    if (answer == "y") {
        this->outputSaveStream.close();
        this->outputSaveStream.open("storage.csv", ios::trunc);
        this->outputSaveStream.close();
        this->outputSaveStream.open("storage.csv", ios::app);
        this->outputSaveStream.flush();
        tasks.clear();
        cout << "\033[32mCleared.\n\033[0m";
    } else {
        cout << "\033[31mCancelled\n\033[0m";
    }
    Util::printBottomRow();
    display();
    showMenu();
}

void Program::init() {
    cout << "Initializing...\n";
    string line;
    while (getline(this->inputReadStream, line)) {
        int state = 0;
        Task task;
        for (char c: line) {
            if (c == ',') {
                state++;
                continue;
            }
            if (state == 0) {
                task.name += c;
            } else if (state == 1) {
                task.date += c;
            } else if (state == 2) {
                task.description += c;
            } else if (state == 3) {
                task.priority += c;
            }
        }
        tasks.push_back(task);
    }
    Util::sort();

    this->inputReadStream.close();
    cout << "\033[32mInitialized.\n\033[0m";
}

void Program::display() {
    Util::printTopRow();
    if (tasks.empty()) {
        printf("\033[31mNo tasks found.\033[0m");
        Util::printBottomRow();
        showMenu();
        return;
    }
    printf("Here are the list of tasks:");
    Util::printf("Sort by: " + Util::sortBy);
    printf("ID | Task Name | Due Date | Priority");
    int id = 1;
    for (const Task& task: tasks) {
        Util::printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
        id++;
    }
    Util::printBottomRow();
    showMenu();
}

void Program::remove() {
    Util::printTopRow();
    string sid;
    cout << "Enter the ID of the task you want to remove: ";
    getline(cin, sid);
    int id = Util::checkValidID(sid);
    cout << "Are you sure you want to remove this task? (y/n): ";
    string answer;
    getline(cin, answer);
    if (answer == "y") {
        tasks.erase(tasks.begin() + id - 1);
        Util::refreshStorage();
        cout << "\033[32mRemoved.\n\033[0m";
    } else {
        cout << "\033[31mCancelled\n\033[0m";
    }
    Util::printBottomRow();
    display();
    showMenu();
}

void Program::showDescription() {
    Util::printTopRow();
    cout << "Enter the ID of the task that you want to view the description of: ";
    string sid;
    getline(cin, sid);
    Util::printBottomRow();
    int id = Util::checkValidID(sid);
    string description = tasks[id - 1].description;
    if (description.empty()) {
        printf("\033[31mNo description found.\033[0m");
        Util::printBottomRow();
        showMenu();
        return;
    }
    Util::printWindow(description);
    showMenu();
}

void Program::edit() {
    Util::printTopRow();
    cout << "Enter the ID of the task that you want to edit: ";
    string sid;
    getline(cin, sid);
    int id = Util::checkValidID(sid);
    Task task = tasks[id - 1];

    cout << "Enter the new name of the task (Optional): ";
    string name;
    getline(cin, name);
    if (!name.empty()) {
        task.name = name;
    }

    cout << "Enter the new date of the task (Optional): ";
    string date;
    getline(cin, date);
    if (!date.empty()) {
        Util::checkDate(date);
        task.date = date;
    }

    cout << "Enter the new description of the task (Optional): ";
    string description;
    getline(cin, description);
    if (!description.empty()) {
        task.description = description;
    }

    cout << "Enter the new priority of the task (Optional): ";
    string priority;
    getline(cin, priority);
    if (!priority.empty()) {
        if (priority == "low" || priority == "medium" || priority == "high") {
            task.priority = priority;
        } else {
            task.priority = "medium";
        }
    }

    tasks[id - 1] = task;
    Util::sort();
    Util::refreshStorage();
    cout << "\033[32mEdited.\n\033[0m";
    Util::printBottomRow();
    display();
    showMenu();
}

void Program::toggleSortBy() {
    Util::sortBy = Util::sortBy == "priority" ? "date" : "priority";
    Util::sort();
    display();
    showMenu();
}

void Program::search() {
    Util::printTopRow();
    cout << "Enter the keyword you want to search for: ";
    string keyword;
    getline(cin, keyword);
    printf("ID | Task Name | Due Date | Priority");
    int id = 1;
    bool found = false;
    for (Task task: tasks) {
        if (task.name.find(keyword) != string::npos) {
            Util::printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
            found = true;
        }
        id++;
    }

    if (!found) {
        printf("\033[31mNo tasks found.\033[0m");
    }
    Util::printBottomRow();
    showMenu();
}