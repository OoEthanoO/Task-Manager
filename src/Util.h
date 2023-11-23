/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.3
 */

// Contribution: aqariio

#ifndef TASK_MANAGER_UTIL_H
#define TASK_MANAGER_UTIL_H
#include <iostream>
#include <algorithm>
#include <string>
#include "Task.h"

using namespace std;

class Util {
public:
    static string sortBy;

    static void printTopRow();

    static void printBottomRow();

    static void printf(const string &text);

    static void printf(const string &text, int size);

    static void checkDate(string date);

    static int checkValidID(string sid);

    static void printWindow(const string &text);

    static void refreshStorage();

    static void toLowerCase(string &s);

    static void sort();

    static bool sortByPriority(const Task &t1, const Task &t2);

    static bool sortByDate(const Task &t1, const Task &t2);
};


#endif //TASK_MANAGER_UTIL_H
