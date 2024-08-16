#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <iomanip>
using namespace std;

enum class Status { ACTUAL, EXPIRED, DELETED };
void SwapMatrix(std::vector<std::vector<int>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        int temp = matrix[i][i];
        matrix[i][i] = matrix[n - i - 1][i];
        matrix[n - i - 1][i] = temp;
    }
}

void WalkAsHorse(std::vector<std::vector<char>>& matrix, int x, int y) {
    matrix[y][x] = 'N';

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((std::abs(i - y) == 2 && std::abs(j - x) == 1) ||
                (std::abs(i - y) == 1 && std::abs(j - x) == 2)) {
                matrix[i][j] = '*';
            }
        }
    }
}

bool CheckMagicSquare(std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    bool isMagic = true;
    std::vector<int> count(n * n + 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int element = matrix[i][j];
            if (element < 1 || element > n * n) {
                isMagic = false;
                break;
            }
            count[element]++;
            if (count[element] > 1) {
                isMagic = false;
                break;
            }

        }
        if (!isMagic) {
            break;
        }
    }
    if (!isMagic) return false;
    int sum = n * (n * n + 1) / 2;

    int rowSum = 0;
    int columnSum = 0;
    int mainDiagSum = 0;
    int subDiagSum = 0;
    for (int i = 0; i < n; i++) {
        int rowSum = 0;
        int colSum = 0;
        for (int j = 0; j < n; j++) {
            rowSum += matrix[i][j];
            colSum += matrix[j][i];
        }
        if (rowSum != sum || colSum != sum) {
            std::cout << "Not a magic square." << std::endl;
            return false;
        }
    }

    int diagSum1 = 0;
    int diagSum2 = 0;
    for (int i = 0; i < n; i++) {
        diagSum1 += matrix[i][i];
        diagSum2 += matrix[i][n - i - 1];
    }
    if (diagSum1 != sum || diagSum2 != sum) {
        std::cout << "Not a magic square." << std::endl;
        return false;
    }
    std::cout << "Is a magic square." << std::endl;

    return true;
}

void fillMatrix(std::vector<std::vector<int>>& matrix, int n, int m) 
{
    int num = 1;
    int top = 0, bottom = n - 1, left = 0, right = m - 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = num;
            num++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = num;
            num++;
        }
        right--;

        for (int i = right; i >= left; i--) {
            matrix[bottom][i] = num++;
        }
        bottom--;

        for (int i = bottom; i >= top; i--) {
            matrix[i][left] = num++;
        }

        left++;
    }
}


struct AnimalObservation {
    string name;
    int days_ago;
    int health_level;
    // в заголовке метода теперь указан полный тип
    auto MakeKey() const {
        // создаём и сразу возвращаем объект:
                // явно указывать его тип не нужно, достаточно указать аргументы конструктора
                // в фигурных скобках
        return std::tie(days_ago, health_level, name);
    }
};
struct Document {
    int id;
    Status status;
    double relevance;
    int rating;

    auto MakeKey() const {
        
        int negRating = -rating;
        double negRelevance = -relevance;
        return std::tie(status, negRating, negRelevance);
    }
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return lhs.MakeKey() < rhs.MakeKey();
           
        });
}

void printBoard(const std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    /*vector<Document> documents = {
        {100, Status::ACTUAL, 0.5, 4}, {101, Status::EXPIRED, 0.5, 4},
        {102, Status::ACTUAL, 1.2, 4}, {103, Status::DELETED, 1.2, 4},
        {104, Status::ACTUAL, 0.3, 5},
    };
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' ' << static_cast<int>(document.status) << ' ' << document.relevance
            << ' ' << document.rating << endl;
    }*/

    int n = 4;
    int m = 4;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));

    fillMatrix(matrix, n, m);

    // Print matrix
    for (const auto& row : matrix) {
        for (int num : row) {
            std::cout << std::setw(5) << num << " ";
        }
        std::cout << std::endl;
    }

    SwapMatrix(matrix, 4);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto& row : matrix) {
        for (int num : row) {
            std::cout << std::setw(5) << num << " ";
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<char>> board(8, std::vector<char>(8, '.'));

    WalkAsHorse(board, 3, 2);

    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::vector<std::vector<int>> matrix1 = {
        {7, 12, 1, 14},
        {2, 13, 8, 11},
        {16, 3, 10, 5},
        {9, 6, 15, 4}
    };
    std::cout << CheckMagicSquare(matrix1) << " ";

    return 0;
}