#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>


enum class TaskStatus {
	NEW,
	IN_PROGRESS,
	TESTING,
	DONE
};

static TaskStatus Next(TaskStatus status) {
	return static_cast<TaskStatus>(static_cast<int>(status) + 1);
}

using TasksInfo = std::map<TaskStatus, int>;

static void RemoveZeros(TasksInfo& tasks_info) {
	// Соберём те статусы, которые нужно убрать из словаря
	std::vector<TaskStatus> statuses_to_remove;
	for (const std::pair<const TaskStatus, int>& task_item : tasks_info) {
		if (task_item.second == 0) {
			statuses_to_remove.push_back(task_item.first);
		}
	}
	for (const TaskStatus status : statuses_to_remove) {
		tasks_info.erase(status);
	}
}

class TeamTasks {
private:
	std::map<std::string, TasksInfo> personTasks;

public:
	TasksInfo GetPersonTaskInfo(const std::string& name) const {
		return personTasks.at(name);
	}

	void AddNewTask(const std::string& name) {
		++personTasks[name][TaskStatus::NEW];
	}
	

	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& name, int task_count) {
		TasksInfo tasks = personTasks[name];
		if (tasks.empty()) return std::tuple<TasksInfo, TasksInfo>(TasksInfo(), TasksInfo());
		TasksInfo updatedTasks;
		TasksInfo untouchedTasks;

		for (auto status = TaskStatus::NEW; status < TaskStatus::DONE; status = Next(status)) {
			updatedTasks[Next(status)] = std::min(task_count, tasks[status]);
			task_count -= updatedTasks[Next(status)];
		}

		for (auto status = TaskStatus::NEW; status < TaskStatus::DONE; status = Next(status)) {
			untouchedTasks[status] = tasks[status] - updatedTasks[Next(status)];
			tasks[status] += updatedTasks[status] - updatedTasks[Next(status)];
		}

		tasks[TaskStatus::DONE] += updatedTasks[TaskStatus::DONE];

		RemoveZeros(updatedTasks);
		RemoveZeros(untouchedTasks);
		RemoveZeros(personTasks.at(name));

		return { updatedTasks, untouchedTasks };
	}
};

void PrintTasksInfo(TasksInfo tasks_info) {
	std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
		", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
		", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}
int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	std::cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTaskInfo("Ilia"));
	std::cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTaskInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
}