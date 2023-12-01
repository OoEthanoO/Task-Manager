/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.5.1
 */

// Contribution: aqariio

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Program.h"
#include "Util.h"
#include "Task.h"
#include <termios.h>
#include <unistd.h>

using namespace std;

string currentVersion = "1.6";
string Program::filename;
const vector<string> COMMANDS = {"showcommands", "version", "add", "exit", "clear", "display", "remove",
                                 "showdescription", "edit", "togglesortby", "search", "erase", "logout", "filter"};

void Program::showMenu() {
    cout << "Execute a command here (showCommands to show commands): ";
    string command;
    getline(cin, command);
    Util::toLowerCase(command);
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
            case 11:
                erase();
                break;
            case 12:
                tasks.clear();
                cout << "\033[32mLogged out.\n\033[0m";
                askForLogin();
                init();
                break;
            case 13:
                filter();
                break;
            default:
                cout << "\033[31mCommand not found.\n\033[0m";
                showMenu();
                break;
        }
    } else {
        cout << "\033[31mCommand not found.\n\033[0m";
        showMenu();
    }
}

void Program::showCommands() {
    Util::printTopRow();
    Util::printf("Here are the list of commands:");
    Util::printf("showcommands -> show all commands available (0.1)");
    Util::printf("version -> show version of program (0.1)");
    Util::printf("add -> add a task (0.2)");
    Util::printf("exit -> exit program (0.2)");
    Util::printf("clear -> clear all tasks (0.3)");
    Util::printf("display -> display all tasks (0.3)");
    Util::printf("remove -> remove a task (0.3)");
    Util::printf("showdescription -> display the description of a task (0.5)");
    Util::printf("edit -> edit a task (0.7)");
    Util::printf("togglesortby -> toggle sort mode between priority and date (1.1)");
    Util::printf("search -> search for a task (1.2)");
    Util::printf("erase -> erase account (1.4)");
    Util::printf("logout -> logout (1.4)");
    Util::printf("filter -> filter by tag (1.5)");
    Util::printBottomRow();
    showMenu();
}

void Program::version() {
    Util::printTopRow();
    Util::printf(
            "\033[31mT\033[32ma\033[33ms\033[34mk \033[31mM\033[32ma\033[33mn\033[34ma\033[35mg\033[36me\033[37mr\033[0m",
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

    cout << "Give it a tag (red, orange, yellow, green, blue, purple, gray) (Optional): ";
    string tag;
    getline(cin, tag);
    if (tag == "red" || tag == "orange" || tag == "yellow" || tag == "green" || tag == "blue" || tag == "purple" || tag == "gray") {
        task.tag = tag;
    } else {
        task.tag = "";
    }

    tasks.push_back(task);
    Util::sort();
    outputSaveStream << "\"" << task.name << "\"" << "," << "\"" << task.date << "\"" << "," << "\"" << task.description << "\"" << "," << "\"" << task.priority << "\"" << "," << "\"" << task.tag << "\"" << "\n";
    outputSaveStream.flush();
    cout << "\033[32mTask added.\n\033[0m";
    Util::printBottomRow();
    display();
    showMenu();
}

void Program::exitProgram() {
    Util::printTopRow();
    Util::printf("Exiting...");
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
        this->outputSaveStream.open(filename, ios::trunc);
        this->outputSaveStream.close();
        this->outputSaveStream.open(filename, ios::app);
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
    inputReadStream.open(filename);
    outputSaveStream.open(filename, ios::app);
    string line;
    while (getline(inputReadStream, line)) {
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
            } else if (state == 4) {
                task.tag += c;
            }
        }
        task.name.erase(0, 1);
        task.name.erase(task.name.size() - 1);
        task.date.erase(0, 1);
        task.date.erase(task.date.size() - 1);
        task.description.erase(0, 1);
        task.description.erase(task.description.size() - 1);
        task.priority.erase(0, 1);
        task.priority.erase(task.priority.size() - 1);
        task.tag.erase(0, 1);
        task.tag.erase(task.tag.size() - 1);
        tasks.push_back(task);
    }
    Util::sort();

    this->inputReadStream.close();
    cout << "\033[32mInitialized.\n\033[0m";
    display();
    showMenu();
}

void Program::display() {
    Util::printTopRow();
    if (tasks.empty()) {
        Util::printf("\033[31mNo tasks found.\033[0m", 15);
        Util::printBottomRow();
        showMenu();
        return;
    }
    Util::printf("Here are the list of tasks:");
    Util::printf("Sort by: " + Util::sortBy);
    Util::printf("ID | Task Name | Due Date | Priority | Tag");
    int id = 1;
    for (const Task &task: tasks) {
        Util::printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority + " | " + task.tag);
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
        Util::printf("\033[31mNo description found.\033[0m");
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
    Util::printf("ID | Task Name | Due Date | Priority");
    int id = 1;
    bool found = false;
    for (const Task& task: tasks) {
        if (task.name.find(keyword) != string::npos) {
            Util::printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
            found = true;
        }
        id++;
    }

    if (!found) {
        Util::printf("\033[31mNo tasks found.\033[0m", 15);
    }
    Util::printBottomRow();
    showMenu();
}


void Program::askForLogin() {
    Util::printTopRow();
    cout << "Enter your username (enter to create new account): ";
    string username;
    getline(cin, username);
    if (username.empty()) {
        cout << "Enter your new username: ";
        getline(cin, username);
        cout << "Enter your new password: ";
        string password;
        getline(cin, password);
        ofstream outputUsernameStream;
        outputUsernameStream.open("username.csv", ios::app);
        outputUsernameStream << username << "," << password << "\n";
        outputUsernameStream.flush();
        outputUsernameStream.close();
        cout << "\033[32mAccount created.\n\033[0m";
        Util::printBottomRow();
        askForLogin();
        return;
    }
    ifstream inputUsernameStream;
    inputUsernameStream.open("username.csv");
    string line;
    while (getline(inputUsernameStream, line)) {
        if (line.find(username) != string::npos) {
            line.erase(0, username.size() + 1);
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            termios newt = oldt;
            newt.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            cout << "Enter your password: ";
            string password;
            getline(cin, password);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            if (line.find(password) != string::npos) {
                cout << "\033[32mLogin successful.\n\033[0m";
                filename = username + ".csv";
                inputUsernameStream.close();
                Util::printBottomRow();
                return;
            } else {
                cout << "\033[31mIncorrect password.\n\033[0m";
                Util::printBottomRow();
                askForLogin();
                return;
            }
        }
    }
    cout << "\033[31mUsername not found.\n\033[0m";
    askForLogin();
}

void Program::erase() {
    Util::printTopRow();
    cout << "Are you sure you want to permanently erase this account? (y/n): ";
    string answer;
    getline(cin, answer);
    if (answer == "y") {
        cout << "Enter your username: ";
        string username;
        getline(cin, username);
        cout << "Enter your password: ";
        string password;
        getline(cin, password);
        ifstream inputUsernameStream;
        inputUsernameStream.open("username.csv");
        string line;
        while (getline(inputUsernameStream, line)) {
            if (line.find(username) != string::npos) {
                line.erase(0, username.size() + 1);
                if (line.find(password) != string::npos) {
                    char charfile[username.size() + 4];
                    strcpy(charfile, username.c_str());
                    strcat(charfile, ".csv");
                    std::remove(charfile);
                    ifstream inputUsernameStream2;
                    inputUsernameStream2.open("username.csv");
                    string line2;
                    ofstream outputUsernameStream;
                    outputUsernameStream.open("username2.csv", ios::app);
                    while (getline(inputUsernameStream2, line2)) {
                        if (line2.find(username) == string::npos) {
                            outputUsernameStream << line2 << "\n";
                        }
                    }
                    outputUsernameStream.flush();
                    outputUsernameStream.close();
                    inputUsernameStream2.close();
                    std::remove("username.csv");
                    std::rename("username2.csv", "username.csv");
                    cout << "\033[32mAccount erased.\n\033[0m";
                    Util::printBottomRow();
                    exitProgram();
                    return;
                } else {
                    cout << "\033[31mIncorrect password.\n\033[0m";
                    Util::printBottomRow();
                    erase();
                    return;
                }
            }
        }
        cout << "\033[31mUsername not found.\n\033[0m";
        Util::printBottomRow();
        erase();
    } else {
        cout << "\033[31mCancelled\n\033[0m";
        Util::printBottomRow();
        showMenu();
    }
}

void Program::filter() {
    Util::printTopRow();
    cout << "Enter the tag you want to filter for: ";
    string tag;
    getline(cin, tag);
    Util::printf("ID | Task Name | Due Date | Priority");
    int id = 1;
    bool found = false;
    for (const Task& task : tasks) {
        if (task.tag.find(tag) != string::npos) {
            Util::printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
            found = true;
        }
        id++;
    }

    if (!found) {
        Util::printf("\033[31mNo tasks found.\033[0m", 15);
    }
    Util::printBottomRow();
    showMenu();
}