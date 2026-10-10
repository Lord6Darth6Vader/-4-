#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

// Структуры данных
struct Student {
    string fio;           // ФИО
    string group;         // название группы
    vector<int> grades;   // оценки (целые числа)
};

struct Group {
    string name;
};

const int MIN_GRADE = 2;
const int MAX_GRADE = 5;

// Читает строку целиком с пробелами, поэтому подходит для ФИО.
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {
        cout << "\nВвод завершен.\n"; // конец ввода (ctrl+d) выходим из программы.
        exit(0);
    }
    return s;
}

// Читает непустую строку
string readNonEmpty(const string& prompt)
{
    while(true) {
        string s = readLine(prompt);
        if (!s.empty()) return s;
        cout << "Строка не может быть пустой.\n";
    }
}

// Читает целое число в диапазоне [lo, hi]. Защищена от ввода букв.
int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {
            // stoi бросает исключение, если в строке нет числа
        }
        cout << "Введите целое число от " << lo << " до " << hi << ".\n";
    }
}

// Поиск и подсчёты

int findGroup(const vector<Group>& groups, const string& name) {
    for (size_t i = 0; i < groups.size(); i++)
        if (groups[i].name == name) return (int)i;
    return -1;
}

int countStudentsInGroup(const vector<Student>& students, const string& group) {
    int cnt = 0;
    for (const Student& s : students)
        if (s.group == group) cnt++;
    return cnt;
}

// Средний балл по списку оценок (0, если оценок нет).
double average(const vector<int>& grades) {
    if (grades.empty()) return 0.0;
    int sum = 0;
    for (int g : grades) sum += g;
    return (double)sum / grades.size();
}

// Вывод и выбор из списков

void printGroups(const vector<Group>& groups, const vector<Student>& students) {
    for (size_t i = 0; i < groups.size(); i++) {
        cout << "  " << i + 1 << ". " << groups[i].name
             << " (студентов: " << countStudentsInGroup(students, groups[i].name) << ")\n";
    }
}

void printStudents(const vector<Student>& students) {
    for (size_t i = 0; i < students.size(); i++) {
        const Student& s = students[i];
        cout << "  " << i + 1 << ". " << s.fio << " | " << s.group << " | оценки: ";
        if (s.grades.empty()) cout << "нет";
        for (int g : s.grades) cout << g << ' ';
        cout << '\n';
    }
}

// Возвращает индекс выбранной группы или -1, если выбор отменён / групп нет.
int chooseGroup(const vector<Group>& groups, const vector<Student>& students) {
    if (groups.empty()) {
        cout << "Групп пока нет.\n";
        return -1;
    }
    cout << "Группы:\n";
    printGroups(groups, students);
    int n = readInt("Номер группы (0 - отмена): ", 0, (int)groups.size());
    return n - 1;  // 0 превращается в -1
}

int chooseStudent(const vector<Student>& students) {
    if (students.empty()) {
        cout << "Студентов пока нет.\n";
        return -1;
    }
    cout << "Студенты:\n";
    printStudents(students);
    int n = readInt("Номер студента (0 - отмена): ", 0, (int)students.size());
    return n - 1;
}

// Операции со студентами

void addStudent(vector<Student>& students, const vector<Group>& groups) {
    int g = chooseGroup(groups, students);
    if (g < 0) return;
    Student s;
    s.fio = readNonEmpty("ФИО: ");
    s.group = groups[g].name;
    students.push_back(s);
    cout << "Студент добавлен.\n";
}

void editStudent(vector<Student>& students, const vector<Group>& groups) {
    int i = chooseStudent(students);
    if (i < 0) return;

    string fio = readLine("Новое ФИО (Enter - оставить \"" + students[i].fio + "\"): ");
    if (!fio.empty()) students[i].fio = fio;

    cout << "Сменить группу? Текущая: " << students[i].group << "\n";
    cout << "Группы:\n";
    printGroups(groups, students);
    int g = readInt("Номер новой группы (0 - не менять): ", 0, (int)groups.size());
    if (g > 0) students[i].group = groups[g - 1].name;

    cout << "Данные студента обновлены.\n";
}

void deleteStudent(vector<Student>& students) {
    int i = chooseStudent(students);
    if (i < 0) return;
    string ans = readLine("Удалить \"" + students[i].fio + "\"? (y/n): ");
    if (ans == "y" || ans == "Y") {
        students.erase(students.begin() + i);
        cout << "Студент удалён.\n";
    } else {
        cout << "Отменено.\n";
    }
}

// Операции с группами

void addGroup(vector<Group>& groups) {
    string name = readNonEmpty("Название группы: ");
    if (findGroup(groups, name) != -1) {
        cout << "Такая группа уже есть.\n";
        return;
    }
    groups.push_back({name});
    cout << "Группа добавлена.\n";
}

void editGroup(vector<Group>& groups, vector<Student>& students) {
    int g = chooseGroup(groups, students);
    if (g < 0) return;

    string newName = readNonEmpty("Новое название: ");
    if (findGroup(groups, newName) != -1) {
        cout << "Группа с таким названием уже существует.\n";
        return;
    }
    string oldName = groups[g].name;
    groups[g].name = newName;
    // Студенты хранят название группы, поэтому обновляем и их.
    for (Student& s : students)
        if (s.group == oldName) s.group = newName;
    cout << "Группа переименована.\n";
}

void deleteGroup(vector<Group>& groups, const vector<Student>& students) {
    int g = chooseGroup(groups, students);
    if (g < 0) return;

    int cnt = countStudentsInGroup(students, groups[g].name);
    if (cnt > 0) {
        cout << "Нельзя удалить: в группе " << groups[g].name
             << " есть студенты (" << cnt << ").\n";
        return;
    }
    groups.erase(groups.begin() + g);
    cout << "Группа удалена.\n";
}

// Оценки и статистика

void addGrade(vector<Student>& students) {
    int i = chooseStudent(students);
    if (i < 0) return;
    int grade = readInt("Оценка (" + to_string(MIN_GRADE) + "-" + to_string(MAX_GRADE) + "): ",
                        MIN_GRADE, MAX_GRADE);
    students[i].grades.push_back(grade);
    cout << "Оценка добавлена.\n";
}

void showStudentAverage(const vector<Student>& students) {
    int i = chooseStudent(students);
    if (i < 0) return;
    if (students[i].grades.empty()) {
        cout << "У студента нет оценок.\n";
        return;
    }
    cout << fixed << setprecision(2);
    cout << "Средний балл студента " << students[i].fio << ": "
         << average(students[i].grades) << '\n';
}

// Средний балл группы считаем по всем оценкам всех её студентов.
void showGroupAverage(const vector<Group>& groups, const vector<Student>& students) {
    int g = chooseGroup(groups, students);
    if (g < 0) return;

    vector<int> all;
    for (const Student& s : students)
        if (s.group == groups[g].name)
            all.insert(all.end(), s.grades.begin(), s.grades.end());

    if (all.empty()) {
        cout << "В группе нет оценок.\n";
        return;
    }
    cout << fixed << setprecision(2);
    cout << "Средний балл группы " << groups[g].name << ": " << average(all) << '\n';
}

// Категория студента определяется его худшей оценкой:
// 5 - отличник; 4 - хорошист; 3 - троечник; 2 - двоещник.
void showGroupStats(const vector<Group>& groups, const vector<Student>& students) {
    int g = chooseGroup(groups, students);
    if (g < 0) return;

    int excellent = 0, good = 0, satisf = 0, failing = 0, noGrades = 0, total = 0;

    for (const Student& s : students) {
        if (s.group != groups[g].name) continue;
        total++;
        if (s.grades.empty()) { noGrades++; continue; }

        int minGrade = MAX_GRADE;
        for (int x : s.grades)
            if (x < minGrade) minGrade = x;

        if (minGrade == 5) excellent++;
        else if (minGrade == 4) good++;
        else if (minGrade == 3) satisf++;
        else failing++;
    }

    cout << "Статистика группы " << groups[g].name << " (студентов: " << total << "):\n";
    cout << "  отличников:  " << excellent << '\n';
    cout << "  хорошистов:  " << good << '\n';
    cout << "  троечников:  " << satisf << '\n';
    cout << "  с двойками:  " << failing << '\n';
    cout << "  без оценок:  " << noGrades << '\n';
}

// Меню

void studentsMenu(vector<Student>& students, vector<Group>& groups) {
    while (true) {
        cout << "\n--- Студенты ---\n"
             << "1. Добавить\n2. Изменить\n3. Удалить\n4. Показать всех\n0. Назад\n";
        int c = readInt("Выбор: ", 0, 4);
        if (c == 0) return;
        if (c == 1) addStudent(students, groups);
        if (c == 2) editStudent(students, groups);
        if (c == 3) deleteStudent(students);
        if (c == 4) {
            if (students.empty()) cout << "Студентов нет.\n";
            else printStudents(students);
        }
    }
}

void groupsMenu(vector<Group>& groups, vector<Student>& students) {
    while (true) {
        cout << "\n--- Группы ---\n"
             << "1. Добавить\n2. Изменить\n3. Удалить\n4. Показать все\n0. Назад\n";
        int c = readInt("Выбор: ", 0, 4);
        if (c == 0) return;
        if (c == 1) addGroup(groups);
        if (c == 2) editGroup(groups, students);
        if (c == 3) deleteGroup(groups, students);
        if (c == 4) {
            if (groups.empty()) cout << "Групп нет.\n";
            else printGroups(groups, students);
        }
    }
}

int main() {
    // Начальные данные
    vector<Group> groups = {{"БД-1-25"}, {"ПО-1-25"}};
    vector<Student> students = {
        {"Иванов Иван Иванович",     "БД-1-25", {5, 5, 5, 5}},
        {"Петров Пётр Петрович",     "БД-1-25", {5, 4, 5}},
        {"Комлев Кирилл Антонович",  "БД-1-25", {4, 4, 5, 4}},
        {"Кулич Никита",  "БД-1-25", {3, 4, 3}},
        {"Владимир Владимирович Даванков",  "П-1-25", {5, 5, 4}},
        {"Филиппов Ярослав Сергеевич", "П-1-25", {2, 3, 3}},
        {"Боков Михаил Сергеевич",  "П-1-25", {}},
    };

    while (true) {
        cout << "\n===== Учебная группа =====\n"
             << "1. Студенты\n"
             << "2. Группы\n"
             << "3. Добавить оценку студенту\n"
             << "4. Средний балл студента\n"
             << "5. Средний балл группы\n"
             << "6. Статистика группы\n"
             << "0. Выход\n";
        int c = readInt("Выбор: ", 0, 6);
        if (c == 0) break;
        if (c == 1) studentsMenu(students, groups);
        if (c == 2) groupsMenu(groups, students);
        if (c == 3) addGrade(students);
        if (c == 4) showStudentAverage(students);
        if (c == 5) showGroupAverage(groups, students);
        if (c == 6) showGroupStats(groups, students);
    }
    cout << "До свидания!\n";
    return 0;
}
