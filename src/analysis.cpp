/*平均値+3σを閾値とした、クラスターを探すプログラムに加えその他諸々一杯機能が詰まったプログラム*/
#define MyClass_cxx
#include "../include/MyClass.hpp"
#include "../include/analysis.hpp"
#include "../include/param.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TBox.h>
#include <TLine.h>
#include <TText.h>
#include <TMath.h>
#include <TRegexp.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <algorithm>
#include <memory>

block analysis::dfs(int x, int y, THRESHOLD_MAP &map){
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

void analysis::Fill_1Dhist(std::shared_ptr<TH1D> h1){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h1->Fill(weight[i][j]);
		}
	}
}

void analysis::Fill_2Dhist(std::shared_ptr<TH2D> h2){
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			h2->Fill(i, j, weight[i][j]);
		}
	}
}

ADC_DATA analysis::pedestal_subtract(){
	if(!opt_subtract) return weight;
	ADC_DATA ADC = weight;
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			if((int)threshold < ADC[i][j]){
				ADC[i][j] -= (int)threshold;
			}
			else{ 
				ADC[i][j] = 0;
			}
		}
	}
	return ADC;
}

PIXEL_MEANS analysis::Get_one_pixel_means() {
	int k_max = myobj->Get_EntryMax();
	PIXEL_MEANS res((x_max - x_min), std::vector<double>(y_max - y_min));
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			TH1D* h1 = new TH1D();
			for(int k = 0; i < k_max; k++){
				int W = myobj->Get_ADC_one_Event(k, i, j);	
				h1->Fill(W);
			}
			res[i][j] = create_threshold(h1->GetMean(), h1->GetStdDev());
			delete h1;
		}
	}
	return res;
}

ADC_DATA analysis::means_subtract() {
	if(means.empty()){
		means = Get_one_pixel_means();
	}
	ADC_DATA adc = weight;
	for(Long64_t i = x_min; i < x_max; i++){
		for(Long64_t j = y_min; j < y_max; j++){
			adc[i][j] -= means[i][j];
		}
	}
	return adc;
}

THRESHOLD_MAP analysis::create_map(){
	THRESHOLD_MAP map(x_max - x_min, std::vector<char>(y_max - y_min));
	if(opt_subtract){
		for(Long64_t i = x_min; i < x_max; i++){
			for(Long64_t j = y_min; j < y_max; j++){
				if(0 < weight[i][j]){
					map[i][j] = 'W';
				}
				else{ 
					map[i][j] = '.';
				}
			}
		}
	}
	else{
		for(Long64_t i = x_min; i < x_max; i++){
			for(Long64_t j = y_min; j < y_max; j++){
				if((int)threshold < weight[i][j]){
					map[i][j] = 'W';
				}
				else{ 
					map[i][j] = '.';
				}
			}
		}
	}
	return map;
}

int analysis::call_dfs(){
	THRESHOLD_MAP map = origin_map;
	int before_Size = cluster.size();
	for(int i = x_min; i < x_max; i++){
		for(int j = y_min; j < y_max; j++){
			if(map[i][j] == 'W'){
				block tmp = dfs(i, j, map);
				tmp.Set_eventnum(event_num);
				tmp.center_of_gravity(weight);
				if(tmp.Get_pixelsize() > FilterSIZE) {
					cluster.push_back(tmp);
				} 
			}
		}
	}
	return cluster.size() - before_Size;
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

void analysis::highlightv2(){
	if(!opt_Red) return;
	int size = cluster.size();
	std::vector<std::shared_ptr<TLine>> lines;
	for(int ci = 0; ci < size; ci++){
		std::set<std::tuple<int, int, int>> place;
		place = cluster[ci].Get_set();
		for(auto &a: place){
			int i = std::get<0>(a);
			int j = std::get<1>(a);
			//閾値を超えているかの条件
			if(origin_map[i][j] == 'W'){
				//線を引く基準を決める条件達
				int dx[4] = {-1, 0, 0, 1}; int dy[4] = {0, -1, 1, 0};
				for(int k = 0; k < 4; k++){
					int nx = i + dx[k]; int ny = j + dy[k];
					if(x_min <= nx && nx < x_max && y_min <= ny && ny < y_max && origin_map[nx][ny] != 'W'){
						std::shared_ptr<TLine> line;
						switch (k){
							case 0:{
								line = std::make_shared<TLine>(i, j, i, j + 1);
								break;
							}
							case 1:{
								line = std::make_shared<TLine>(i, j, i + 1, j);
								break;
							}
							case 2:{
								line = std::make_shared<TLine>(i, j + 1, i + 1, j + 1);
								break;
							}
							
							default:{
								line = std::make_shared<TLine>(i + 1, j, i + 1, j + 1);
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
	for(auto line : lines) {
		p.share(line);
		line->SetLineColor(kRed);
		line->SetLineWidth(2);
		line->Draw("same");
	}
	return;
}

void analysis::AdjustBinsToIntegers(std::shared_ptr<TH1D> histgram, Int_t step, Int_t start) {
    for (Int_t i = 1; i <= histgram->GetNbinsX(); ++i) {
        if ((i - 1) % step == start) {
            histgram->GetXaxis()->SetBinLabel(i, Form("%d", i - 1));
        } else {
            histgram->GetXaxis()->SetBinLabel(i, "");
        }
    }
    histgram->SetLabelSize(0.05, "x");
}

int analysis::make_step(int min, int max){
	int res = ((max - min) /10) + 1;
	return res;
}

void analysis::SaveCanvasWithHVPart(TCanvas* c1, const TString& path, const TString& customInfo) {
    // 正規表現で"HV"と数字部分を抽出
    TRegexp reg("HV[0-9]+");
    Ssiz_t pos = path.Index(reg);
    
    // 抽出した部分をファイル名に使用
    if (pos != kNPOS) {
        TString hvPart = path(reg);
        TString newFileName = Form("../results/%s_%s.pdf", hvPart.Data(), customInfo.Data());
        
        // キャンバスを保存
        c1->SaveAs(newFileName);
        std::cout << "Saved as: " << newFileName << std::endl;
    } else {
        std::cout << "HV部分が見つかりませんでした。" << std::endl;
    }
}

void analysis::AnalyzeAndVisualizeClusters(){
	//1次元ヒストグラムの作成
	std::shared_ptr<TH1D> h1 = std::make_shared<TH1D>("h1", "ADC Distribution;ADC;Number of Entries", 1600, 0, 8000);
	p.share(h1);
	Fill_1Dhist(h1);
	h1->GetXaxis()->SetRangeUser(h1->GetMean() - 6 * h1->GetStdDev(), h1->GetMean() + 6 * h1->GetStdDev());
	//閾値の設定
	threshold = create_threshold(h1->GetMean(), h1->GetStdDev());
	std::cout << "Threshold: " << threshold << std::endl;
	std::shared_ptr<TCanvas> hist1D = std::make_shared<TCanvas>("hist1D", "1D Histogram", 600, 400);
	p.share(hist1D);
	std::shared_ptr<TLine> thre_line = std::make_shared<TLine>(threshold, 0, threshold, 1.05 * h1->GetMaximum());
	p.share(thre_line);
	thre_line->SetLineColor(kRed - 9);
	thre_line->SetLineWidth(2);
	h1->Draw();
	thre_line->Draw("same");
	hist1D->Update();

	//2次元マップの作成
	weight = pedestal_subtract();
	origin_map = create_map();
	cluster_found = call_dfs();
	std::cout << "Cluster count: " << cluster_found << std::endl;//0は一時的な修正

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
	std::shared_ptr<TH2D> h2 = std::make_shared<TH2D>("h2", "Pixel Distribution;X;Y", (x_max - x_min), x_min, x_max, (y_max - y_min), y_min, y_max);
	p.share(h2);
	h2->Sumw2();
	h2->SetStats(0);
	Fill_2Dhist(h2);

	//閾値を超えた部分を強調表示
	std::shared_ptr<TCanvas> hist2D = std::make_shared<TCanvas>("hist2D", "2D Histogram", 600, 400);
	p.share(hist2D);
	h2->Draw("COLZ");
	highlightv2();
	hist2D->Update();
}

void analysis::AnalyzeAndVisualizeClusters(MyClass* myobj){
	// //1次元ヒストグラムの作成
	// TH1D *h1 = new TH1D("h1", "ADC Distribution;ADC;Number of Entries", 1600, 0, 8000);
	// p.share(h1);
	// int nentries = myobj->Get_EntryMax();
	// for(int i = 0; i < nentries; i++){
	// 	weight = myobj->Get_ADC(i);
	// 	Fill_1Dhist(h1);
	// }
	// h1->GetXaxis()->SetRangeUser(h1->GetMean() - 6 * h1->GetStdDev(), h1->GetMean() + 6 * h1->GetStdDev());

	// TCanvas *hist1D = new TCanvas("hist1D", "1D Histogram", 600, 400);
	// p.share(hist1D);
	// h1->Draw();
	// hist1D->Update();

	// //閾値の設定
	// threshold = create_threshold(h1->GetMean(), h1->GetStdDev());
	// std::cout << "Threshold: " << threshold << std::endl;

	// //2次元マップの作成
	// weight = pedestal_subtract();
	// origin_map = create_map();
	// cluster_found = call_dfs();
	// std::cout << "Cluster count: " << cluster_found << std::endl;//0は一時的な修正

	// //クラスターがある場合、重心を求める
	// if(!cluster.empty()){
	// 	for(int i = 0; i < cluster.size(); i++){
	// 		std::pair<double, double> ans;
	// 		ans.first = cluster[i].Get_xcenter();
	// 		ans.second = cluster[i].Get_ycenter();
	// 		std::cout << "Center of gravity: " << ans.first << ", " << ans.second << std::endl;
	// 	}
	// }

	// //2次元ヒストグラムの作成
	// TH2D *h2 = new TH2D("h2", "Pixel Distribution;X;Y", (x_max - x_min), x_min, x_max, (y_max - y_min), y_min, y_max);
	// p.share(h2);
	// h2->Sumw2();
	// h2->SetStats(0);
	// Fill_2Dhist(h2);

	// //閾値を超えた部分を強調表示
	// TCanvas *hist2D = new TCanvas("hist2D", "2D Histogram", 600, 400);
	// p.share(hist2D);
	// h2->Draw("COLZ");
	// std::vector<TLine*> lines;
	// p.share(lines);
	// if(opt_Red) {
	// 	highlight(lines);
	// }
	// hist2D->Update();
}

void analysis::Find_AutoCluster(){
	gStyle->SetOptStat(0);
	//使用する変数。
	Long64_t nentries;
	//クラスターがあるエントリーナンバーを全て返す
	nentries = myobj->Get_EntryMax();
	std::vector<std::pair<int,int>> res(nentries);

	for(Long64_t entry_num = 0; entry_num < nentries; entry_num++){
		weight = myobj->Get_ADC(entry_num);

		std::shared_ptr<TH1D> hist = std::make_shared<TH1D>("hist", "1D Histogram;X;Entries", 100, 700, 1800);
		Fill_1Dhist(hist);
		//閾値の設定
		threshold = hist->GetMean() + num_sigma * hist->GetStdDev(); 
		weight = pedestal_subtract();
		origin_map = create_map();

		event_num = entry_num;
		int ans = call_dfs();

		res[entry_num] = std::make_pair(entry_num, ans); 
		
	}
	std::shared_ptr<TH1D> h1 = std::make_shared<TH1D>("h1", "Number of Clusters per Event;Number of Clusters;Number of Events", 1000, 0, 1000);
	p.share(h1);
	std::shared_ptr<TH1D> h2 = std::make_shared<TH1D>("h2", "Cluster Size Distribution;Cluster Size;Number of Events", 1000, 0, 1000);
	p.share(h2);
	std::shared_ptr<TH1D> h3 = std::make_shared<TH1D>("h3", "Distribution of Clusters in number of Events;Entry num;Number of Events", nentries / 10, 0, nentries);
	p.share(h3);
	std::shared_ptr<TH2D> h4 = std::make_shared<TH2D>("h4", "The Event with the Largest Pixel Size;X;y", (x_max - x_min) , x_min, x_max, (y_max - y_min), y_min, y_max);
	p.share(h4);

	// X軸とY軸のラベルから小数点を非表示に設定
	// h1->GetYaxis()->SetNdivisions(505);
	std::shared_ptr<TCanvas> c1 = std::make_shared<TCanvas>("c1", "Information about clusters", 1000, 1000);
	p.share(c1);
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
	std::shared_ptr<TH1D> hist = std::make_shared<TH1D>("hist", "1D Histogram;X;Entries", 100, 700, 1800);
	p.share(hist);
	Fill_1Dhist(hist);
	//閾値の設定
	threshold = create_threshold(hist->GetMean(), hist->GetStdDev()); 
	weight = pedestal_subtract();
	Fill_2Dhist(h4);
	int h1_min = h1->GetBinCenter(h1->FindFirstBinAbove(0)), h1_max = h1->GetBinCenter(h1->FindLastBinAbove(0));
	int h2_min = h2->GetBinCenter(h2->FindFirstBinAbove(0)), h2_max = h2->GetBinCenter(h2->FindLastBinAbove(0));

	h1->GetXaxis()->SetRangeUser(h1_min, h1_max + 1);
	h2->GetXaxis()->SetRangeUser(h2_min, h2_max + 1);
	h1->SetFillColor(kAzure-9);
	h2->SetFillColor(kGreen-9);
	h3->SetFillColor(kPink-9);
	
	AdjustBinsToIntegers(h1, make_step(h1_min, h1_max));
	// AdjustBinsToIntegers(h2, make_step(h2_min, h2_max), 1);

	std::shared_ptr<TText> text3 = std::make_shared<TText>(50, (h3->GetMaximum()), "Separated every 10 events");
	p.share(text3);
	// pixel_maxの情報を追加
    std::shared_ptr<TText> text4 = std::make_shared<TText>(0, -15, Form("max_event:%lld", pixel_max));
	p.share(text4);
	c1->cd(1); h1->Draw("");
	c1->cd(2); h2->Draw("");
	c1->cd(3); h3->Sumw2(0);  h3->Draw(""); text3->Draw();
	c1->cd(4); h4->Draw(""); text4->Draw();
	c1->Update();
	//SaveCanvasWithHVPart(c1, path, "info");
	Position_Resolution();
}

void analysis::Position_Resolution(){
	int size = cluster.size();
	std::shared_ptr<TH2D> pixels_pos = std::make_shared<TH2D>("pixels_pos", "Position_Resolution", 30, 0, 30, 30, 0, 30);
	pixels_pos->Reset();
	p.share(pixels_pos);
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
	std::shared_ptr<TCanvas> hist2D = std::make_shared<TCanvas>("hist2D", "Position_Resolution", 600, 600);
	p.share(hist2D);
	pixels_pos->Draw("COLZ");
	hist2D->Update();
	// SaveCanvasWithHVPart(hist2D, path, "pos");
	std::shared_ptr<TCanvas> hist1D_pjX = std::make_shared<TCanvas>("hist1D_pjx", "ProjectionX", 600, 600);
	p.share(hist1D_pjX);
	std::shared_ptr<TH1D> projX(pixels_pos->ProjectionX(), [](TH1D* ptr) {
		if (ptr) {
			delete ptr; // メモリを解放
		}
	});
	projX->SetTitle("ProjectionX");
	double stddev_X = projX->GetStdDev();
	std::shared_ptr<TText> text1 = std::make_shared<TText>(20, (projX->GetMaximum()), Form("sigma: %.2f", stddev_X));
	p.share(text1);
	p.share(projX);
	projX->Sumw2(0);
	projX->Draw();
	text1->Draw();
	hist1D_pjX->Update();
	// SaveCanvasWithHVPart(hist1D_pjX, path, "pjx");
	std::shared_ptr<TCanvas> hist1D_pjY = std::make_shared<TCanvas>("hist1D_pjY", "ProjectionY", 600, 600);
	p.share(hist1D_pjY);
	// TH1D* を shared_ptr にラップして管理
	std::shared_ptr<TH1D> projY(pixels_pos->ProjectionY(), [](TH1D* ptr) {
		if (ptr) {
			delete ptr; // メモリを解放
		}
	});
	projY->SetTitle("ProjectionY");
	p.share(projY);
	double stddev_Y = projY->GetStdDev();
	std::shared_ptr<TText> text2 = std::make_shared<TText>(20, (projY->GetMaximum()), Form("sigma: %.2f", stddev_Y));
	p.share(text2);
	projY->Sumw2(0);
	projY->Draw();
	text2->Draw();
	hist1D_pjY->Update();
	// SaveCanvasWithHVPart(hist1D_pjY, path, "pjy");
}

void analysis::read_param(param params){
	opt_Red = params.in.opt_Red;
	opt_subtract = params.in.opt_Subtract;
	opt_Fitting = params.in.opt_Fitting;
	opt_AutoCluster = params.in.AutoCluster;
	event_num = params.in.num_entry;
	path = params.in.Enter_path;
	num_sigma = params.in.num_sigma;
	FilterSIZE = params.in.filter_clusterSize;
}

void analysis::write_param(param &params){
	params.out.threshold = threshold;
	params.out.cluster_found = cluster_found;
	params.out.cluster = cluster;
}

double analysis::create_threshold(double mean, double stddev){
	return mean + num_sigma * stddev;
}

analysis::analysis(){
}

analysis::~analysis(){
	// clear_pointer();
	// closefile();
}

AnalyzeType analysis::runMyClass(param &params) {
    read_param(params);

    // ファイルが既に開かれていない場合
    if (!file) {
        file = std::shared_ptr<TFile>(TFile::Open(path), [](TFile* ptr) {
            if (ptr) {
                ptr->Close(); // TFileを適切に閉じる
                delete ptr;   // メモリを解放
            }
        });

        // ファイルが開けない場合やZombieの場合のエラーチェック
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file: " << path << std::endl;
            return ANALYZE_ERROR;
        }
    }

    // TTreeを取得
    TTree* SOFIST_Data = dynamic_cast<TTree*>(file->Get("SOFIST_Data"));
    if (!SOFIST_Data) {
        std::cerr << "Error: TTree 'SOFIST_Data' not found in file." << std::endl;
        return ANALYZE_ERROR;
    }

    printf("ok\n");

    // myobjをスマートポインタで管理
    myobj = std::make_unique<MyClass>(SOFIST_Data);

    // クラスターの分析処理
    if (!opt_AutoCluster) {
        weight = myobj->Get_ADC(event_num);
        AnalyzeAndVisualizeClusters();
        write_param(params);
        return ANALYZE_ONE_EVENT;
    } else {
        Find_AutoCluster();
        return ANALYZE_ALL_CLUSTERS;
    }
}


void analysis::closefile(){
	if(file->IsOpen()){	
		file->Close();
		file.reset();
		file = nullptr;
	}
}

void analysis::clear_pointer(){
	p.clear();
}

void analysis::init_DataStructure(){
	cluster.clear();
	weight.clear();
	origin_map.clear();
}