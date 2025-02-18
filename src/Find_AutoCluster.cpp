#include "../include/alias.hpp"
#include "../include/block.hpp"
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
	THRESHOLD_MAP map(x_max, std::vector<char>(y_max));
	double threshold;
	CLUSTER_DATA cluster;
	Long64_t nentries;
	ADC_DATA weight(128, std::vector<int>(128));
	//クラスターがあるエントリーナンバーを全て返す
	nentries = Get_EntryMax();
	std::vector<std::pair<int,int>> res(nentries);

	for(Long64_t entry_num = 0; entry_num < nentries; entry_num++){
		weight = Get_ADC(entry_num);

		TH1D *hist = new TH1D("hist", "1D Histogram;X;Entries", 100, 700, 1800);
		Fill_1Dhist(hist, weight);
		//閾値の設定
		threshold = hist->GetMean() + 5 * hist->GetStdDev(); 
		delete hist;
		hist = nullptr;
		map = create_map(weight, threshold, opt_sub);

		int ans = call_dfs(map, cluster, weight, opt_sub, entry_num);

		res[entry_num] = std::make_pair(entry_num, ans); 
		
	}
	TH1I *h1 = new TH1I("h1", "Number of Clusters per Event;Number of Clusters;Number of Events", 10, 0, 10);
	TH1I *h2 = new TH1I("h2", "Cluster Size Distribution;Cluster Size;Number of Events", 100, 0, 100);
	TH1I *h3 = new TH1I("h3", "Distribution of Clusters in number of Events;Entry num;Number of Events", 100, 0, 1000);
	TH2D *h4 = new TH2D("h4", "The Event with the Largest Pixel Size;X;y", (x_max - x_min) , x_min, x_max, (y_max - y_min), y_min, y_max);

	// X軸とY軸のラベルから小数点を非表示に設定
	h1->GetXaxis()->SetNdivisions(50020);
	// h1->GetYaxis()->SetNdivisions(505);
	TCanvas *c1 = new TCanvas("c1", "Information about clusters", 1200, 800);
	c1->Divide(2,2);
	for(Long64_t i = 0; i < res.size(); i++){
		h1->Fill(res[i].second);
		if(res[i].second >= 1){
			h3->Fill(res[i].first, res[i].second);
		}
	}
	for(int i = 0; i < cluster.size(); i++){
		h2->Fill(cluster[i].Get_pixelsize());
	}
	sort(cluster.rbegin(), cluster.rend());
	Long64_t pixel_max = cluster[0].Get_eventnum();
	weight = Get_ADC(pixel_max);
	TH1D *hist = new TH1D("hist", "1D Histogram;X;Entries", 100, 700, 1800);
	Fill_1Dhist(hist, weight);
	//閾値の設定
	threshold = hist->GetMean() + 5 * hist->GetStdDev(); 
	delete hist;
	hist = nullptr;
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			if(weight[i][j]){
				h4->Fill(i, j, weight[i][j]);
			}
		}
	}
	h1->SetFillColor(kAzure-9);
	h2->SetFillColor(kGreen-9);
	h3->SetFillColor(kPink-9);
	
	TText *text3 = new TText(50, (h3->GetMaximum()), "Separated every 10 events");
	// pixel_maxの情報を追加
    TText *text4 = new TText(0, -15, Form("max_event:%lld", pixel_max));
	c1->cd(1); h1->Draw("");
	c1->cd(2); h2->Draw("");
	c1->cd(3); h3->Sumw2(0);  h3->Draw(""); text3->Draw();
	c1->cd(4); h4->Draw(""); text4->Draw();
	c1->Update();
	Position_Resolution(cluster);
}