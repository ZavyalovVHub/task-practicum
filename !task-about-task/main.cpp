#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return persons_tasks_.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++persons_tasks_[person][TaskStatus::NEW];
    }

    void TEST_AddTask(const string& person, const map<TaskStatus, int>& test_dict)
    {
        persons_tasks_[person] = test_dict;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        TasksInfo updated_tasks;
        TasksInfo no_updated_tasks;

        // если такого человека нет в списке
        if (persons_tasks_.count(person) == 0) {
            return tuple(updated_tasks, no_updated_tasks);
        }

        TasksInfo temp_dict = persons_tasks_.at(person);

        for (const auto& [current_status, count] : persons_tasks_.at(person)) {
            if (current_status == TaskStatus::DONE) {
                continue;
            }
            TaskStatus next_status = static_cast<TaskStatus>(static_cast<int>(current_status) +1);
            if (count <= task_count) {
                temp_dict[current_status] -= count;
                temp_dict[next_status] += count;
                if (count != 0) updated_tasks[next_status] = count;
                task_count -= count;
            } else {
                temp_dict[current_status] -= task_count;
                temp_dict[next_status] += task_count;
                no_updated_tasks[current_status] = count - task_count;
                if (task_count != 0) updated_tasks[next_status] = task_count;
                task_count = 0;
            }
        }

        //обновляем основной словарь по результатам
        for (const auto& [current_status, count] : temp_dict) {

            persons_tasks_.at(person)[current_status] = count;
        }
        return tuple(updated_tasks, no_updated_tasks);
    }


private:
    map<string, TasksInfo> persons_tasks_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;

    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Ivan's tasks "s << "-" << 2 << ":"s << endl;
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    cout << "Ivan's tasks "s << "-" << 2 << ":"s << endl;
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    cout << "Ivan's tasks "s << "+" << 3 << ":"s << endl;
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Ivan's tasks "s << "-" << 2 << ":"s << endl;
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    cout << "Ivan's tasks "s << "-" << 2 << ":"s << endl;
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));


//    cout << "Ivan's tasks "s << "-" << 6 << ":"s << endl;
//    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 6);
//    cout << "Updated Ivan's tasks: "s;
//    PrintTasksInfo(updated_tasks);
//    cout << "Untouched Ivan's tasks: "s;
//    PrintTasksInfo(untouched_tasks);
//    cout << "Ivan's tasks: "s;
//    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
}


