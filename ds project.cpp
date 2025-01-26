#include <iostream>
#include <string>
#include <limits> // Required for std::numeric_limits
#include <ctime>

using namespace std;

class Task
{
public:
    string name;
    string description;
    string duedate;
    bool completed;
    int priority;
    Task *next;
    Task *prev;

    Task(string name, string description, string duedate, int priority)
        : name(name), description(description), duedate(duedate), priority(priority), completed(false), next(nullptr), prev(nullptr) {}
};

class StackNode
{
public:
    string Operation_Type;
    Task *task;
    Task taskBackup;
    StackNode *next;

    StackNode(string t, Task *taskPtr, Task backup)
        : Operation_Type(t), task(taskPtr), taskBackup(backup), next(nullptr) {}
};

class Stack
{
public:
    StackNode *top;
    Stack() : top(nullptr) {}

    void push(string Operation_Type, Task *taskPtr, Task backup)
    {
        StackNode *newNode = new StackNode(Operation_Type, taskPtr, backup);
        newNode->next = top;
        top = newNode;
    }

    StackNode *pop()
    {
        if (top == nullptr)
        {
            return nullptr;
        }
        StackNode *nodeToPop = top;
        top = top->next;
        return nodeToPop;
    }

    bool isEmpty()
    {
        if (top == nullptr)
        {
            return true;
        }
        return false;
    }
};

class queuetask
{
public:
    int priority;
    queuetask *next;
    queuetask *front = NULL;
    queuetask *rear = NULL;
    void enqueue(int priority)
    {
        queuetask *new_task = new queuetask;
        new_task->priority = priority;
        new_task->next = NULL;

        if (front == NULL || new_task->priority < front->priority)
        {
            new_task->next = front;
            front = new_task;
            if (rear == NULL)
            {
                rear = new_task;
            }
        }
        else
        {
            queuetask *current = front;
            while (current->next != NULL && current->next->priority <= new_task->priority)
            {
                current = current->next;
            }
            new_task->next = current->next;
            current->next = new_task;

            if (current->next == NULL)
            {
                rear = new_task;
            }
        }
    }
};

class ToDoTask
{
public:
    Task *head;
    Task *tail;

    Stack undoStack, redoStack;

    ToDoTask() : head(nullptr), tail(nullptr) {}

    void addTask(string name, string description, string duedate, int priority)
    {
        Task *newTask = new Task(name, description, duedate, priority);
        if (head == nullptr)
        {
            head = newTask;
            tail = newTask;
        }
        else
        {
            tail->next = newTask;
            newTask->prev = tail;
            tail = newTask;
        }
        undoStack.push("add", newTask, *newTask);
        redoStack = Stack(); // Clear redo stack
        cout << "Task added successfully!" << endl;
    }

    bool isFound(string name)
    {
        Task *current = head;
        while (current != nullptr)
        {
            if (current->name == name)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void editTask(string name)
    {
        Task *current = head;
        while (current != nullptr)
        {
            if (current->name == name)
            {
                cout << "Editing task: " << name << endl;

                cout << "Enter new description: ";
                getline(cin, current->description);

                cout << "Enter new due date: ";
                getline(cin, current->duedate);

                cout << "Task updated successfully!" << endl;
                return;
            }
            current = current->next;
        }
        cout << "Task \"" << name << "\" not found!" << endl;
    }

    void markTaskCompleted(string name)
    {
        Task *current = head;

        while (current != nullptr && current->name != name)
        {
            current = current->next;
        }
        if (current != nullptr)
        {
            current->completed = true;
            cout << "Task '" << current->name << "' marked as completed." << endl;
        }
        else
        {
            cout << "Task '" << name << "' not found." << endl;
        }
    }

    void removeTask(string name)
    {

        Task *current = head;

        while (current != nullptr && current->name != name)
        {
            current = current->next;
        }
        if (current == nullptr)
        {
            cout << "Task '" << name << "' not found." << endl;
            return;
        }

        if (current == head)
        {

            head = current->next;
            if (head != nullptr)
            {
                head->prev = nullptr;
            }
        }
        else if (current == tail)
        {
            tail = current->prev;
            if (tail != nullptr)
            {
                tail->next = nullptr;
            }
        }
        else
        {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        undoStack.push("remove", current, *current);
        delete current;
        redoStack = Stack(); // Clear redo stack
        cout << "Task '" << name << "' removed successfully." << endl;
    }

    void clearTasks()
    {
        while (head != nullptr)
        {
            Task *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        cout << "All tasks cleared successfully." << endl;
    }

    void displayScore()
    {
        int completedCount = 0;
        int uncompletedCount = 0;
        Task *current = head;

        while (current != nullptr)
        {
            if (current->completed)
            {
                completedCount++;
            }
            else
            {
                uncompletedCount++;
            }

            current = current->next;
        }
        cout << "\n----- Task Score -----" << endl<<endl;
        cout << "Completed tasks:" << completedCount << endl;
        cout << "Uncompleted tasks:" << uncompletedCount << endl<<endl;
        cout << "---------------------" << endl;
    }

    const string currentDateTime()
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

        return buf;
    }

    void displayTasks()
    {
        Task *current = head;
        if (current == nullptr)
        {
            cout << "No tasks found." << endl;
            return;
        }

        cout << "Tasks:" << endl;
        string today = currentDateTime();
        while (current != nullptr)
        {
            cout << "- " << current->name
                 << " (description: " << current->description << ")"
                 << " (Due: " << current->duedate << ")"
                 << " (" << (current->completed ? "completed" : "not completed") << ")"
                 << " (Priority: " << (current->priority == 1 ? "1" : (current->priority == 2 ? "2" : "3")) << ") " << endl;

            if (current->duedate < today)
            {
                cout << "-This task is overdue!";
            }
            else if (current->duedate == today)
            {
                cout << "-Due Today !";
            }
            cout << endl;
            current = current->next;
        }
    }

    void count()
    {
        int counter = 0;
        Task *temp = head;

        while (temp != nullptr)
        {
            counter++;
            temp = temp->next;
        }
        cout << "Number of tasks is: " << counter << endl;
    }

    void undo()
    {
        if (undoStack.isEmpty())
        {
            cout << "Nothing to undo!" << endl;
            return;
        }
        StackNode *action = undoStack.pop();
        if (action->Operation_Type == "add")
        {
            removeTask(action->task->name);
        }
        else if (action->Operation_Type == "remove")
        {
            addTask(action->taskBackup.name, action->taskBackup.description, action->taskBackup.duedate, action->taskBackup.priority);
        }
        redoStack.push(action->Operation_Type, action->task, action->taskBackup);
        delete action;
        cout << "Undo completed." << endl;
    }

    void redo()
    {
        if (redoStack.isEmpty())
        {
            cout << "Nothing to redo!" << endl;
            return;
        }
        StackNode *action = redoStack.pop();
        if (action->Operation_Type == "add")
        {
            addTask(action->taskBackup.name, action->taskBackup.description, action->taskBackup.duedate, action->taskBackup.priority);
        }
        else if (action->Operation_Type == "remove")
        {
            removeTask(action->taskBackup.name);
        }
        else
        {
            undoStack.push(action->Operation_Type, action->task, action->taskBackup);
            delete action;
            cout << "Redo completed." << endl;
        }
    }
};

int main()
{

    ToDoTask toDoList;
    queuetask queue;
    int choice;
    string name, description, duedate;
    int priority;

    cout << " \n------- Welcome to your Task Manager -------" << endl
         << endl
         << endl;

    do
    {
        cout << "1. Add a Task" << endl;
        cout << "2. Edit a Task" << endl;
        cout << "3. Mark a Task as completed" << endl;
        cout << "4. Remove a Task" << endl;
        cout << "5. Display All Tasks" << endl;
        cout << "6. Clear All Tasks" << endl;
        cout << "7. Number of tasks" << endl;
        cout << "8. Do undo" << endl;
        cout << "9. Do redo" << endl;
        cout << "10. Show Task Score" << endl;
        cout << "11. Manage Tasks by priority" << endl;
        cout << "12. Exit" << endl
             << endl;
        cout << "Enter your choice: " << endl;
        cin >> choice;

        // Clear the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {

        case 1:
            cout << "Enter task name: " << endl;
            getline(cin, name);

            cout << "Enter task description: " << endl;
            getline(cin, description);

            cout << "Enter task due date: " << endl;
            getline(cin, duedate);

            toDoList.addTask(name, description, duedate, priority);
            break;

        case 2:
            cout << "Enter task name to edit: " << endl;
            getline(cin, name);
            toDoList.editTask(name);
            break;

        case 3:
            cout << "Enter task name to mark as completed: " << endl;
            getline(cin, name);
            toDoList.markTaskCompleted(name);
            break;

        case 4:
            if (toDoList.head == nullptr)
            {
                cout << "No tasks to remove" << endl;
            }
            else
            {
                cout << "Enter task name to remove: " << endl;
                getline(cin, name);
                toDoList.removeTask(name);
            }
            break;
        case 5:
            toDoList.displayTasks();
            break;

        case 6:
            toDoList.clearTasks();
            break;

        case 7:
            toDoList.count();
            break;

        case 8:
            toDoList.undo();
            break;

        case 9:
            toDoList.redo();
            break;

        case 10:
            toDoList.displayScore();
            break;

        case 11:
            if (toDoList.head == nullptr)
            {
                cout << "No tasks to manage by priority!" << endl;
                break;
            }
            else
            {
                cout << "Enter the name of the task to manage by priority :" << endl;
                getline(cin, name);
            }

            if (!toDoList.isFound(name))
            {
                cout << "Task " << name << " not found in the list " << endl;
            }
            else
            {
                cout << "Enter priority of the task (1-3, 1 is the highest priority):" << endl;
                cin >> priority;
            }

            if (priority < 1 || priority > 3)
            {
                cout << "Invalid priority! Must be between 1 and 3." << endl;
                break;
            }
            else
            {
                Task *current = toDoList.head;
                while (current != nullptr)
                {
                    if (current->name == name)
                    {
                        current->priority = priority;
                        queue.enqueue(priority);
                        cout << "Task " << name << " updated with priority" << endl;
                        break;
                    }

                    current = current->next;
                }
            }
            break;

        case 12:
            cout << "Exiting the program..." << endl;

            break;

        default:
            cout << "Invalid choice! Please try again." << endl;
        }

        cout << " \n---------------------------------------------------" << endl
             << endl
             << endl;
    } while (choice != 12);
    return 0;
}
