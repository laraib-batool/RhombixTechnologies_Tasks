


#ifndef TASK_1_H
#define TASK_1_H

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

const int MAX_TASKS = 100;

struct Task {
    int         id;
    std::string title;
    bool        done;
};

void printLine(char ch = '-', int width = 50);
void printHeader();
void printMenu();
void printTasks(const Task tasks[], int count);

int         readInt(const std::string& prompt);
std::string readLine(const std::string& prompt);

void addTask       (Task tasks[], int& count, int& nextId);
void markDone      (Task tasks[], int  count);
void deleteTask    (Task tasks[], int& count);
void clearCompleted(Task tasks[], int& count);
void clearScreen   ();

#endif