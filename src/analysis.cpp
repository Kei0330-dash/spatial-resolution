/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
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
const int x_min = 0, x_max = 128,
            y_min = 0, y_max = 128;
//
all_delete p;
TFile *file = nullptr;

block dfs(int x, int y, THRESHOLD_MAP &map, ADC_DATA &weight){
	std::stack<std::pair<int, int>> st;
	st.push({x, y});
	block cluster;
	cluster.insert_pixels(x, y, weight[x][y]);
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
						cluster.insert_pixels(nx, ny, weight[nx][ny]);
						st.push({nx, ny});
				}
			}
		}
	}
	return cluster;
}

void Fill_1Dhist(TH1D* &h1, ADC_DATA &weight){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h1->Fill(weight[i][j]);
		}
	}
}

void Fill_2Dhist(TH2D* &h2, ADC_DATA &weight){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h2->Fill(i, j, weight[i][j]);
		}
	}
}

ADC_DATA pedestal_subtract(ADC_DATA weight, double threshold, bool opt_sub){
	if(!opt_sub) return weight;
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if((int)threshold < weight[i][j]){
				weight[i][j] -= (int)threshold;
			}
			else{ 
				weight[i][j] = 0;
			}
		}
	}
}

THRESHOLD_MAP create_map(ADC_DATA &weight, double threshold, bool opt_sub){
	THRESHOLD_MAP map(x_max - x_min, std::vector<char>(y_max - y_min));
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if((int)threshold < weight[i][j]){
				map[i][j] = 'W';
				if(opt_sub) 
				weight[i][j] -= (int)threshold;
			}
			else{ 
				map[i][j] = '.';
				if(opt_sub) 
				weight[i][j] = 0;
			}
		}
	}
	return map;
}

int call_dfs(THRESHOLD_MAP &map, CLUSTER_DATA &cluster, ADC_DATA &weight, bool opt_sub, int event_num){
	int count = 0;
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			if(map[i][j] == 'W'){
				block tmp = dfs(i, j, map, weight);
				tmp.Set_eventnum(event_num);
				tmp.center_of_gravity(weight);
				if(tmp.Get_pixelsize() > 2) cluster.push_back(tmp);
				else{if(opt_sub)weight[i][j] = 0, count--;}
				count++;
			}
		}
	}
	return count;
}

void highlight(ADC_DATA &weight, TBox* &box, double threshold, bool opt_sub){
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
	// //使用する変数。
	// THRESHOLD_MAP map(x_max, std::vector<char>(y_max));
	// double threshold;
	// CLUSTER_DATA cluster;
	// Long64_t nentries;
	// ADC_DATA weight(128, std::vector<UShort_t>(128));
	// double totalWeight = 0.0;
	// UShort_t maxWeight = 0;
	// UShort_t minWeight = 9999;
	
	// //エントリー数を指定する
	// if (fChain == nullptr) return;
	// nentries = fChain->GetEntriesFast();
	// fChain->GetEntry(entry_num);

	// TH1D *h1 = new TH1D("h1", "1D Histogram;X;Entries", 100, 1000, 1500);
	// create_1Dhist(h1, weight, ADC);
	// TCanvas *c2 = new TCanvas("c2", "1D Histogram", 600, 400);
	// h1->Draw();
	// c2->Update();
	// //閾値の設定
	// threshold = h1->GetMean() + 3 * h1->GetStdDev(); 
	// std::cout << "閾値:" << threshold << std::endl;

	// create_map(map, weight, threshold, opt_sub);
	// std::cout << "クラスターカウント:" << call_dfs(map, cluster, weight, opt_sub) << std::endl;
	// if(!cluster.empty()){
	// 	for(int i = 0; i < cluster.size(); i++){
	// 		std::pair<double, double> ans = cluster[i].center_of_gravity(weight);
	// 		std::cout << "重心:" << ans.first << ", " << ans.second << std::endl;
	// 		}
	// }
	// for(int i = x_min; i < x_max; i++){
	// 	for(int j = y_min; j < y_max; j++){
	// 		if(weight[i][j]){
	// 			maxWeight = std::max(weight[i][j], maxWeight);
	// 			minWeight = std::min(weight[i][j], minWeight);
	// 		}
	// 	}
	// }
	// TH2D *h2 = new TH2D("h2", "2D Histogram;X;Y", (x_max - x_min) , x_min, x_max, (y_max - y_min), y_min, y_max);
	// h2->Sumw2();
	// if(!opt_fit){
	// 	for(int i = x_min; i < x_max; i++){
	// 		for(int j = y_min; j < y_max; j++){
	// 			if(weight[i][j]){
	// 				h2->Fill(i, j, weight[i][j]);
	// 			}
	// 		}
	// 	}
	// }
	// else{
	// 	std::cout << maxWeight << "," << minWeight << std::endl;
	// 	for(int i = x_min; i < x_max; i++){
	// 		for(int j = y_min; j < y_max; j++){
	// 			if(weight[i][j]){
	// 				double normalizedWeight = ((double)weight[i][j] - (double)minWeight) / ((double)maxWeight - (double)minWeight);
	// 				double sqrtWeight = std::sqrt(weight[i][j]);
	// 				h2->Fill(i, j, normalizedWeight);
	// 			}
	// 		}
	// 	}
	// }
	// TH1D *pj = (TH1D*)h2->ProjectionX();
	// // Z-Score Standarization	
	// // int nbins = pj->GetNbinsX();
	// // double mean = pj->GetMean();
	// // double stdDev = pj->GetStdDev();
	// // for (int i = 1; i <= nbins; ++i) {
	// // 	double binContent = pj->GetBinContent(i);
	// // 	double normalizedValue = (binContent - mean) / (stdDev);
	// // 	pj->SetBinContent(i, normalizedValue);
	// // }

	// if(opt_fit){
	// 	TCanvas *c1 = new TCanvas("c1", "2D Histogram", 650, 700);
	// 	c1->Divide(1,2);
	// 	//subplot1
	// 	c1->cd(1);
	// 	//h2->SetStats(0);
	// 	h2->Draw("COLZ");
	// 	TBox *box = nullptr;
	// 	if(opt_Red) highlight(weight, box, threshold, opt_sub);
	// 	p.pointer_share(h1, h2, box, c1);

	// 	//subplot2
	// 	c1->cd(2);
	// 	// ガウスフィッティング
	// 	TF1 *gaus = new TF1("gaus", "gaus", -5, 5);

	// 	double pvalue = gaus->GetProb();
	// 	std::cout << "p-value: " << pvalue << std::endl; 

	// 	pj->Draw("same");
	// 	pj->Fit("gaus");
	// 	gaus->Draw("same");
	// 	c1->Update();
	// }
	
	// else{
	// 	TCanvas *c1 = new TCanvas("c1", "2D Histogram", 650, 400);
	// 	//h2->SetStats(0);
	// 	h2->Draw("COLZ");
	// 	TBox *box = nullptr;
	// 	if(opt_Red) highlight(weight, box, threshold, opt_sub);
	// 	p.pointer_share(h1, h2, box, c1);
	// 	h2->Draw();
	// 	c1->Update();
	// }

}


void AnalyzeAndVisualizeClusters(ADC_DATA weight, bool opt_Red, bool opt_sub, bool opt_fit){
	//使用する変数。
	THRESHOLD_MAP map(x_max - x_min, std::vector<char>(y_max - y_min));
	double threshold;
	CLUSTER_DATA cluster;

	//1次元ヒストグラムの作成
	TH1D *h1 = new TH1D("h1", "ADC Distribution;ADC;Number of Entries", 100, 1000, 1500);
	Fill_1Dhist(h1, weight);
	TCanvas *hist1D = new TCanvas("hist1D", "1D Histogram", 600, 400);
	h1->Draw();
	hist1D->Update();

	//閾値の設定
	threshold = h1->GetMean() + 5 * h1->GetStdDev();
	std::cout << "Threshold: " << threshold << std::endl;

	//2次元マップの作成
	map = create_map(weight, threshold, opt_sub);
	std::cout << "Cluster count: " << call_dfs(map, cluster, weight, opt_sub, 0) << std::endl;//0は一時的な修正

	//クラスターがある場合、重心を求める
	if(!cluster.empty()){
		for(int i = 0; i < cluster.size(); i++){
			std::pair<double, double> ans;
			ans.first = cluster[i].Get_xcenter();
			ans.second = cluster[i].Get_ycenter();
			std::cout << "Center of gravity: " << ans.first << ", " << ans.second << std::endl;
		}
	}

	//2次元ヒストグラムの作成
	TH2D *h2 = new TH2D("h2", "Pixel Distribution;X;Y", (x_max - x_min), x_min, x_max, (y_max - y_min), y_min, y_max);
	h2->Sumw2();
	h2->SetStats(0);
	Fill_2Dhist(h2, weight);

	//閾値を超えた部分を強調表示
	TCanvas *hist2D = new TCanvas("hist2D", "2D Histogram", 600, 400);
	// hist2D->Divide(1, 2);
	// hist2D->cd(1);
	h2->Draw("COLZ");
	TBox *box = nullptr;
	if(opt_Red) highlight(weight, box, threshold, opt_sub);
	p.pointer_share(h1, h2, box, hist2D);

}

void Position_Resolution(CLUSTER_DATA data){
	int size = data.size();
	TH2D *pixels_pos = new TH2D("h2", "Position_Resolution", 30, 0, 30, 30, 0, 30);
	for(int i = 0; i < size; i++){
		std::pair<double, double> grav;
		grav.first = data[i].Get_xcenter();
		grav.second = data[i].Get_ycenter();
		//SOFIST3の30umの位置情報に変換
		std::pair<int, int> pos;
		pos.first = (grav.first + 0.5) * 30;
		pos.second = (grav.second + 0.5) * 30;
		//剰余計算をし、行と列のどこに格納されるか計算。
		pos.first = pos.first % 30; 
		pos.second = pos.second % 30;

		pixels_pos->Fill(pos.first, pos.second);
	}
	TCanvas *c1 = new TCanvas("hist2D", "Position_Resolution", 600, 800);
	c1->Divide(1,2);
	c1->cd(1);
	pixels_pos->Draw("COLZ");
	c1->cd(2);
	TH1D *projX = new TH1D("projx", "projectionX", 30, 0, 30);
	projX = pixels_pos->ProjectionX();
	projX->Sumw2(0);
	projX->Draw();
	c1->Update();
}


AnalyzeType runMyClass(Int_t event_num, bool opt_Red, bool opt_sub, bool opt_fit, bool opt_AutoCluster, TString path) {
	MyClass *myobj = nullptr;
	p.pointer_delete(); // 共有ポインタの解放
	if(!file){
		file = TFile::Open(path);
		if (!file || file->IsZombie()) {
			std::cerr << "Error opening file" << std::endl;
			return ANALYZE_ERROR;
		}
	}

	TTree *SOFIST_Data = (TTree*)file->Get("SOFIST_Data");
	myobj = new MyClass(SOFIST_Data);
	if(!opt_AutoCluster){
		ADC_DATA data = myobj->Get_ADC(event_num);
		// myobj->Loop(event_num, opt_Red, opt_sub, opt_fit);
		AnalyzeAndVisualizeClusters(data, opt_Red, opt_sub, opt_fit);
		delete myobj;
		return ANALYZE_ONE_EVENT;
	}
	else{
		myobj->Find_AutoCluster(opt_sub);
		delete myobj;
		return ANALYZE_ALL_CLUSTERS;
	}
//    myobj->Loop(event_num, opt_Red, opt_sub, opt_fit);
	// ADC_DATA data = myobj->Get_ADC(event_num);
	// AnalyzeAndVisualizeClusters(data, opt_Red, opt_sub, opt_fit);
}

void closefile(){
	if(file->IsOpen()){	
	file->Close();
	delete file;
	file = nullptr;
	}
}

void run_100(int start, bool opt_Red, bool opt_sub){
   for(int i = start; i < start + 100; i++){
      runMyClass(i, opt_Red, opt_sub, false);
   }
}


// フィット結果を取得
// double mean = gaus->GetParameter(1);
// double sigma = gaus->GetParameter(2);
// double chi2 = gaus->GetChisquare();
// int ndf = gaus->GetNDF();
// double pvalue = gaus->GetProb();

// ヒストグラムの色の範囲を固定
//h2->SetMinimum(1000); // 最小値を設定
//h2->SetMaximum(1500); // 最大値を設定