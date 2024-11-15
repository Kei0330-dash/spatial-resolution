#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TF1.h>

void weightedProjection() {
    // ランダム生成の初期化
    TRandom3 rand(0);

    // 2次元ヒストグラムの作成
    int nBinsX = 40;
    int nBinsY = 40;
    double xMin = -4.0, xMax = 4.0;
    double yMin = -4.0, yMax = 4.0;
    TH2D *h2 = new TH2D("h2", "2D Histogram with Weights", nBinsX, xMin, xMax, nBinsY, yMin, yMax);

    // データの生成と重み付け
    int nEntries = 100;
    for (int i = 0; i < nEntries; ++i) {
        double x = rand.Gaus(0, 1);  // 平均0、標準偏差1のガウス分布
        double y = rand.Gaus(0, 1);  // 平均0、標準偏差1のガウス分布
        double weight = rand.Uniform(0.5, 1.5);  // 重みは0.5から1.5の一様分布

        h2->Fill(x, y, weight);
    }

    // 2次元ヒストグラムの描画
    TCanvas *c1 = new TCanvas("c1", "2D Histogram", 800, 600);
    h2->Draw("COLZ");
    c1->SaveAs("2D_Histogram_With_Weights.png");

    // X軸に沿ったプロジェクション
    TH1D *projX = h2->ProjectionX("projX", 1, -1, "e");

    // プロジェクションの標準誤差計算
    double stddev = projX->GetStdDev();
    double n = projX->GetEntries();
    double standardError = stddev / sqrt(n);

    std::cout << "Standard Error: " << standardError << std::endl;

    // 1次元ヒストグラムの描画
    TCanvas *c2 = new TCanvas("c2", "Projection on X-axis", 800, 600);
    projX->Draw();
    c2->SaveAs("ProjectionX_With_Weights.png");
}

int main() {
    weightedProjection();
    return 0;
}
