/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
#include "../include/MyClass.hpp"
#include "../include/analysis.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TBox.h>
#include <TLine.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
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

void highlight(TBox* &box, THRESHOLD_MAP map){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if(map[i][j] == 'W'){
				box = new TBox(i, j, i+1, j+1);
				box->SetLineColor(kRed);
				box->SetLineWidth(2);
				box->SetFillStyle(0); // 塗りつぶしなし
				box->Draw("same");
			}
		}
	}
}

void highlight(std::vector<TLine*> &lines, THRESHOLD_MAP map){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			//閾値を超えているかの条件
			if(map[i][j] == 'W'){
				//線を引く基準を決める条件達
				int dx[4] = {-1, 0, 0, 1}; int dy[4] = {0, -1, 1, 0};
				for(int k = 0; k < 4; k++){
					int nx = i + dx[k]; int ny = j + dy[k];
					if(x_min <= nx && nx < x_max && y_min <= ny && ny < y_max && map[nx][ny] != 'W'){
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

void AnalyzeAndVisualizeClusters(ADC_DATA weight, bool opt_Red, bool opt_sub, bool opt_fit){
	//使用する変数。
	THRESHOLD_MAP original_map(x_max - x_min, std::vector<char>(y_max - y_min));
	THRESHOLD_MAP map(x_max - x_min, std::vector<char>(y_max - y_min));
	double threshold;
	CLUSTER_DATA cluster;

	//1次元ヒストグラムの作成
	TH1D *h1 = new TH1D("h1", "ADC Distribution;ADC;Number of Entries", 1600, 0, 8000);
	Fill_1Dhist(h1, weight);
	TCanvas *hist1D = new TCanvas("hist1D", "1D Histogram", 600, 400);
	h1->Draw();
	h1->GetXaxis()->SetRangeUser(h1->GetMean() - 6 * h1->GetStdDev(), h1->GetMean() + 6 * h1->GetStdDev());

	//閾値の設定
	threshold = h1->GetMean() + 3 * h1->GetStdDev();
	std::cout << "Threshold: " << threshold << std::endl;
	TLine* thre_line = new TLine(threshold, 0, threshold, 1.05 * h1->GetMaximum());
	thre_line->SetLineColor(kRed - 9);
	thre_line->SetLineWidth(2);
	thre_line->Draw("same");
	hist1D->Update();
	//2次元マップの作成
	original_map = create_map(weight, threshold, opt_sub);
	map = original_map;
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
	std::vector<TLine*> lines;
	if(opt_Red) {
		highlight(lines, original_map);
	}
	hist2D->Update();
	// p.pointer_share(h1, h2, box, hist2D);

}

void Position_Resolution(CLUSTER_DATA data){
	int size = data.size();
	TH2D *pixels_pos = new TH2D("h2", "Position_Resolution", 30, 0, 30, 30, 0, 30);
	for(int i = 0; i < size; i++){
		std::pair<double, double> grav;
		grav.first = data[i].Get_xcenter();
		grav.second = data[i].Get_ycenter();
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



// フィット結果を取得
// double mean = gaus->GetParameter(1);
// double sigma = gaus->GetParameter(2);
// double chi2 = gaus->GetChisquare();
// int ndf = gaus->GetNDF();
// double pvalue = gaus->GetProb();

// ヒストグラムの色の範囲を固定
//h2->SetMinimum(1000); // 最小値を設定
//h2->SetMaximum(1500); // 最大値を設定