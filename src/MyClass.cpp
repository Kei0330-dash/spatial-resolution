#include "../include/MyClass.hpp"
#include <vector>
int MyClass::Get_EntryMax(){
	// if(fChain == nullptr) return;
	int ans = fChain->GetEntriesFast();
	return ans;
}

ADC_DATA MyClass::Get_ADC(Int_t entry_num){
	Long64_t nentries;
	//エントリー数を指定する
	// if (fChain == nullptr) return;
	nentries = fChain->GetEntriesFast();
	fChain->GetEntry(entry_num);

	ADC_DATA res(128, std::vector<int>(128));
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 128; j++){
			res[i][j] = ADC[i][j];
		}
	}
	return res;
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
