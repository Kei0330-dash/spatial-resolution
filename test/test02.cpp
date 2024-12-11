#include <TGraphQQ.h>
#include <TCanvas.h>
#include <TRandom.h>

void example() {
    const int n = 1000;
    double x[n], y[n];
    
    TRandom rand;
    for (int i = 0; i < n; ++i) {
        x[i] = rand.Gaus(0, 1);  // 標準正規分布のデータ
        y[i] = rand.Gaus(1, 2);  // 平均1、標準偏差2の正規分布のデータ
    }
    
    // xデータのQQプロットを生成
    TGraphQQ *qq_x = new TGraphQQ(n, x, n, x);
    qq_x->SetTitle("QQ Plot for X;Theoretical Quantiles;Sample Quantiles");
    // yデータのQQプロットを生成
    TGraphQQ *qq_y = new TGraphQQ(n, y, n, y);
    qq_y->SetTitle("QQ Plot for Y;Theoretical Quantiles;Sample Quantiles");

    // キャンバスに描画
    TCanvas *c1 = new TCanvas("c1", "QQ Plots", 1200, 600);
    c1->Divide(2, 1);
    
    c1->cd(1);
    qq_x->Draw("AL");

    c1->cd(2);
    qq_y->Draw("AL");

    // キャンバスを保存
    c1->SaveAs("qq_plots.png");
}
