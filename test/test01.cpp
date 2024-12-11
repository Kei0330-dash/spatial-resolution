// ROOTのヘッダーファイルをインクルード
#include <TGraphQQ.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TRandom.h>

void example() {
    // データセットを作成
    const int n = 1000;
    double x[n], y[n];
    
    TRandom rand;
    for (int i = 0; i < n; ++i) {
        x[i] = rand.Gaus(0, 1);  // 標準正規分布のデータ
        y[i] = rand.Gaus(1, 2);  // 平均1、標準偏差2の正規分布のデータ
    }
    
    // QQプロットを生成
    TGraphQQ *qq = new TGraphQQ(n, x, n, y);
    qq->SetTitle("QQ Plot;Theoretical Quantiles;Sample Quantiles");

    // キャンバスに描画
    TCanvas *c1 = new TCanvas("c1", "QQ Plot", 800, 600);
    qq->Draw("AL");

    // キャンバスを保存
    c1->SaveAs("qq_plot.png");
}
