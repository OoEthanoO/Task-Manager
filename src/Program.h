/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.3
 */

// Contribution: aqariio

#ifndef TASK_MANAGER_PROGRAM_H
#define TASK_MANAGER_PROGRAM_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Task.h"

using namespace std;

class Program {
public:
    ofstream outputSaveStream;

    ifstream inputReadStream;

    vector<Task> tasks;

    void showMenu();

    void showCommands();

    void version();

    void add();

    void exitProgram();

    void clear();

    void display();

    void remove();

    void showDescription();

    void edit();

    void toggleSortBy();

    void search();

    void init();
};


#endif //TASK_MANAGER_PROGRAM_H
