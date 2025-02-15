/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
#include "../include/MyClass.hpp"
#include "../include/analysis_class.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TBox.h>
#include <TLine.h>
#include <TText.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <algorithm>

block analysis::dfs(int x, int y, THRESHOLD_MAP map){
	std::stack<std::pair<int, int>> st;
	st.push({x, y});
	block cluster;
	cluster.insert_pixels(x, y, weight[x][y]);
	while(!st.empty()){
		auto[cx, cy] = st.top();
		st.pop();
		map[cx][cy] = '.';
		//8方向に探索するための2重forループ(計算量は定数倍)
		for(int dx = -1; dx <= 1; dx++){
			for(int dy = -1; dy <= 1; dy++){
				int nx = cx + dx, ny = cy + dy;
				//2次元で作成したmapが範囲外でないときかつmapの値がWであればその座標でstackにpushする。
				if(x_min <= nx && nx < x_max && y_min <= ny && ny < y_max && map[nx][ny] == 'W'){
						cluster.insert_pixels(nx, ny, weight[nx][ny]);
						st.push({nx, ny});
				}
			}
		}
	}
	return cluster;
};

void analysis::Fill_1Dhist(TH1D* &h1){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h1->Fill(weight[i][j]);
		}
	}
}

void analysis::Fill_2Dhist(TH2D* &h2){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h2->Fill(i, j, weight[i][j]);
		}
	}
}

ADC_DATA analysis::pedestal_subtract(){
	if(!opt_subtract) return weight;
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

THRESHOLD_MAP analysis::create_map(){
	THRESHOLD_MAP map(x_max - x_min, std::vector<char>(y_max - y_min));
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if((int)threshold < weight[i][j]){
				map[i][j] = 'W';
				if(opt_subtract) 
				weight[i][j] -= (int)threshold;
			}
			else{ 
				map[i][j] = '.';
				if(opt_subtract) 
				weight[i][j] = 0;
			}
		}
	}
	return map;
}

int analysis::call_dfs(){
	int count = 0;
	THRESHOLD_MAP map = origin_map;
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			if(map[i][j] == 'W'){
				block tmp = dfs(i, j, map);
				tmp.Set_eventnum(event_num);
				tmp.center_of_gravity(weight);
				if(tmp.Get_pixelsize() > 2) cluster.push_back(tmp);
				else{if(opt_subtract)weight[i][j] = 0, count--;}
				count++;
			}
		}
	}
	return count;
}

void analysis::highlight(TBox* &box){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if(origin_map[i][j] == 'W'){
				box = new TBox(i, j, i+1, j+1);
				box->SetLineColor(kRed);
				box->SetLineWidth(2);
				box->SetFillStyle(0); // 塗りつぶしなし
				box->Draw("same");
			}
		}
	}
}

void analysis::highlight(std::vector<TLine*> &lines){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			//閾値を超えているかの条件
			if(origin_map[i][j] == 'W'){
				//線を引く基準を決める条件達
				int dx[4] = {-1, 0, 0, 1}; int dy[4] = {0, -1, 1, 0};
				for(int k = 0; k < 4; k++){
					int nx = i + dx[k]; int ny = j + dy[k];
					if(x_min <= nx && nx < x_max && y_min <= ny && ny < y_max && origin_map[nx][ny] != 'W'){
						TLine *line;
						switch (k){
							case 0:{
								line = new TLine(i, j, i, j + 1);
								break;
							}
							case 1:{
								line = new TLine(i, j, i + 1, j);
								break;
							}
							case 2:{
								line = new TLine(i, j + 1, i + 1, j + 1);
								break;
							}
							
							default:{
								line = new TLine(i + 1, j, i + 1, j + 1);
								break;
							}
						}
						lines.push_back(line); // TLineオブジェクトをリストに追加
					}
				}
			}
		}
	}
	// すべてのラインを描画
	for(TLine* line : lines) {
		line->SetLineColor(kRed);
		line->SetLineWidth(2);
		line->Draw("same");
	}
}

void analysis::AnalyzeAndVisualizeClusters(){
	//1次元ヒストグラムの作成
	TH1D *h1 = new TH1D("h1", "ADC Distribution;ADC;Number of Entries", 100, 1000, 1500);
	Fill_1Dhist(h1);
	TCanvas *hist1D = new TCanvas("hist1D", "1D Histogram", 600, 400);
	h1->Draw();
	hist1D->Update();

	//閾値の設定
	threshold = h1->GetMean() + 5 * h1->GetStdDev();
	std::cout << "Threshold: " << threshold << std::endl;

	//2次元マップの作成
	origin_map = create_map();
	std::cout << "Cluster count: " << call_dfs() << std::endl;//0は一時的な修正

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
	Fill_2Dhist(h2);

	//閾値を超えた部分を強調表示
	TCanvas *hist2D = new TCanvas("hist2D", "2D Histogram", 600, 400);
	h2->Draw("COLZ");
	std::vector<TLine*> lines;
	if(opt_Red) {
		highlight(lines);
	}
	hist2D->Update();

}

void analysis::Find_AutoCluster(MyClass* myobj){
	//使用する変数。
	THRESHOLD_MAP map(x_max, std::vector<char>(y_max));
	double threshold;
	CLUSTER_DATA cluster;
	Long64_t nentries;
	ADC_DATA weight(128, std::vector<int>(128));
	//クラスターがあるエントリーナンバーを全て返す
	nentries = myobj->Get_EntryMax();
	std::vector<std::pair<int,int>> res(nentries);

	for(Long64_t entry_num = 0; entry_num < nentries; entry_num++){
		weight = myobj->Get_ADC(entry_num);

		TH1D *hist = new TH1D("hist", "1D Histogram;X;Entries", 100, 700, 1800);
		Fill_1Dhist(hist);
		//閾値の設定
		threshold = hist->GetMean() + 5 * hist->GetStdDev(); 
		delete hist;
		hist = nullptr;
		map = create_map();

		int ans = call_dfs();

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
	weight = myobj->Get_ADC(pixel_max);
	TH1D *hist = new TH1D("hist", "1D Histogram;X;Entries", 100, 700, 1800);
	Fill_1Dhist(hist);
	//閾値の設定
	threshold = hist->GetMean() + 3 * hist->GetStdDev(); 
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
	Position_Resolution();
}

void analysis::Position_Resolution(){
	int size = cluster.size();
	TH2D *pixels_pos = new TH2D("h2", "Position_Resolution", 30, 0, 30, 30, 0, 30);
	for(int i = 0; i < size; i++){
		std::pair<double, double> grav;
		grav.first = cluster[i].Get_xcenter();
		grav.second = cluster[i].Get_ycenter();
		//SOFIST3の30umの位置情報に変換(0を中央にする)
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

AnalyzeType analysis::runMyClass(param params) {
	MyClass *myobj = nullptr;
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
		weight = data;
		AnalyzeAndVisualizeClusters();
		delete myobj;
		return ANALYZE_ONE_EVENT;
	}
	else{
		myobj->Find_AutoCluster(opt_subtract);
		delete myobj;
		return ANALYZE_ALL_CLUSTERS;
	}

}

void analysis::closefile(){
	if(file->IsOpen()){	
	file->Close();
	delete file;
	file = nullptr;
	}
}
