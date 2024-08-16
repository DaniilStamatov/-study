#pragma once

#include "people.h"

/*
    ����������� �� ������� ����������������.
    �������� ��������� �� ���������� ��������
    ���� ������� ���������������� �������� ��������� ���� ������������ ������, �����������
    ��������� �������� ��������� �� ��� ���, ���� ������� ������� ����������������
    �� ������ ������ ��� ����� ������������� ��������
*/
class SatisfactionSupervisor : public PersonObserver {
private:
    int min_satisfaction_;
    int max_satisfaction_;
public:
    // ����������� ��������� �������� ������� � �������� ������ ����������������
    SatisfactionSupervisor(int min_satisfaction, int max_satisfaction) : min_satisfaction_(min_satisfaction), max_satisfaction_(max_satisfaction) {
    }

    void OnSatisfactionChanged(Person& person, int old_value, int new_value) {
        if (new_value < old_value) {
            if (person.GetSatisfaction() < min_satisfaction_) {
                while (person.GetSatisfaction() < max_satisfaction_) {
                    person.Dance();
                }
            }
        }
    }

    // �������� ���������� ��������������
};