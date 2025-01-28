#include "../include/MyClass.hpp"
#include "../include/analysis.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TText.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <TF2.h>
#include <algorithm>

//2次元ヒストグラムの最小値から最大値
const Int_t x_min = 0, x_max = 128,
            y_min = 0, y_max = 128;
			
void MyClass::Find_AutoCluster(bool opt_sub){
	//使用する変数。
	std::vector<std::vector<char>> map(x_max, std::vector<char>(y_max));
	double threshold;
	std::vector<block> cluster;
	Long64_t nentries;
	std::vector<std::vector<UShort_t>> weight(128, std::vector<UShort_t>(128));
	//クラスターがあるエントリーナンバーを全て返す
	std::vector<std::pair<int,int>> res(1000);
	// エントリー数の数だけ、走査
	if (fChain == nullptr) return ;
	nentries = fChain->GetEntriesFast();
	for(Long64_t entry_num = 0; entry_num < 1000; entry_num++){
		//ientryに、TChainの中でjentryを含むtreeの番号をいれる
		Long64_t ientry = LoadTree(entry_num);
		//もしもjentryを含むtreeが存在しなければbreak
		if (ientry < 0) break;
		//エントリー数を指定する
		if (fChain == nullptr) return;

		fChain->GetEntry(entry_num);

		TH1D *h1 = new TH1D("h1", "1D Histogram;X;Entries", 100, 700, 1800);
		create_1Dhist(h1, weight, ADC);
		//閾値の設定
		threshold = h1->GetMean() + 4 * h1->GetStdDev(); 
		delete h1;
		h1 = nullptr;
		create_map(map, weight, threshold, opt_sub);

		int ans = call_dfs(map, cluster, weight, opt_sub);

		res[entry_num] = std::make_pair(entry_num, ans); 
		
	}
	TH1I *h1 = new TH1I("h1", "Number of Clusters per Event;Number of Clusters;Number of Events", 10, 0, 10);
	TH1I *h2 = new TH1I("h2", "Cluster Size Distribution;Cluster Size;Number of Events", 100, 0, 100);
	TH1I *h3 = new TH1I("h3", "Distribution of Clusters in number of Events;Entry num;Number of Events", 100, 0, 1000);
	TH2D *h4 = new TH2D("h4", "The Event with the Largest Pixel Size;X;y", (x_max - x_min) , x_min, x_max, (y_max - y_min), y_min, y_max);

	// X軸とY軸のラベルから小数点を非表示に設定
	h1->GetXaxis()->SetNdivisions(50020);
	// h1->GetYaxis()->SetNdivisions(505);
	TCanvas *c1 = new TCanvas("c2", "1D Histogram", 1200, 800);
	c1->Divide(2,2);
	for(Long64_t i = 0; i < res.size(); i++){
		h1->Fill(res[i].second);
		if(res[i].second >= 1){
			h3->Fill(res[i].first, res[i].second);
		}
	}
	for(int i = 0; i < cluster.size(); i++){
		h2->Fill(cluster[i].Get_pixel_count());
	}
	sort(cluster.rbegin(), cluster.rend());
	Long64_t pixel_max = cluster[0].Get_pixel_count();
	LoadTree(pixel_max);
	fChain->GetEntry(pixel_max);
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			if(weight[i][j]){
				h4->Fill(i, j, weight[i][j]);
			}
		}
	}
	h1->SetFillColor(kBlue);
	h2->SetFillColor(kBlue);
	h3->SetFillColor(kBlue);
	// pixel_maxの情報を追加
    TText *text = new TText(0, -15, Form("max_event:%lld", pixel_max));
	c1->cd(1); h1->Draw("");
	c1->cd(2); h2->Draw("");
	c1->cd(3); h3->Sumw2(0); h3->Draw("");
	c1->cd(4); h4->Draw(""); text->Draw();
	c1->Update();
}