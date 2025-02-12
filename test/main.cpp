#include <vector>
#include <iostream>
#include <TBox.h>

typedef std::vector<std::vector<int>> ADC_DATA;

struct Region {
    Long64_t x_min;
    Long64_t x_max;
    Long64_t y_min;
    Long64_t y_max;
};

void detectRegions(ADC_DATA &weight, double threshold, bool opt_sub, std::vector<Region> &regions) {
    Long64_t rows = weight.size();
    Long64_t cols = weight[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    for (Long64_t i = 0; i < rows; i++) {
        for (Long64_t j = 0; j < cols; j++) {
            if ((0 < weight[i][j] && opt_sub) || (static_cast<u_short>(threshold) < weight[i][j])) {
                if (!visited[i][j]) {
                    // 新しい領域を検出
                    Region region = {i, i, j, j};
                    std::vector<std::pair<Long64_t, Long64_t>> stack;
                    stack.push_back({i, j});

                    while (!stack.empty()) {
                        auto [x, y] = stack.back();
                        stack.pop_back();

                        if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y])
                            continue;

                        if ((0 < weight[x][y] && opt_sub) || (static_cast<u_short>(threshold) < weight[x][y])) {
                            visited[x][y] = true;
                            region.x_min = std::min(region.x_min, x);
                            region.x_max = std::max(region.x_max, x);
                            region.y_min = std::min(region.y_min, y);
                            region.y_max = std::max(region.y_max, y);

                            // 隣接するピクセルをスタックに追加
                            stack.push_back({x + 1, y});
                            stack.push_back({x - 1, y});
                            stack.push_back({x, y + 1});
                            stack.push_back({x, y - 1});
                        }
                    }
                    regions.push_back(region);
                }
            }
        }
    }
}

void highlight(ADC_DATA &weight, TBox* &box, double threshold, bool opt_sub) {
    std::vector<Region> regions;
    detectRegions(weight, threshold, opt_sub, regions);

    for (const auto& region : regions) {
        box = new TBox(region.x_min, region.y_min, region.x_max + 1, region.y_max + 1);
        box->SetLineColor(kRed);
        box->SetLineWidth(2);
        box->SetFillStyle(0); // 塗りつぶしなし
        box->Draw("same");
    }
}

int main() {
    ADC_DATA data = {
        {5, 5, 5, 5, 5},
        {5, 15, 15, 15, 5},
        {5, 15, 20, 15, 5},
        {5, 15, 15, 15, 5},
        {5, 5, 5, 5, 5}
    };
    TBox *box = nullptr;
    highlight(data, box, 10, true);

    return 0;
}
