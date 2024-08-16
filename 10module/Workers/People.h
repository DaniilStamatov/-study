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
// �����������. ����� ��������� ������ ����������������
class Programmer : public Person {
private:
    std::unordered_set<ProgrammingLanguage> languages_;
public:
    Programmer(const std::string& name, int age, Gender gender);

    // ��������� ������������ ������ ����� ���������������� language
    // ����� ������ ����� ������ ����������� ����� ��������������� �� ���� �����
    // � �� ����� �����������
    void AddProgrammingLanguage(ProgrammingLanguage language);

    // ��������, ����� �� ����������� ��������������� �� ����� ���������������� language
    // ���������� ����������� �� ������� �� ����� ������ ����������������
    bool CanProgram(ProgrammingLanguage language) const;
    
};

enum class WorkerSpeciality { BLACKSMITH, CARPENTER, WOOD_CHOPPER, ENGINEER, PLUMBER };

// �������. ������� ����������� ���������������
class Worker : public Person {
private:
    std::unordered_set<WorkerSpeciality> specialities_;
public:
    Worker(const std::string& name, int age, Gender gender);

    // ��������� �������� ����������� �������� �� ������������� speciality
    void AddSpeciality(WorkerSpeciality speciality);

    // ��������, ����� �� ������� �������� �� ��������� �������������.
    // ����� ����� ������ �������� ������� �� ������� �������� ���������������
    bool HasSpeciality(WorkerSpeciality speciality) const;
};