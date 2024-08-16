#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <string>

using namespace std;


#include <cmath>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, int> ks = {
    {3, 2},
    {4, 2},
    {5, 2},
    {6, 3},
    {7, 3},
    {8, 3},
    {9, 3},
    {10, 4},
    {11, 4},
    {12, 4},
    {13, 4},
    {14, 4},
    {15, 5}
};

//int towerOfHanoiMoves(int n) {
//    switch (n) {
//    case 1: return 1;
//    case 2: return 3;
//    case 3: return 5;
//    case 4: return 9;
//    case 5: return 13;
//    case 6: return 17;
//    case 7: return 25;
//    case 8: return 33;
//    case 9: return 41;
//    case 10: return 49;
//    case 11: return 65;
//    case 12: return 81;
//    case 13: return 97;
//    case 14: return 113;
//    case 15: return 129;
//    case 16: return 161;
//    case 17: return 193;
//    case 18: return 225;
//    case 19: return 257;
//    case 20: return 289;
//    default:
//        std::cerr << "Error: n is out of range" << std::endl;
//        return -1;
//    }
//}



int towerOfHanoiMoves(int n) {
    int k = (std::sqrt(1 + 8 * n) - 1) / 2;

    return static_cast<int>((k + (n - (k * (k + 1)) / 2 )- 1) * std::pow(2, k) + 1);
    
    //if (ks.find(n) != ks.end()) {
    //    int k = ks[n];
    //    return static_cast<int>((k + 4 - 1) * std::pow(2, k) + 1);
    //}
    //else {
    //    // handle case where n is not in the map
    //    std::cerr << "Error: n is not in the map" << std::endl;
    //    return -1;
    //}
}

#include <iostream>



int main() {
    int n = 3;  // number of disks
    // number of columns
    int moves = towerOfHanoiMoves(n);
    std::cout << moves << std::endl;
    return 0;
}
class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    int GetDisksNum() const {
        return disks_.size();
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    void MoveDisks(int disks_num, Tower& destination, Tower& buffer) {
        if (disks_num > 0) {
            MoveDisks(disks_num - 1, buffer, destination);
            int top_disk = RemoveFromTop();
            destination.AddToTop(top_disk);
            buffer.MoveDisks(disks_num - 1, destination, *this);
        }
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        }
        else {
            disks_.push_back(disk);
            // допишите этот метод и используйте его в вашем решении
        }
    }

    int RemoveFromTop() {
        if (disks_.empty()) throw runtime_error("tower is empty");

        int top_disk = disks_.back();
        disks_.pop_back();
        return top_disk;
    }
    // вы можете дописывать необходимые для вашего решения методы

private:
    vector<int> disks_;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};


void SolveHanoi(vector<Tower>& towers) {
    int n = towers[0].GetDisksNum();
    if (n == 1) {
        int top_disk = towers[0].RemoveFromTop();
        towers[2].AddToTop(top_disk);
    }
    else {
        towers[0].MoveDisks(n - 1, towers[1], towers[2]);
        int top_disk = towers[0].RemoveFromTop();
        towers[2].AddToTop(top_disk);
        towers[1].MoveDisks(n - 1, towers[2], towers[0]);
    }
}

