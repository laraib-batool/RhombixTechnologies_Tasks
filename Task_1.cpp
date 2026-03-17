#include "Task_1.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(char ch, int width) {
    std::cout << std::string(width, ch) << '\n';
}

void printHeader() {
    printLine('=');
    std::cout << std::setw(35) << "TO-DO LIST\n";
    printLine('=');
}

void printMenu() {
    std::cout << "\n  [1] View tasks\n"
              << "  [2] Add task\n"
              << "  [3] Mark task as completed\n"
              << "  [4] Delete task\n"
              << "  [5] Clear all completed tasks\n"
              << "  [0] Exit\n\n";
    printLine();
    std::cout << "  Choice: ";
}

void printTasks(const Task tasks[], int count) {
    printLine();
    if (count == 0) {
        std::cout << "  (no tasks yet)\n";
        printLine();
        return;
    }
    std::cout << std::left
              << "  " << std::setw(5) << "ID"
              << std::setw(6)         << "Done"
              << "Title\n";
    printLine();
    for (int i = 0; i < count; i++) {
        std::string status = tasks[i].done ? "[✔]" : "[ ]";
        std::cout << "  " << std::setw(5) << tasks[i].id
                  << std::setw(6)          << status
                  << tasks[i].title << '\n';
    }
    printLine();
}

int readInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Please enter a valid non-negative number.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    size_t start = s.find_first_not_of(" \t");
    size_t end   = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

void addTask(Task tasks[], int& count, int& nextId) {
    if (count >= MAX_TASKS) {
        std::cout << "  Task limit reached. Cannot add more.\n";
        return;
    }
    std::string title = readLine("\n  Task title: ");
    if (title == "") {
        std::cout << "  Title cannot be empty. Task not added.\n";
        return;
    }
    tasks[count].id    = nextId++;
    tasks[count].title = title;
    tasks[count].done  = false;
    count++;
    std::cout << "  Task added!\n";
}

void markDone(Task tasks[], int count) {
    if (count == 0) { std::cout << "  No tasks to mark.\n"; return; }
    printTasks(tasks, count);
    int id = readInt("  Enter task ID to mark as completed: ");
    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            if (tasks[i].done)
                std::cout << "  Task is already completed.\n";
            else {
                tasks[i].done = true;
                std::cout << "  Task marked as completed!\n";
            }
            return;
        }
    }
    std::cout << "  Task ID not found.\n";
}

void deleteTask(Task tasks[], int& count) {
    if (count == 0) { std::cout << "  No tasks to delete.\n"; return; }
    printTasks(tasks, count);
    int id = readInt("  Enter task ID to delete: ");
    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < count - 1; j++)
                tasks[j] = tasks[j + 1];
            count--;
            std::cout << "  Task deleted.\n";
            return;
        }
    }
    std::cout << "  Task ID not found.\n";
}

void clearCompleted(Task tasks[], int& count) {
    int removed = 0;
    int i = 0;
    while (i < count) {
        if (tasks[i].done) {
            for (int j = i; j < count - 1; j++)
                tasks[j] = tasks[j + 1];
            count--;
            removed++;
        } else {
            i++;
        }
    }
    std::cout << "  Removed " << removed << " completed task(s).\n";
}