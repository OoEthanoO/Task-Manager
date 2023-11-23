/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.5
 */

// Contribution: aqariio

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <termios.h>
#include <unistd.h>

using namespace std;

ofstream outputSaveStream;
ifstream inputReadStream;
string currentVersion = "1.5";
string sortBy = "priority";
string filename;
const vector<string> COMMANDS = {"showcommands", "version", "add", "exit", "clear", "display", "remove", "showdescription", "edit", "togglesortby", "search", "erase", "logout", "filter"};
const int WIDTH = 70;

class Task {
public:
    string name;
    string date;
    string description;
    string priority;
    string tag;
};

vector<Task> tasks;

class Program {
private:
    static void printTopRow() {
        cout << "\n";
        cout << "\033[34m";
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\033[0m";
        cout << "\n";
    }

    static void printBottomRow() {
        cout << "\033[34m";
        for (int i = 0; i < WIDTH; i++) {
            cout << "-";
        }
        cout << "\033[0m";
        cout << "\n\n";
    }

    static void printf(string text) {
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

    static void printf(string text, int size) {
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

    void checkDate(string date) {
        if (date.size() != 10) {
            cout << "\033[31mDate must be in the format of YYYY-MM-DD.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        if (date[4] != '-' || date[7] != '-') {
            cout << "\033[31mDate must be in the format of YYYY-MM-DD.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }

        string sYear = date.substr(0, 4);
        if (!all_of(sYear.begin(), sYear.end(), ::isdigit)) {
            cout << "\033[31mYear must be a number.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        string sMonth = date.substr(5, 2);
        if (!all_of(sMonth.begin(), sMonth.end(), ::isdigit)) {
            cout << "\033[31mMonth must be a number.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        string sDay = date.substr(8, 2);
        if (!all_of(sDay.begin(), sDay.end(), ::isdigit)) {
            cout << "\033[31mDay must be a number.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }

        int year = stoi(sYear);
        int month = stoi(sMonth);
        int day = stoi(sDay);
        if (year < 2023) {
            cout << "\033[31mYear must be greater than or equal to 2023.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        if (month < 1 || month > 12) {
            cout << "\033[31mMonth must be between 1 and 12.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        if (day < 1 || day > 31) {
            cout << "\033[31mDay must be between 1 and 31.\n\033[0m";
            printBottomRow();
            showMenu();
            return;
        }
        if (month == 2) {
            if (year % 4 == 0) {
                if (day > 29) {
                    cout << "\033[31mDay must be between 1 and 29.\n\033[0m";
                    printBottomRow();
                    showMenu();
                    return;
                }
            } else {
                if (day > 28) {
                    cout << "\033[31mDay must be between 1 and 28.\n\033[0m";
                    printBottomRow();
                    showMenu();
                    return;
                }
            }
        }
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) {
                cout << "\033[31mDay must be between 1 and 30.\n\033[0m";
                printBottomRow();
                showMenu();
                return;
            }
        }
    }

    int checkValidID(string sid) {
        if (!all_of(sid.begin(), sid.end(), ::isdigit)) {
            cout << "\033[31mID must be a number.\n\033[0m";
            printBottomRow();
            showMenu();
            return -1;
        }

        if (sid.empty()) {
            cout << "\033[31mID must not be empty.\n\033[0m";
            printBottomRow();
            showMenu();
            return -1;
        }

        int id = stoi(sid);
        if (id < 1) {
            cout << "\033[31mID must be greater than or equal to 1.\n\033[0m";
            printBottomRow();
            showMenu();
            return -1;
        }
        if (tasks.size() < id) {
            cout << "\033[31mTask not found.\n\033[0m";
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
        outputSaveStream.open(filename, ios::trunc);
        outputSaveStream.close();
        outputSaveStream.open(filename, ios::app);
        for (Task task : tasks) {
            outputSaveStream << "\"" << task.name << "\"" << "," << "\"" << task.date << "\"" << "," << "\"" << task.description << "\"" << "," << "\"" << task.priority << "\"" << "," << "\"" << task.tag << "\"" << "\n";
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

    void toLowerCase(string &s) {
        for (char &c : s) {
            c = tolower(c);
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
        toLowerCase(command);
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
                    initialization();
                    break;
                case 13:
                    filter();
                    break;
            }
        } else {
            cout << "\033[31mCommand not found.\n\033[0m";
            showMenu();
        }
    }

    void showCommands() {
        printTopRow();
        printf("Here are the list of commands:");
        printf("showcommands -> show all commands available (0.1)");
        printf("version -> show version of program (0.1)");
        printf("add -> add a task (0.2)");
        printf("exit -> exit program (0.2)");
        printf("clear -> clear all tasks (0.3)");
        printf("display -> display all tasks (0.3)");
        printf("remove -> remove a task (0.3)");
        printf("showdescription -> display the description of a task (0.5)");
        printf("edit -> edit a task (0.7)");
        printf("togglesortby -> toggle sort mode between priority and date (1.1)");
        printf("search -> search for a task (1.2)");
        printf("erase -> erase account (1.4)");
        printf("logout -> logout (1.4)");
        printf("filter -> filter by tag (1.5)");
        printBottomRow();
        showMenu();
    }

    void version() {
        printTopRow();
        printf("\033[31mT\033[32ma\033[33ms\033[34mk \033[31mM\033[32ma\033[33mn\033[34ma\033[35mg\033[36me\033[37mr\033[0m", 12);
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

        cout << "Give it a tag (red, orange, yellow, green, blue, purple, gray) (Optional): ";
        string tag;
        getline(cin, tag);
        if (tag == "red" || tag == "orange" || tag == "yellow" || tag == "green" || tag == "blue" || tag == "purple" || tag == "gray") {
            task.tag = tag;
        } else {
            task.tag = "";
        }

        tasks.push_back(task);
        sort();
        outputSaveStream << "\"" << task.name << "\"" << "," << "\"" << task.date << "\"" << "," << "\"" << task.description << "\"" << "," << "\"" << task.priority << "\"" << "," << "\"" << task.tag << "\"" << "\n";
        outputSaveStream.flush();
        cout << "\033[32mTask added.\n\033[0m";
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
            outputSaveStream.open(filename, ios::trunc);
            outputSaveStream.close();
            outputSaveStream.open(filename, ios::app);
            outputSaveStream.flush();
            tasks.clear();
            cout << "\033[32mCleared.\n\033[0m";
        } else {
            cout << "\033[31mCancelled\n\033[0m";
        }
        printBottomRow();
        display();
        showMenu();
    }

    void initialization() {
        cout << "Initializing...\n";
        inputReadStream.open(filename);
        outputSaveStream.open(filename, ios::app);
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
        sort();

        inputReadStream.close();
        cout << "\033[32mInitialized.\n\033[0m";
        display();
        showMenu();
    }

    void display() {
        printTopRow();
        if (tasks.empty()) {
            printf("\033[31mNo tasks found.\033[0m", 15);
            printBottomRow();
            showMenu();
            return;
        }
        printf("Here are the list of tasks:");
        printf("Sort by: " + sortBy);
        printf("ID | Task Name | Due Date | Priority | Tag");
        int id = 1;
        for (Task task : tasks) {
            printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority + " | " + task.tag);
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
            cout << "\033[32mRemoved.\n\033[0m";
        } else {
            cout << "\033[31mCancelled\n\033[0m";
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
            printf("\033[31mNo description found.\033[0m");
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
        cout << "\033[32mEdited.\n\033[0m";
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

    void search() {
        printTopRow();
        cout << "Enter the keyword you want to search for: ";
        string keyword;
        getline(cin, keyword);
        printf("ID | Task Name | Due Date | Priority");
        int id = 1;
        bool found = false;
        for (Task task : tasks) {
            if (task.name.find(keyword) != string::npos) {
                printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
                found = true;
            }
            id++;
        }

        if (!found) {
            printf("\033[31mNo tasks found.\033[0m", 15);
        }
        printBottomRow();
        showMenu();
    }

    void askForLogin() {
        printTopRow();
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
            printBottomRow();
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
                    printBottomRow();
                    return;
                } else {
                    cout << "\033[31mIncorrect password.\n\033[0m";
                    printBottomRow();
                    askForLogin();
                    return;
                }
            }
        }
        cout << "\033[31mUsername not found.\n\033[0m";
        askForLogin();
    }

    void erase() {
        printTopRow();
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
                        printBottomRow();
                        exitProgram();
                        return;
                    } else {
                        cout << "\033[31mIncorrect password.\n\033[0m";
                        printBottomRow();
                        erase();
                        return;
                    }
                }
            }
            cout << "\033[31mUsername not found.\n\033[0m";
            printBottomRow();
            erase();
        } else {
            cout << "\033[31mCancelled\n\033[0m";
            printBottomRow();
            showMenu();
        }
    }

    void filter() {
        printTopRow();
        cout << "Enter the tag you want to filter for: ";
        string tag;
        getline(cin, tag);
        printf("ID | Task Name | Due Date | Priority");
        int id = 1;
        bool found = false;
        for (Task task : tasks) {
            if (task.tag.find(tag) != string::npos) {
                printf(to_string(id) + " | " + task.name + " | " + task.date + " | " + task.priority);
                found = true;
            }
            id++;
        }

        if (!found) {
            printf("\033[31mNo tasks found.\033[0m", 15);
        }
        printBottomRow();
        showMenu();
    }
};


int main() {
    Program program;
    program.askForLogin();
    program.initialization();
}
