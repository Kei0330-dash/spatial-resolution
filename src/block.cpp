#include "../include/block.hpp"
#include <TCanvas.h>
#include <TH1D.h>

block::block(){

}

block::block(ADC_DATA &weight){
	center_of_gravity(weight);
}

// クラスターのピクセルの数を出力する。
int block::Get_pixelsize() const{
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

int block::Get_ADCsum() const{
	return (int)ADCsum;
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

std::set<std::tuple<int, int, int>> block::Get_set() const{
	return place;
}


void block::insert_pixels(int x, int y, int weight){
	place.insert(std::make_tuple(x, y, weight));
}

// 重心を計算して返り値として pair の first に x 軸の重心の値、second に y 軸の重心の値を返す。
std::pair<double, double> block::center_of_gravity(ADC_DATA &weight) {
    x_g = 0.0, y_g = 0.0, ADCsum = 0.0;
    std::pair<double, double> res;
    for (auto &a : place) {
        x_g += std::get<2>(a) * std::get<0>(a);
        y_g += std::get<2>(a) * std::get<1>(a);
        ADCsum += static_cast<double>(std::get<2>(a));
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
		x_min = std::min(x_min, std::get<0>(a));
		x_max = std::max(x_max, std::get<0>(a));
		y_min = std::min(y_min, std::get<1>(a));
		y_max = std::max(y_max, std::get<1>(a));
	}
}

// 廃棄予定
void block::Print_NormalDistribution(ADC_DATA &weight) {
	// TCanvas c2("c2", "1D hist", 600, 500);
    // TH1D h1("h1", "1D Histogram;X;Entries", 100, 0, 1500);
    // for (auto &a : place) {
    //     h1.Fill(weight[a.first][a.second]);
    // }
    // h1.Draw();
    // c2.Update();
    // c2.WaitPrimitive();
}

bool block::operator<(const block& other) const {
	return Get_pixelsize() < other.Get_pixelsize();
}