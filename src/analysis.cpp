/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
#include "MyClass2.h"
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
//2次元ヒストグラムの最小値から最大値
const Int_t x_min = 0, x_max = 128,
            y_min = 0, y_max = 128;
class block : public virtual MyClass{
   public:
   bool flag = false;
   std::set<std::pair<int, int>> place;
   int Get_pixel_count();
   double get_xcenter(){
      return x_g / ADCsum;
   };
   double get_ycenter(){
      return y_g / ADCsum;
   };
   double get_ADCsum(){
      return ADCsum;
   };
   std::pair<double, double> center_of_gravity(UShort_t weight[128][128]);
   void Print_NormalDistribution(UShort_t weight[128][128]);
   private:
   int i;
   double x_g, y_g, ADCsum;
};
class all_delete{
   private:
      TH1D *h1 = nullptr;
      TH2D *h2 = nullptr;
      TBox *box = nullptr;
      TCanvas *c1 = nullptr;
   public:
      void pointer_share(TH1D *s1 = nullptr, TH2D *s2 = nullptr, TBox *s3 = nullptr, TCanvas* s4 = nullptr);
      void pointer_delete();
};
//クラスターのピクセルの数を出力する。
int block:: Get_pixel_count(){
   return place.size();
}
//重心を計算して返り値としてpairのfirstにx軸の重心の値、secondにy軸の重心の値を返す
std::pair<double, double> block::center_of_gravity(UShort_t weight[128][128]){ 
   x_g = 0.0, y_g = 0.0, ADCsum = 0.0;
   std::pair<double, double> res;
      for(auto &a : place){
         x_g += weight[a.first][a.second]* a.first;
         y_g += weight[a.first][a.second]* a.second;
         ADCsum += (double)weight[a.first][a.second];
      }
      std::cout << ADCsum << "\n";
      res.first = x_g / ADCsum;
      res.second = y_g / ADCsum;
      return res;
}
//
void block::Print_NormalDistribution(UShort_t weight[128][128]){
   TCanvas c2("c2", "1D hist", 600, 500);
   //TH1D* h1_X = h2->ProjectionX();
   //TH1D* h1_Y = h2->ProjectionY();
   //h1_X->Draw();
   TH1D h1("h1", "1D Histogram;X;Entries", 100, 0, 1500);
   for(auto &a : place){
      h1.Fill(weight[a.first][a.second]);
   }
   h1.Draw();
   c2.Update();
   c2.WaitPrimitive();
}

//
void all_delete :: pointer_share(TH1D *s1 = nullptr, TH2D *s2 = nullptr, TBox *s3 = nullptr, TCanvas* s4 = nullptr){
   h1 = s1;
   h2 = s2;
   box = s3;
   c1 = s4; 
}
//pointer_shareで共有した場所を全て消去するメンバ関数
void all_delete :: pointer_delete(){
   if(h1) delete(h1);
   if(h2) delete(h2);
   if(box) delete(box);
   if(c1) delete(c1);
   h1 = nullptr;
   h2 = nullptr;
   box = nullptr;
   c1 = nullptr;
}
all_delete p;

void MyClass::Gaus2D_fitting(double x_center, double y_center, TH2D* h2){
   // 2次元ガウス関数を定義（重心を中心に）
   TF2 *gaus2D = new TF2("gaus2D", "[0]*exp(-0.5*((x-[1])/[2])^2)*exp(-0.5*((y-[3])/[4])^2)", -5, 5, -5, 5);
   gaus2D->SetParameters(1, x_center, 1, y_center, 1);
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
      std::cout << "Parameter " << i << ": " << par << " ± " << err << " (95% CI: [" << lower << ", " << upper << "])" << std::endl;
      }
   // カイ二乗値、自由度、p値を表示
   std::cout << "Chi2: " << chi2 << std::endl;
   std::cout << "NDF: " << ndf << std::endl;
   std::cout << "p-value: " << pvalue << std::endl;
}
void MyClass::Loop(Int_t entry_num, bool opt_Red, bool opt_sub, bool fit_opt)
{
   
   std::vector<std::vector<char>> map(x_max, std::vector<char>(y_max));
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH1D *h1 = new TH1D("h1", "1D Histogram;X;Entries", 100, 1000, 1500);
   UShort_t weight[128][128];
      fChain->GetEntry(entry_num); 
      for(Long64_t i = x_min; i < x_max; i++){
         for(Long64_t j = y_min; j < y_max; j++){
            h1->Fill(ADC[i][j]);
            weight[i][j] = ADC[i][j];
         }
      }
      //閾値の設定
      double threshold = h1->GetMean() + 3 * h1->GetStdDev(); 
      std::cout << threshold << std::endl;

      for(Long64_t i = x_min; i < x_max; i++){
         for(Long64_t j = y_min; j < y_max; j++){
            if((u_short)threshold < weight[i][j]){
               map[i][j] = 'W';
               if(opt_sub) 
                  weight[i][j] -= (u_short)threshold;
            }
            else{ 
               map[i][j] = '.';
               if(opt_sub) 
               weight[i][j] = 0;
            }
         }
      }

      //深さ優先探索のラムダ式を作成
      auto dfs = [&](int x, int y) -> block {
         std::stack<std::pair<int, int>> st;
         st.push({x, y});
         block cluster;
         cluster.place.insert({x, y});
         while(!st.empty()){
            /*2回以上dfsを実行したら、cluster.flagをtrueにして最後の結果を表示しない*/
            //if(st.size() >= 2)cluster.flag = true;
            //cout << st.size() << endl;
            auto[cx, cy] = st.top();
            st.pop();
            map[cx][cy] = '.';
            //8方向に探索するための2重forループ(計算量は定数倍)
            for(int dx = -1; dx <= 1; dx++){
               for(int dy = -1; dy <= 1; dy++){
                  int nx = cx + dx, ny = cy + dy;
                  //2次元で作成したmapが範囲外でないときかつmapの値がWであればその座標でstackにpushする。
                  if(x_min <= nx && nx < x_max && y_min <= ny && ny < y_max && map[nx][ny] == 'W'){
                     cluster.flag = true;
                     cluster.place.insert({nx, ny});
                     st.push({nx, ny});
                  }
               }
            }
         }
         return cluster;
      };
      std::vector<block> cluster;
      int count = 0;
      for(int i = x_min; i < x_max; i++){
         for(int j = y_min; j < y_max; j++){
            if(map[i][j] == 'W'){
               block tmp = dfs(i, j);
               if(tmp.flag)cluster.push_back(tmp);
               else{if(opt_sub)weight[i][j] = 0, count--;}
               count++;
            }
         }
      }
      std::cout << count << std::endl;
      if(!cluster.empty()){
      for(int i = 0; i < cluster.size(); i++){
         std::pair<double, double> ans = cluster[i].center_of_gravity(ADC);
         std::cout << ans.first << ", " << ans.second << std::endl;
         }
      }
      //for(auto &cx : cluster[1].place)
      //std::cout << cx.first << " "<< cx.second << std::endl;
      //std::cout << cluster[0].px_count() << ", " << cluster[1].px_count() << std::endl;

   TH2D *h2 = new TH2D("h2", "2D Histogram;X;Y", 20, 50, 70, 128, y_min, y_max);
   if(!fit_opt){
      for(int i = x_min; i < x_max; i++){
         for(int j = y_min; j < y_max; j++){
            if(weight[i][j])
               h2->Fill(i, j, weight[i][j]);
         }
      }
   }
   else{
      for(int i = x_min; i < x_max; i++){
         for(int j = y_min; j < y_max; j++){
            if(weight[i][j])
               h2->Fill(i, j, weight[i][j]);
         }
      }
   }
   // ヒストグラムの色の範囲を固定
   //h2->SetMinimum(1000); // 最小値を設定
   //h2->SetMaximum(1500); // 最大値を設定
   TCanvas *c1 = new TCanvas("c1", "2D Histogram", 650, 700);
   c1->Divide(1,2);
   c1->cd(1);
   h2->SetStats(0);
   h2->Draw("COLZ");
   TBox *box = nullptr;
   if(opt_Red){
      for(Long64_t i = x_min; i < x_max; i++){
         for(Long64_t j = y_min; j < y_max; j++){
            if(0 < weight[i][j] && opt_sub ||(u_short)threshold < weight[i][j]){
               box = new TBox(i, j, i+1, j+1);
               box->SetLineColor(kRed);
               box->SetLineWidth(2);
               box->SetFillStyle(0); // 塗りつぶしなし
               box->Draw("same");
            }
         }
      }
   }
   if(!cluster.empty())
   if(fit_opt) Gaus2D_fitting(cluster[0].get_xcenter(),cluster[0].get_ycenter(), h2);

   p.pointer_share(h1, h2, box, c1);
   /*それぞれのメモリをnullptrに指す*/
   // h1 = nullptr;
   // h2 = nullptr;
   // box = nullptr;
   // c1 = nullptr;
   c1->cd(2);

   TH1D *pj = (TH1D*)h2->ProfileX("T",0,-1,"o");
   // ガウスフィッティング
   TF1 *gaus = new TF1("gaus", "gaus", -5, 5);
   pj->Fit("gaus");
   double pvalue = gaus->GetProb();
   std::cout << "p-value: " << pvalue << std::endl; 

   // フィット結果を取得
   // double mean = gaus->GetParameter(1);
   // double sigma = gaus->GetParameter(2);
   // double chi2 = gaus->GetChisquare();
   // int ndf = gaus->GetNDF();
   // double pvalue = gaus->GetProb();
   pj->Draw();
   gaus->Draw("SAME");
   //cluster[0].Print_NormalDistribution(weight);
   c1->Update();

}

TFile *file = nullptr;
/*Rootでこのコードを立ち上げたときはイベント数の引数を設定してこの関数を呼び出す。*/
void runMyClass(Int_t event_num, bool opt_Red = false, bool opt_sub = false, bool fit_opt = false) {
   MyClass *myobj = nullptr;
   p.pointer_delete(); // 共有ポインタの解放
   if(!file){
      file = TFile::Open("../DATA/SOFIST3_DATA_HV130_chip1_alpha_241009.root");
      if (!file || file->IsZombie()) {
         std::cerr << "Error opening file" << std::endl;
         return;
      }
   }
   TTree *SOFIST_Data = (TTree*)file->Get("SOFIST_Data");
   myobj = new MyClass(SOFIST_Data);
   myobj->Loop(event_num, opt_Red, opt_sub, fit_opt);
   delete myobj;
   //myobj = nullptr;
   //file->Close();
   //delete file;
   //file = nullptr;
}

void run_100(int start, bool opt_Red = false, bool opt_sub = false){
   for(int i = start; i < start + 100; i++){
      runMyClass(i, opt_Red, opt_sub,false);
   }
}

