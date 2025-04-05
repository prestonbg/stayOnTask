#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include "json.hpp"
using json = nlohmann::json;

struct Task {
    std::string title;
    std::string project;
    std::vector<std::string> tags;
    std::string deadline;

    std::string toCSV() const {
        std::ostringstream ss;
        ss << "\"" << title << "\",\"" << project << "\",\"";
        for (size_t i = 0; i < tags.size(); ++i) {
            ss << tags[i];
            if (i != tags.size() - 1) ss << ";";
        }
        ss << "\",\"" << deadline << "\"";
        return ss.str();
    }

    json toJSON() const {
        return json{ {"title", title}, {"project", project}, {"tags", tags}, {"deadline", deadline} };
    }
};

std::vector<Task> tasks;

void addTask() {
    Task t;
    std::cout << "Task Title: ";
    std::getline(std::cin, t.title);

    std::cout << "Project: ";
    std::getline(std::cin, t.project);

    std::string tagInput;
    std::cout << "Tags (separated by commas): ";
    std::getline(std::cin, tagInput);
    std::istringstream ss(tagInput);
    std::string tag;
    while (std::getline(ss, tag, ',')) {
        t.tags.push_back(tag);
    }

    std::cout << "Deadline (YYYY-MM-DD): ";
    std::getline(std::cin, t.deadline);

    tasks.push_back(t);
    std::cout << "Task added!\n";
}

void showTasks(const std::vector<Task>& list) {
    if (list.empty()) {
        std::cout << "There are no tasks.\n";
        return;
    }

    for (size_t i = 0; i < list.size(); ++i) {
        const auto& t = list[i];
        std::cout << i + 1 << ". " << t.title << " | Project: " << t.project << " | Deadline: " << t.deadline << " | Tags: ";
        for (const auto& tag : t.tags) {
            std::cout << tag << " ";
        }
        std::cout << "\n";
    }
}

void filterTasks() {
    std::string keyword;
    std::cout << "Enter a keyword (project / tag / date): ";
    std::getline(std::cin, keyword);

    std::vector<Task> result;
    for (const auto& t : tasks) {
        if (t.project == keyword || t.deadline == keyword ||
            std::any_of(t.tags.begin(), t.tags.end(), [&](const std::string& tag) { return tag == keyword; })) {
            result.push_back(t);
        }
    }

    showTasks(result);
}

void exportCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Title,Project,Tags,Deadline\n";
    for (const auto& t : tasks) {
        file << t.toCSV() << "\n";
    }
    file.close();
    std::cout << "Export in " << filename << "\n";
}

void exportJSON(const std::string& filename) {
    json j;
    for (const auto& t : tasks) {
        j.push_back(t.toJSON());
    }

    std::ofstream file(filename);
    file << std::setw(4) << j << std::endl;
    file.close();
    std::cout << "Export in " << filename << "\n";
}

void menu() {
    std::string choice;
    while (true) {
        std::cout << "\n1. Add a task\n2. Show all tasks{n3. Filter\n4. Export JSON\n5. Export CSV\n6. Output\nSelect:";
        std::getline(std::cin, choice);

        if (choice == "1") addTask();
        else if (choice == "2") showTasks(tasks);
        else if (choice == "3") filterTasks();
        else if (choice == "4") exportJSON("tasks.json");
        else if (choice == "5") exportCSV("tasks.csv");
        else if (choice == "6") break;
        else std::cout << "Wrong choice\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Simple task manager\n";
    menu();
    return 0;
}
