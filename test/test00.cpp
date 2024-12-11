#include <TH2.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>
#include <TRandom.h>

void fit_gaussian_2D() {
    // 2次元ヒストグラムを作成
    TH2F *h2 = new TH2F("h2", "2D Histogram", 50, -5, 5, 50, -5, 5);

    // ランダムデータをヒストグラムにフィル
    for (int i = 0; i < 10000; i++) {
        h2->Fill(gRandom->Gaus(), gRandom->Gaus());
    }

    // 2次元ガウス関数を定義
    TF2 *gaus2D = new TF2("gaus2D", "[0]*exp(-0.5*((x-[1])/[2])^2)*exp(-0.5*((y-[3])/[4])^2)", -5, 5, -5, 5);
    gaus2D->SetParameters(1, 0, 1, 0, 1);

    // フィット
    h2->Fit("gaus2D");

    // フィット結果を取得
    double chi2 = gaus2D->GetChisquare();
    int ndf = gaus2D->GetNDF();
    double pvalue = gaus2D->GetProb();

    // フィットパラメータとその不確かさを表示
    for (int i = 0; i < 5; i++) {
        double par = gaus2D->GetParameter(i);
        double err = gaus2D->GetParError(i);
        double lower = par - 1.96 * err;
        double upper = par + 1.96 * err;
        std::cout << "Parameter " << i << ": " << par << " ± " << err 
                  << " (95% CI: [" << lower << ", " << upper << "])" << std::endl;
    }

    // カイ二乗値、自由度、p値を表示
    std::cout << "Chi2: " << chi2 << std::endl;
    std::cout << "NDF: " << ndf << std::endl;
    std::cout << "p-value: " << pvalue << std::endl;
    
    // キャンバスを作成して描画
    TCanvas *c1 = new TCanvas("c1", "2D Gaussian Fit Example", 800, 600);
    h2->Draw("COLZ");
    gaus2D->Draw("SAME");

    // 結果を保存
    c1->SaveAs("2D_gaussian_fit_example.png");
}
