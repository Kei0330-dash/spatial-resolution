#include "../include/MyClass.hpp"
#include "../include/analysis.hpp"
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
#include <algorithm>

//2次元ヒストグラムの最小値から最大値
const Int_t x_min = 0, x_max = 128,
            y_min = 0, y_max = 128;
			
void MyClass::Find_AutoCluster(){
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
	nentries = fChain->GetEntries();
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
		threshold = h1->GetMean() + 3 * h1->GetStdDev(); 
		delete h1;
		h1 = nullptr;
		create_map(map, weight, threshold, true);

		int ans = call_dfs(map, cluster, weight, true);

		res[entry_num] = std::make_pair(entry_num, ans); 
		
	}
	TH1I *h1 = new TH1I("h1", "1D Histogram;X;Entries", 100, 0, 1000);
	// X軸とY軸のラベルから小数点を非表示に設定
	h1->GetXaxis()->SetNdivisions(50020);
	// h1->GetYaxis()->SetNdivisions(505);
	TCanvas *c1 = new TCanvas("c2", "1D Histogram", 600, 400);
	for(Long64_t i = 0; i < res.size(); i++){
		if(res[i].second >= 1){
			h1->Fill(res[i].first, res[i].second);
			std::cout << res[i].first << " " << res[i].second << std::endl;
		}
	}
	h1->Sumw2(0);
	h1->Draw("");
	c1->Update();
}