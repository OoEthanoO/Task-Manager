/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 0.5.1
 */

// Credits: aquario

#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

ofstream outputSaveStream;
ifstream inputReadStream;
string currentVersion = "0.5.1";
const vector<string> COMMANDS = {"showCommands", "version", "add", "exit", "clear", "display", "remove", "showDescription"};
const int WIDTH = 70;

class Task {
public:
    string name;
    string date;
    string description;
};

vector<Task> tasks;

class Program {
private:
    void printTopRow() {
        cout << "\n";
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\n";
    }

    void printBottomRow() {
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\n\n";
    }

    void printf(string text) {
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

    int checkValidID(string sid) {
        if (!all_of(sid.begin(), sid.end(), ::isdigit)) {
            cout << "ID must be a number.\n";
            printBottomRow();
            showMenu();
            return -1;
        }
        int id = stoi(sid);
        if (id < 1) {
            cout << "ID must be greater than or equal to 1.\n";
            printBottomRow();
            showMenu();
            return -1;
        }
        if (tasks.size() < id) {
            cout << "Task not found.\n";
            printBottomRow();
            showMenu();
            return -1;
        }

        return id;
    }

    void printWindow(string text) {
        printTopRow();
        cout << "| ";
        int chars = 2;
        for (auto c : text) {
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
public:
    void showMenu() {
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
            }
        } else {
            cout << "Command not found.\n";
            showMenu();
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
        printf("showDescription -> display the description of a task (0.5)");
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

        cout << "Give a short description of the task (Optional): ";
        string description;
        getline(cin, description);
        task.description = description;

        tasks.push_back(task);
        outputSaveStream << task.name << "," << task.date << "," << task.description << "\n";
        outputSaveStream.flush();
        cout << "Task added.\n";
        printBottomRow();
        display();
        showMenu();
    }

    void exitProgram() {
        printTopRow();
        printf("Exiting...");
        outputSaveStream.close();
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
            outputSaveStream.open("storage.csv", ios::trunc);
            outputSaveStream.close();
            outputSaveStream.open("storage.csv", ios::app);
            outputSaveStream.flush();
            tasks.clear();
            cout << "Cleared.\n";
        } else {
            cout << "Cancelled\n";
        }
        printBottomRow();
        display();
        showMenu();
    }

    void initialization() {
        cout << "Initializing...\n";
        string line;
        while (getline(inputReadStream, line)) {
            int state = 0;
            Task task;
            string name;
            string date;
            string description;
            for (char c : line) {
                if (c == ',') {
                    state++;
                    continue;
                }
                if (state == 0) {
                    name += c;
                } else if (state == 1) {
                    date += c;
                } else if (state == 2) {
                    description += c;
                }
            }
            task.name = name;
            task.date = date;
            task.description = description;
            tasks.push_back(task);
        }
        inputReadStream.close();
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
        int id = checkValidID(sid);
        cout << "Are you sure you want to remove this task? (y/n): ";
        string answer;
        getline(cin, answer);
        if (answer == "y") {
            tasks.erase(tasks.begin() + id - 1);
            outputSaveStream.close();
            outputSaveStream.open("storage.csv", ios::trunc);
            outputSaveStream.close();
            outputSaveStream.open("storage.csv", ios::app);
            for (Task task : tasks) {
                outputSaveStream << task.name << "," << task.date << "," << task.description << "\n";
            }
            outputSaveStream.flush();
            cout << "Removed.\n";
        } else {
            cout << "Cancelled\n";
        }
        printBottomRow();
        display();
        showMenu();
    }

    void showDescription() {
        printTopRow();
        cout << "Enter the ID of the task that you want to view the description of: ";
        string sid;
        getline(cin, sid);
        printBottomRow();
        int id = checkValidID(sid);
        string description = tasks[id - 1].description;
        if (description.empty()) {
            printf("No description found.");
            printBottomRow();
            showMenu();
            return;
        }
        printWindow(description);
        showMenu();
    }
};

int main() {
    outputSaveStream.open("storage.csv", ios::app);
    inputReadStream.open("storage.csv");
    Program program;
    program.initialization();
    program.display();
    program.showMenu();
}
