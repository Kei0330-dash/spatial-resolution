#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// グリッドサイズ、目印の個数、シミュレーション回数
const int n = 5; // グリッドの行数
const int m = 5; // グリッドの列数
const int k = 5; // 目印の個数
const int simulations = 1000000; // シミュレーション回数

bool isValidPlacement(const std::vector<std::vector<int>>& grid, int x, int y) {
    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == 1) {
            return false;
        }
    }
    return true;
}

int main() {
    std::srand(std::time(0));
    int successfulPlacements = 0;

    for (int sim = 0; sim < simulations; ++sim) {
        std::vector<std::vector<int>> grid(n, std::vector<int>(m, 0));
        bool valid = true;

        for (int i = 0; i < k; ++i) {
            int attempts = 0;
            bool placed = false;

            while (attempts < 1000) {
                int x = std::rand() % n;
                int y = std::rand() % m;

                if (grid[x][y] == 0 && isValidPlacement(grid, x, y)) {
                    grid[x][y] = 1;
                    placed = true;
                    break;
                }
                ++attempts;
            }

            if (!placed) {
                valid = false;
                break;
            }
        }

        if (valid) {
            ++successfulPlacements;
        }
    }

    double probability = static_cast<double>(successfulPlacements) / simulations;
    std::cout << "目印が1つも隣接しない確率: " << probability * 100 << "%" << std::endl;

    return 0;
}
