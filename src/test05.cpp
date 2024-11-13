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
    TF1 *gaus = new TF1("gaus", "gaus", -5, 5);

    // フィット
    h2->Fit("gaus");

    // キャンバスを作成して描画
    TCanvas *c1 = new TCanvas("c1", "2D Gaussian Fit Example", 800, 1000);
    c1->Divide(1,2);
    c1->cd(1);
    h2->Draw("COLZ");
    c1->cd(2);
    TH1D *pj = (TH1D*)h2->ProjectionX("T");
    pj->Fit("gaus");
    pj->Draw();
    gaus->Draw("SAME");

    // 結果を保存
    c1->SaveAs("2D_gaussian_fit_example.png");
}
