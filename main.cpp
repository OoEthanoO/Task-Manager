/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 */

#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

ifstream idStream;
ofstream outputSaveStream;
ifstream inputReadStream;
string currentVersion = "0.3";
vector<string> commands = {"showCommands", "version", "add", "exit", "clear", "display", "remove"};
const int WIDTH = 60;

class Task {
public:
    string name;
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

public:
    void showMenu() {
        cout << "Execute a command here (showCommands to show commands): ";
        string command;
        cin >> command;
        if (commands.end() != find(commands.begin(), commands.end(), command)) {
            if (command == "showCommands") {
                showCommands();
            } else if (command == "version") {
                version();
            } else if (command == "add") {
                add();
            } else if (command == "exit") {
                exitProgram();
            } else if (command == "clear") {
                clear();
            } else if (command == "display") {
                display();
            } else if (command == "remove") {
                remove();
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
        cin.ignore();
        getline(cin, name);
        task.name = name;
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
        cin >> answer;
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
            string id = "";
            string name = "";
            bool isId = true;
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
        printf("ID | Task Name                     ");
        int id = 1;
        for (Task task : tasks) {
            printf(to_string(id) + " | " + task.name);
            id++;
        }
        printBottomRow();
        showMenu();
    }

    void remove() {
        printTopRow();
        string sid;
        cout << "Enter the ID of the task you want to remove: ";
        cin >> sid;
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
        cin >> answer;
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
    idStream.open("id.txt");
    Program program;
    program.initialization();
    program.showMenu();
}
