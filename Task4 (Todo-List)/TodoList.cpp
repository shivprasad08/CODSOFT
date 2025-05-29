#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

// Class representing a single task
class Task {
private:
    string description;  // Task description
    bool completed;      // Status: true if task is completed

public:
    Task(const string& desc) : description(desc), completed(false) {}

    string getDescription() const { return description; }  // Returns task description
    bool isCompleted() const { return completed; }         // Returns completion status

    void markCompleted() { completed = true; }             // Marks the task as completed
};

// Class for input validation and user prompts
class InputManager {
public:
    // Prompt user for menu choice between 1 and 5
    int getMenuChoice() {
        int choice;
        while (true) {
            cout << "Enter your choice (1-5): ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 5) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a number between 1 and 5.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }

    // Prompt user for a valid task index
    int getTaskIndex(int maxSize) {
        int index;
        while (true) {
            cout << "Enter task number (1-" << maxSize << "): ";
            cin >> index;
            if (cin.fail() || index < 1 || index > maxSize) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a number between 1 and " << maxSize << ".\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return index - 1;  // Adjust for 0-based indexing
            }
        }
    }

    // Prompt user for a non-empty task description
    string getTaskDescription() {
        string desc;
        while (true) {
            cout << "Enter task description: ";
            getline(cin, desc);
            if (desc.empty()) {
                cout << "Task description cannot be empty. Try again!\n";
            } else {
                return desc;
            }
        }
    }
};

// Abstract base class for list managers
class BaseListManager {
protected:
    InputManager input;  // For managing user input
public:
    virtual ~BaseListManager() = default;
    virtual void run() = 0;  // Pure virtual function to be implemented by derived class
};

// Class that manages the to-do list
class ToDoList : public BaseListManager {
private:
    vector<Task> tasks;  // Vector to store all tasks

    // Display the list of tasks
    void viewTasks() const {  // Removed 'override' since it's not overriding a base class method
        if (tasks.empty()) {
            cout << "Your to-do list is empty. Add some tasks!\n";
            return;
        }
        cout << "\nYour To-Do List:\n";
        for (size_t i = 0; i < tasks.size(); i++) {
            cout << (i + 1) << ". " << tasks[i].getDescription()
                 << " [" << (tasks[i].isCompleted() ? "Completed" : "Pending") << "]\n";
        }
        cout << endl;
    }

    // Add a new task to the list
    void addTask() {
        string desc = input.getTaskDescription();  // Get task description
        tasks.push_back(Task(desc));               // Add to vector
        cout << "Task added successfully!\n";
    }

    // Mark a specific task as completed
    void markTaskCompleted() {
        if (tasks.empty()) {
            cout << "No tasks to mark. Add some tasks first!\n";
            return;
        }
        viewTasks();  // Show tasks
        int index = input.getTaskIndex(tasks.size());
        if (tasks[index].isCompleted()) {
            cout << "Task is already marked as completed.\n";
        } else {
            tasks[index].markCompleted();  // Mark selected task
            cout << "Task marked as completed!\n";
        }
    }

    // Remove a task from the list
    void removeTask() {
        if (tasks.empty()) {
            cout << "No tasks to remove. Add some tasks first!\n";
            return;
        }
        viewTasks();  // Show tasks
        int index = input.getTaskIndex(tasks.size());
        string desc = tasks[index].getDescription();  // Store description for confirmation
        tasks.erase(tasks.begin() + index);           // Remove task from vector
        cout << "Task \"" << desc << "\" removed successfully!\n";
    }

public:
    // Run the main loop of the to-do list manager
    void run() override {
        int choice;
        do {
            cout << "\nTo-Do List Manager\n";
            cout << "1. Add Task\n";
            cout << "2. View Tasks\n";
            cout << "3. Mark Task as Completed\n";
            cout << "4. Remove Task\n";
            cout << "5. Exit\n";
            choice = input.getMenuChoice();  // Get user's choice

            switch (choice) {
                case 1:
                    addTask();
                    break;
                case 2:
                    viewTasks();
                    break;
                case 3:
                    markTaskCompleted();
                    break;
                case 4:
                    removeTask();
                    break;
                case 5:
                    cout << "Goodbye! Stay organized!\n";
                    break;
            }
        } while (choice != 5);  // Exit loop on choice 5
    }
};

// Main function
int main() {
    cout << "Welcome to your To-Do List Manager!\n";
    ToDoList list;  // Create ToDoList object
    list.run();     // Start the application
    return 0;
}