#include "../include/block.hpp"
#include <TCanvas.h>
#include <TH1D.h>

block::block(){

}

block::block(ADC_DATA &weight){
	center_of_gravity(weight);
}

// クラスターのピクセルの数を出力する。
int block::Get_pixel_count() const{
    return place.size();
}
void block::Set_eventnum(int input){
	event_num = input;
}

int block::Get_eventnum() const{
	return event_num;
}

double block::Get_xcenter() const{ 
	return x_g;
}

double block::Get_ycenter() const{
	return y_g;
}
double block::Get_ADCsum() const{
	return ADCsum;
}
double block::Get_xmin() const{
	return x_min;
}
double block::Get_xmax() const{
	return x_max;
}
double block::Get_ymin() const{
	return y_min;
}
double block::Get_ymax() const{
	return y_max;
}
// 重心を計算して返り値として pair の first に x 軸の重心の値、second に y 軸の重心の値を返す。
std::pair<double, double> block::center_of_gravity(ADC_DATA &weight) {
    x_g = 0.0, y_g = 0.0, ADCsum = 0.0;
    std::pair<double, double> res;
    for (auto &a : place) {
        x_g += weight[a.first][a.second] * a.first;
        y_g += weight[a.first][a.second] * a.second;
        ADCsum += static_cast<double>(weight[a.first][a.second]);
    }
    // std::cout << "ADC合計値:" << ADCsum << "\n";
    x_g = x_g / ADCsum;
    y_g = y_g / ADCsum;
    res.first = x_g;
    res.second = y_g;
    return res;
}

// 最大値と最小値を計算する。
void block::Calculate_min_max() {
	for (auto &a : place) {
		x_min = std::min(x_min, a.first);
		x_max = std::max(x_max, a.first);
		y_min = std::min(y_min, a.second);
		y_max = std::max(y_max, a.second);
	}
}

// 廃棄予定
void block::Print_NormalDistribution(ADC_DATA &weight) {
	TCanvas c2("c2", "1D hist", 600, 500);
    TH1D h1("h1", "1D Histogram;X;Entries", 100, 0, 1500);
    for (auto &a : place) {
        h1.Fill(weight[a.first][a.second]);
    }
    h1.Draw();
    c2.Update();
    c2.WaitPrimitive();
}

bool block::operator<(const block& other) const {
	return Get_pixel_count() < other.Get_pixel_count();
}