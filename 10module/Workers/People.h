#pragma once
#include <string>
#include <unordered_set>

enum class ProgrammingLanguage { CPP, JAVA, PYTHON, PHP };

enum class Gender { MALE, FEMALE };

class Person {
private:
    Gender gender_;
    std::string name_;
    int age_;

public:
    Person(const std::string& name, int age, Gender gender);
    const std::string& GetName() const;
    int GetAge() const;
    Gender GetGender() const;
};
// Программист. Знает несколько языков программирования
class Programmer : public Person {
private:
    std::unordered_set<ProgrammingLanguage> languages_;
public:
    Programmer(const std::string& name, int age, Gender gender);

    // Добавляет программисту знание языка программирования language
    // После вызова этого метода программист может программировать на этом языке
    // и на ранее добавленных
    void AddProgrammingLanguage(ProgrammingLanguage language);

    // Сообщает, может ли программист программировать на языке прогарммирования language
    // Изначально программист не владеет ни одним языком программирования
    bool CanProgram(ProgrammingLanguage language) const;
    
};

enum class WorkerSpeciality { BLACKSMITH, CARPENTER, WOOD_CHOPPER, ENGINEER, PLUMBER };

// Рабочий. Владеет несколькими специальностями
class Worker : public Person {
private:
    std::unordered_set<WorkerSpeciality> specialities_;
public:
    Worker(const std::string& name, int age, Gender gender);

    // Добавляет рабочему способность работать по специальности speciality
    void AddSpeciality(WorkerSpeciality speciality);

    // Сообщает, может ли рабочий работать по указанной специальности.
    // Сразу после своего создания рабочий не владеет никакими специальностями
    bool HasSpeciality(WorkerSpeciality speciality) const;
};