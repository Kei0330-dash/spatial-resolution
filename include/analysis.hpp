#ifndef header_root
#define header_root

#include "MyClass.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TBox.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <TF2.h>

class block : public virtual MyClass {
public:
    bool flag = false;
    std::set<std::pair<int, int>> place;
    inline int Get_pixel_count();
    double get_xcenter() { return x_g; }
    double get_ycenter() { return y_g; }
    double get_ADCsum() { return ADCsum; }
    inline std::pair<double, double> center_of_gravity(std::vector<std::vector<UShort_t>> &weight);
    inline void Print_NormalDistribution(std::vector<std::vector<UShort_t>> &weight);

private:
    int i;
    double x_g, y_g, ADCsum;
};

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

// クラスターのピクセルの数を出力する。
inline int block::Get_pixel_count() {
    return place.size();
}

// 重心を計算して返り値として pair の first に x 軸の重心の値、second に y 軸の重心の値を返す。
inline std::pair<double, double> block::center_of_gravity(std::vector<std::vector<UShort_t>> &weight) {
    x_g = 0.0, y_g = 0.0, ADCsum = 0.0;
    std::pair<double, double> res;
    for (auto &a : place) {
        x_g += weight[a.first][a.second] * a.first;
        y_g += weight[a.first][a.second] * a.second;
        ADCsum += static_cast<double>(weight[a.first][a.second]);
    }
    std::cout << "ADC合計値:" << ADCsum << "\n";
    x_g = x_g / ADCsum;
    y_g = y_g / ADCsum;
    res.first = x_g;
    res.second = y_g;
    return res;
}

// 廃棄予定
inline void block::Print_NormalDistribution(std::vector<std::vector<UShort_t>> &weight) {
    TCanvas c2("c2", "1D hist", 600, 500);
    TH1D h1("h1", "1D Histogram;X;Entries", 100, 0, 1500);
    for (auto &a : place) {
        h1.Fill(weight[a.first][a.second]);
    }
    h1.Draw();
    c2.Update();
    c2.WaitPrimitive();
}

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

block dfs(int x, int y, std::vector<std::vector<char>> &map);

void create_1Dhist(TH1D* &h1, std::vector<std::vector<UShort_t>> &weight, UShort_t ADC[256][128]);

void create_map(std::vector<std::vector<char>> &map, std::vector<std::vector<UShort_t>> &weight, double threshold, bool opt_sub);

int call_dfs(std::vector<std::vector<char>> &map, std::vector<block> &cluster, std::vector<std::vector<UShort_t>> &weight, bool opt_sub);

void highlight(std::vector<std::vector<UShort_t>> &weight, TBox* &box, double threshold, bool opt_sub);

void runMyClass(Int_t event_num, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false);

void closefile();

void run_100(int start, bool opt_Red = false, bool opt_sub = false);
#endif
