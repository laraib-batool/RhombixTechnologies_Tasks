#include "Task_1.h"

int main() {
    Task tasks[MAX_TASKS];
    int  count  = 0;
    int  nextId = 1;

    printHeader();
     
    while (true) {
        clearScreen();       
        printMenu();

        int choice = readInt("");

        switch (choice) {
            case 1:
                std::cout << '\n';
                printTasks(tasks, count);
                break;
            case 2:
                addTask(tasks, count, nextId);
                break;
            case 3:
                markDone(tasks, count);
                break;
            case 4:
                deleteTask(tasks, count);
                break;
            case 5:
                clearCompleted(tasks, count);
                break;
            case 0:
                std::cout << "\n  Goodbye! Stay productive.\n\n";
                return 0;
            default:
                std::cout << "  Invalid choice. Please try again.\n";
        }

        std::cout << "\n  Press ENTER to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}