#include <iostream>
#include <string>
#include <fstream>   // for file handling
using namespace std;

// Node structure for each task
struct Node {
    int id;             // Unique ID for the task
    string title;       // Task description
    bool done;          // Status of task
    Node* next;         // Pointer to next node

    Node(int _id, const string& _title) {
        id = _id;
        title = _title;
        done = false;   // new task is not completed
        next = nullptr;
    }
};

// Linked list class for To-Do list
class ToDoList {
private:
    Node* head;
    int nextId;          // to give unique ID to every task
    string filename;     // file where tasks are stored

    // Helper: clear the linked list (used in destructor and reload)
    void clearList() {
        Node* temp;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Save tasks to file
    void saveToFile() {
        ofstream out(filename);
        if (!out) {
            cout << "Error: Unable to open file for writing: " << filename << endl;
            return;
        }

        Node* temp = head;
        while (temp != nullptr) {
            // Format: id|done|title
            out << temp->id << '|'
                << (temp->done ? 1 : 0) << '|'
                << temp->title << '\n';
            temp = temp->next;
        }

        out.close();
        // cout << "Tasks saved to file.\n"; // optional debug
    }

    // Load tasks from file
    void loadFromFile() {
        ifstream in(filename);
        if (!in) {
            // File doesn't exist yet; that's fine, start empty
            return;
        }

        clearList(); // in case anything was there
        head = nullptr;
        nextId = 1;

        string line;
        int maxId = 0;

        while (getline(in, line)) {
            if (line.empty()) continue;

            // Parse format: id|done|title
            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);

            if (p1 == string::npos || p2 == string::npos) {
                continue; // bad line, skip
            }

            int id = stoi(line.substr(0, p1));
            int doneInt = stoi(line.substr(p1 + 1, p2 - p1 - 1));
            string title = line.substr(p2 + 1);

            Node* newNode = new Node(id, title);
            newNode->done = (doneInt != 0);

            // Insert at end
            if (head == nullptr) {
                head = newNode;
            } else {
                Node* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }

            if (id > maxId) maxId = id;
        }

        nextId = maxId + 1; // continue IDs from last used
        in.close();
    }

public:
    ToDoList(const string& file = "tasks.txt") {
        head = nullptr;
        nextId = 1;
        filename = file;
        loadFromFile(); // load existing tasks (if file exists)
    }

    // 1. Add new task at the end of the list
    void addTask(const string& title) {
        Node* newNode = new Node(nextId, title);
        nextId++;

        if (head == nullptr) { // empty list
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        cout << "Task added successfully with ID: " << newNode->id << endl;
        saveToFile(); // save after change
    }

    // 2. Mark a task as completed using task ID
    void markTaskDone(int id) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                if (temp->done) {
                    cout << "Task is already marked as completed.\n";
                } else {
                    temp->done = true;
                    cout << "Task ID " << id << " marked as completed.\n";
                    saveToFile(); // save after change
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Task with ID " << id << " not found.\n";
    }

    // 3. Delete a task using task ID
    void deleteTask(int id) {
        if (head == nullptr) {
            cout << "List is empty. No task to delete.\n";
            return;
        }

        // If first node is to be deleted
        if (head->id == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            cout << "Task ID " << id << " deleted successfully.\n";
            saveToFile(); // save after change
            return;
        }

        Node* prev = head;
        Node* curr = head->next;
        while (curr != nullptr) {
            if (curr->id == id) {
                prev->next = curr->next;
                delete curr;
                cout << "Task ID " << id << " deleted successfully.\n";
                saveToFile(); // save after change
                return;
            }
            prev = curr;
            curr = curr->next;
        }

        cout << "Task with ID " << id << " not found.\n";
    }

    // 4. Display all tasks
    void displayTasks() {
        if (head == nullptr) {
            cout << "No tasks in the list.\n";
            return;
        }

        cout << "\n------ TO-DO LIST ------\n";
        Node* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->id
                 << " | Task: " << temp->title
                 << " | Status: " << (temp->done ? "Completed" : "Pending")
                 << endl;
            temp = temp->next;
        }
        cout << "------------------------\n";
    }

    // Destructor to free memory and save tasks
    ~ToDoList() {
        saveToFile(); // final save before exit
        clearList();
    }
};

int main() {
    ToDoList todo("tasks.txt");   // file used for saving/loading
    int choice;

    do {
        cout << "\n===== TO-DO TASK MANAGER (Linked List) =====\n";
        cout << "1. Add new task\n";
        cout << "2. Mark task as completed\n";
        cout << "3. Delete a task\n";
        cout << "4. Display all tasks\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // clear newline from buffer

        if (choice == 1) {
            string title;
            cout << "Enter task description: ";
            getline(cin, title);
            todo.addTask(title);
        }
        else if (choice == 2) {
            int id;
            cout << "Enter task ID to mark as completed: ";
            cin >> id;
            todo.markTaskDone(id);
        }
        else if (choice == 3) {
            int id;
            cout << "Enter task ID to delete: ";
            cin >> id;
            todo.deleteTask(id);
        }
        else if (choice == 4) {
            todo.displayTasks();
        }
        else if (choice == 5) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
