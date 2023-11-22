/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.1
 */

// Credits: aquario

#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

ofstream outputSaveStream;
ifstream inputReadStream;
string currentVersion = "1.1";
string sortBy = "priority";
const vector<string> COMMANDS = {"showCommands", "version", "add", "exit", "clear", "display", "remove", "showDescription", "edit", "toggleSortBy"};
const int WIDTH = 70;

class Task {
public:
    string name;
    string date;
    string description;
    string priority;
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

        if (sid.empty()) {
            cout << "ID must not be empty.\n";
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

    void refreshStorage() {
        outputSaveStream.close();
        outputSaveStream.open("storage.csv", ios::trunc);
        outputSaveStream.close();
        outputSaveStream.open("storage.csv", ios::app);
        for (Task task : tasks) {
            outputSaveStream << task.name << "," << task.date << "," << task.description << "," << task.priority << "\n";
        }
        outputSaveStream.flush();
    }

    void sort() {
        if (sortBy == "priority") {
            std::sort(tasks.begin(), tasks.end(), sortByPriority);
        } else {
            std::sort(tasks.begin(), tasks.end(), sortByDate);
        }
    }

    static bool sortByPriority(Task t1, Task t2) {
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

    static bool sortByDate(Task t1, Task t2) {
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
                case 8:
                    edit();
                    break;
                case 9:
                    toggleSortBy();
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
        printf("edit -> edit a task (0.7)");
        printf("toggleSortBy -> toggle sort mode between priority and date (1.1)");
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

        cout << "Enter the priority of the task (low, medium (default), high): ";
        string priority;
        getline(cin, priority);
        if (priority == "low" || priority == "medium" || priority == "high") {
            task.priority = priority;
        } else {
            task.priority = "medium";
        }

        tasks.push_back(task);
        sort();
        outputSaveStream << task.name << "," << task.date << "," << task.description << "," << task.priority << "\n";
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
            for (char c : line) {
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
        sort();

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
        printf("Sort by: " + sortBy);
        printf("ID | Task Name | Due Date | Priority");
        int id = 1;
        for (Task task : tasks) {
            printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
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
            refreshStorage();
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

    void edit() {
        printTopRow();
        cout << "Enter the ID of the task that you want to edit: ";
        string sid;
        getline(cin, sid);
        int id = checkValidID(sid);
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
            checkDate(date);
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
        sort();
        refreshStorage();
        cout << "Edited.\n";
        printBottomRow();
        display();
        showMenu();
    }

    void toggleSortBy() {
        sortBy = sortBy == "priority" ? "date" : "priority";
        sort();
        display();
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
