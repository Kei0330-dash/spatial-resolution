#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "alias.hpp"
#include "MyClass.hpp"
#include "block.hpp"
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <algorithm>
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TF2.h>

// ========================================================================================================
// プロトタイプ宣言
// ========================================================================================================

/// @brief 深さ優先探索を実行してクラスターの塊を走査する。ただし、直接呼ばずint call_dfs()を経由して呼び出すこと。。
/// @param x ピクセルのx軸
/// @param y ピクセルのy軸
/// @param map 閾値が超えた情報を格納した2次元の配列
/// @return クラスターの塊としての情報を返す。
block dfs(int x, int y, THRESHOLD_MAP &map);

/// @brief 1次元のヒストグラムを作ります。
/// @param h1 動的に確保したROOTの1次元ヒストグラム
/// @param weight 2次元のADC値を格納した配列
void Fill_1Dhist(TH1D* &h1, ADC_DATA &weight);

/// @brief 2次元のヒストグラムを作ります。
/// @param h2 動的に確保したROOTの2次元ヒストグラム
/// @param weight 2次元のADC値を格納した配列
void Fill_2Dhist(TH2D* &h2, ADC_DATA &weight);

/// @brief 深さ優先探索をするためにmapを作成する関数。
/// @param weight ADC値
/// @param threshold 閾値
/// @param opt_sub オプションで、ADC値から閾値を引くかどうかを指定する。閾値より小さい場合は0にする。
/// @return 閾値が超えた情報を格納した2次元の配列を返す。
THRESHOLD_MAP create_map(ADC_DATA &weight, double threshold, bool opt_sub);

/// @brief 深さ優先探索を呼び出す関数。これを使うことでそのイベントのクラスターの数がわかる。
/// @param map 閾値を超えた後のmapであり、閾値が超えたところを'W',それ以外を'.'で表現する。
/// @param cluster クラスターの塊の配列で、dfsで走査された結果がこの中に格納される。
/// @param weight クラスターが単体だった場合その重みのデータは削除される。
/// @param opt_sub ペデスタル減算をするかどうかのフラグ。
/// @return そのイベントのクラスターの数を出力する。
int  call_dfs(THRESHOLD_MAP &map, CLUSTER_DATA &cluster, ADC_DATA &weight, const bool opt_sub);

/// @brief この関数を起動すると、クラスターを強調表示します。
/// @param weight 重みを基準にします。
/// @param box このclassに強調表示の情報がはいっています。
/// @param threshold 閾値です。
/// @param opt_sub 
void highlight(ADC_DATA &weight, TBox* &box, double threshold, bool opt_sub);

/// @brief 
/// @param weight 
/// @param opt_Red 
/// @param opt_sub 
/// @param opt_fit 
void AnalyzeAndVisualizeClusters(ADC_DATA weight, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false);

/// @brief root -l analysis.cppをターミナルでやって読み込んだ時、まずこの関数を呼び出します。
/// @param event_num eventのエントリー数を選ぶ
/// @param opt_Red クラスターを強調表示するかを選ぶ。
/// @param opt_sub ペデスタルを減算するかを選ぶ。
/// @param opt_fit フィッティングをするかを選ぶ。
/// @param opt_AutoCluster オートでクラスターがあるかどうかを判定します。他の引数に左右されません
/// @param path データのパスを入力します。デフォルトで選ばれている引数を変更して自分の環境に合わせたパスを入力してください。
void runMyClass(Int_t event_num, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false, bool opt_AutoCluster = false, TString path = "/home/otokun241/newRepository/data/SOFIST3_DATA_HV130_chip1_alpha_241009.root");

void closefile();

void run_100(int start, bool opt_Red = false, bool opt_sub = false);

// ========================================================================================================



class all_delete {
private:
    TH1D *h1 = nullptr;
    TH2D *h2 = nullptr;
    TBox *box = nullptr;
    TCanvas *c1 = nullptr;

public:
    inline void pointer_share(TH1D *s1 = nullptr, TH2D *s2 = nullptr, TBox *s3 = nullptr, TCanvas* s4 = nullptr);
    inline void pointer_delete();
};



// pointer_share で共有した場所を全て消去するメンバ関数。
// リニューアル後デストラクタとなる予定。
inline void all_delete::pointer_share(TH1D *s1, TH2D *s2, TBox *s3, TCanvas *s4) {
    h1 = s1;
    h2 = s2;
    box = s3;
    c1 = s4;
}

inline void all_delete::pointer_delete() {
    if (h1) delete h1;
    if (h2) delete h2;
    if (box) delete box;
    if (c1) delete c1;
    h1 = nullptr;
    h2 = nullptr;
    box = nullptr;
    c1 = nullptr;
}

inline void MyClass::Gaus2D_fitting(double x_center, double y_center, TH2D* h2) {
    TF2 *gaus2D = new TF2("gaus2D", "[0]*exp(-0.5*((x-[1])/[2])^2)*exp(-0.5*((y-[3])/[4])^2)");
    gaus2D->SetParameters(1, x_center, 1, y_center, 1);
    h2->Fit("gaus2D");
    double chi2 = gaus2D->GetChisquare();
    int ndf = gaus2D->GetNDF();
    double pvalue = gaus2D->GetProb();
    for (int i = 0; i < 5; i++) {
        double par = gaus2D->GetParameter(i);
        double err = gaus2D->GetParError(i);
        double lower = par - 1.96 * err;
        double upper = par + 1.96 * err;
        std::cout << "Parameter " << i << ": " << par << " ± " << err << " (95% CI: [" << lower << ", " << upper << "])" << std::endl;
    }
    std::cout << "Chi2: " << chi2 << std::endl;
    std::cout << "NDF: " << ndf << std::endl;
    std::cout << "p-value: " << pvalue << std::endl;
}

#endif
