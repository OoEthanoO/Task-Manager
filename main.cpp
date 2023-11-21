/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 0.3
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ofstream outputSaveStream;
ifstream inputReadStream;
string currentVersion = "0.4";
const static vector<string> COMMANDS = {"showCommands", "version", "add", "exit", "clear", "display", "remove"};
const int WIDTH = 60;

class Task {
public:
    string name;
    string date;
};

vector<Task> tasks;

class Program {
private:
    static void printTopRow() {
        cout << "\n";
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\n";
    }

    static void printBottomRow() {
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\n\n";
    }

    static void printf(const string &text) {
        int size = text.size();
        int width = WIDTH;
        width -= 3;
        cout << "| ";
        cout << text;
        for (int i = 0; i < (int) width - size; i++) {
            cout << " ";
        }

        cout << "|";
        cout << "\n";
    }

public:
    void checkDate(string date) {
        if (date.size() != 10) {
            cout << "Date must be in the format of YYYY-MM-DD.\n";
            showMenu();
            return;
        }
        if (date[4] != '-' || date[7] != '-') {
            cout << "Date must be in the format of YYYY-MM-DD.\n";
            showMenu();
            return;
        }
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        if (year < 2023) {
            cout << "Year must be greater than or equal to 2023.\n";
            showMenu();
            return;
        }
        if (month < 1 || month > 12) {
            cout << "Month must be between 1 and 12.\n";
            showMenu();
            return;
        }
        if (day < 1 || day > 31) {
            cout << "Day must be between 1 and 31.\n";
            showMenu();
            return;
        }
        if (month == 2) {
            if (year % 4 == 0) {
                if (day > 29) {
                    cout << "Day must be between 1 and 29.\n";
                    showMenu();
                    return;
                }
            } else {
                if (day > 28) {
                    cout << "Day must be between 1 and 28.\n";
                    showMenu();
                    return;
                }
            }
        }
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) {
                cout << "Day must be between 1 and 30.\n";
                showMenu();
                return;
            }
        }
    }

    void showMenu() {
        cout << "Execute a command here (showCommands to show COMMANDS): ";
        string command;
        getline(cin, command);
        auto it = find(COMMANDS.begin(), COMMANDS.end(), command);
        if (COMMANDS.end() == it) {
            cout << "Command not found.\n";
            showMenu();
        }
        int commandIndex = it - COMMANDS.begin();

        switch (commandIndex) {
            case 0:
                showCommands();
            case 1:
                version();
            case 2:
                add();
            case 3:
                exitProgram();
            case 4:
                clear();
            case 5:
                display();
            case 6:
                remove();
            default:
                cout << "Command not found.\n";
        }
    }

    void showCommands() {
        printTopRow();
        printf("Here are the list of commands:");
        printf("showCommands -> show all commands available (0.1)");
        printf("version -> show version of program (0.1)");
        printf("add -> add a task (0.2)");
        printf("exit -> exit program (0.2)");
        printf("clear -> clear all tasks (0.3)");
        printf("display -> display all tasks (0.3)");
        printf("remove -> remove a task (0.3)");
        printBottomRow();
        showMenu();
    }

    void version() {
        printTopRow();
        printf("Task Manager");
        printf("Version: " + currentVersion);
        printBottomRow();
        showMenu();
    }

    void add() {
        printTopRow();
        Task task;
        cout << "Enter the name of the task: ";
        string name;
        getline(cin, name);
        task.name = name;
        cout << "Enter the date of the task (YYYY-MM-DD): ";
        string date;
        getline(cin, date);
        checkDate(date);
        task.date = date;
        tasks.push_back(task);
        outputSaveStream << task.name << "\n";
        cout << "Task added.\n";
        printBottomRow();
        showMenu();
    }

    void exitProgram() {
        printTopRow();
        printf("Exiting...");
        outputSaveStream.close();
        inputReadStream.close();
        printBottomRow();
        exit(0);
    }

    void clear() {
        printTopRow();
        cout << "Are you sure you want to clear all tasks? (y/n): ";
        string answer;
        getline(cin, answer);
        if (answer == "y") {
            outputSaveStream.close();
            inputReadStream.close();
            outputSaveStream.open("storage.csv", ios::trunc);
            outputSaveStream.close();
            outputSaveStream.open("storage.csv", ios::app);
            inputReadStream.open("storage.csv");
            tasks.clear();
            cout << "Cleared.\n";
        } else {
            cout << "Cancelled\n";
        }
        printBottomRow();
        showMenu();
    }

    void initialization() {
        cout << "Initializing...\n";
        string line;
        while (getline(inputReadStream, line)) {
            Task task;
            string name = "";
            for (char c : line) {
                name += c;
            }
            task.name = name;
            tasks.push_back(task);
        }
        cout << "Initialized.\n";
    }

    void display() {
        printTopRow();
        if (tasks.empty()) {
            printf("No tasks found.");
            printBottomRow();
            showMenu();
            return;
        }
        printf("Here are the list of tasks:");
        printf("ID | Task Name | Due Date");
        int id = 1;
        for (Task task : tasks) {
            printf(to_string(id) + " | " + task.name + " | " + task.date);
            id++;
        }
        printBottomRow();
        showMenu();
    }

    void remove() {
        printTopRow();
        string sid;
        cout << "Enter the ID of the task you want to remove: ";
        getline(cin, sid);
        if (!all_of(sid.begin(), sid.end(), ::isdigit)) {
            cout << "ID must be a number.\n";
            printBottomRow();
            showMenu();
            return;
        }
        int id = stoi(sid);
        if (tasks.size() < id) {
            cout << "Task not found.\n";
            printBottomRow();
            showMenu();
            return;
        }
        cout << "Are you sure you want to remove this task? (y/n): ";
        string answer;
        getline(cin, answer);
        if (answer == "y") {
            tasks.erase(tasks.begin() + id - 1);
            cout << "Removed.\n";
        } else {
            cout << "Cancelled\n";
        }
        printBottomRow();
        showMenu();
    }
};

int main() {
    outputSaveStream.open("storage.csv", ios::app);
    inputReadStream.open("storage.csv");
    Program program;
    program.initialization();
    program.showMenu();
}