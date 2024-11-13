/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
#include "MyClass2.h"
#include "header.h"
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
//
all_delete p;
//深さ優先探索の関数を作成
block dfs(int x, int y, std::vector<std::vector<char>> &map){
   std::stack<std::pair<int, int>> st;
   st.push({x, y});
   block cluster;
   cluster.place.insert({x, y});
   while(!st.empty()){
      /*2回以上dfsを実行したら、cluster.flagをtrueにして最後の結果を表示しない*/
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
}

void create_map(std::vector<std::vector<char>> &map, UShort_t weight[128][128], double threshold, bool opt_sub){
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
}

int call_dfs(std::vector<std::vector<char>> &map, std::vector<block> &cluster, UShort_t weight[128][128], bool opt_sub){
   int count = 0;
   for(int i = x_min; i < x_max; i++){
      for(int j = y_min; j < y_max; j++){
         if(map[i][j] == 'W'){
            block tmp = dfs(i, j, map);
            if(tmp.flag)cluster.push_back(tmp);
            else{if(opt_sub)weight[i][j] = 0, count--;}
            count++;
         }
      }
   }
   return count;
}

void highlight(UShort_t weight[128][128], TBox* &box, double threshold, bool opt_sub){
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

void MyClass::Loop(Int_t entry_num, bool opt_Red, bool opt_sub, bool opt_fit){
   //使用する変数。
   std::vector<std::vector<char>> map(x_max, std::vector<char>(y_max));
   double threshold;
   std::vector<block> cluster;
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
      threshold = h1->GetMean() + 3 * h1->GetStdDev(); 
      std::cout << "閾値:" << threshold << std::endl;

      create_map(map, weight, threshold, opt_sub);
      std::cout << "クラスターカウント:" << call_dfs(map, cluster, weight, opt_sub) << std::endl;
      if(!cluster.empty()){
      for(int i = 0; i < cluster.size(); i++){
         std::pair<double, double> ans = cluster[i].center_of_gravity(ADC);
         std::cout << "重心:" << ans.first << ", " << ans.second << std::endl;
         }
      }

   TH2D *h2 = new TH2D("h2", "2D Histogram;X;Y", 128, x_min, x_max, 128, y_min, y_max);
   if(!opt_fit){
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
   if(opt_Red) highlight(weight, box, threshold, opt_sub);
   if(!cluster.empty()) if(opt_fit) Gaus2D_fitting(cluster[0].get_xcenter(),cluster[0].get_ycenter(), h2);

   p.pointer_share(h1, h2, box, c1);
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
   c1->Update();
}

TFile *file = nullptr;
/*Rootでこのコードを立ち上げたときはイベント数の引数を設定してこの関数を呼び出す。
第一引数はeventのどこを参照するか選ぶ。第二引数は、クラスターの強調表示をするかを選ぶ。第三引数はペデスタルの減算するかを選ぶ。
第四引数はフィッテイングするかを選ぶ。*/
void runMyClass(Int_t event_num, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false) {
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
   myobj->Loop(event_num, opt_Red, opt_sub, opt_fit);
   delete myobj;
}

void closefile(){
   file->Close();
   delete file;
   file = nullptr;
}

void run_100(int start, bool opt_Red = false, bool opt_sub = false){
   for(int i = start; i < start + 100; i++){
      runMyClass(i, opt_Red, opt_sub, false);
   }
}

