/*
 * Author: Ethan Xu
 * Project Start Date: November 21, 2023
 * Version Number: 1.3
 */

// Contribution: aqariio

#include <iostream>
#include "Program.h"

using namespace std;

int main() {
    Program program = Program();
    program.outputSaveStream.open("storage.csv", ios::app);
    program.inputReadStream.open("storage.csv");
    program.init();
    program.display();
    program.showMenu();
}
