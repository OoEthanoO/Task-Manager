# Task Manager Console Application

## Overview

This Task Manager Console Application is a simple tool for managing tasks, allowing users to add, view, edit, and remove tasks with ease. The application stores tasks in a CSV file to ensure data persistence between different program runs.

## Features

- Add tasks with details such as name, due date, description, and priority.
- Display a list of all tasks with their respective IDs.
- Edit existing tasks, modifying name, due date, description, and priority.
- Remove tasks based on their unique IDs.
- View task descriptions for more detailed information.

## Getting Started

### Prerequisites

- C++ compiler
- Standard Template Library (STL) support

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/task-manager-cpp.git
   cd task-manager-cpp
   
2. Compile the program:

    ```bash
    g++ -o task_manager main.cpp
   
3. Run the program:
    ```bash
    ./task_manager

## Usage
Upon running the program, you'll be presented with a command-line interface. Follow the prompts to execute commands. Common commands include:

- `add`: Add a new task.
- `display`: View all tasks.
- `edit`: Edit an existing task.
- `remove`: Remove a task.
- `showDescription`: View the description of a task.
- `clear`: Clear all tasks.
- Type `exit` to close the program.

## Contributing
Contributions are welcome! Please follow these guidelines:

Fork the repository.
Create a new branch for your feature or bug fix.
Commit your changes with descriptive messages.
Push your branch to your fork.
Create a pull request.