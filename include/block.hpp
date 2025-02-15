#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "alias.hpp"
#include <set>
#include <utility>

class block{
public:
	block();
	//重心を求めた状態で初期化
	block(ADC_DATA &weight);
    int Get_pixelsize() const;
	void   Set_eventnum(int input);
	int    Get_eventnum() const;
    double Get_xcenter() const;
    double Get_ycenter() const;
    double Get_ADCsum() const;
	double Get_xmin() const;
	double Get_xmax() const;
	double Get_ymin() const;
	double Get_ymax() const;
	void insert_pixels(int x, int y, int weight);
    std::pair<double, double> center_of_gravity(ADC_DATA &weight);
	void Calculate_min_max();
    void Print_NormalDistribution(ADC_DATA &weight);
	// ソートのために"<"演算子をオーバーロード
    bool operator<(const block& other) const;

private:
    int i = 0;
    double x_g = 0.0, y_g = 0.0, ADCsum = 0.0;
	int x_min = 0, x_max = 128, y_min = 0, y_max = 128;
	int event_num = 0;
	std::set<std::tuple<int, int, int>> place;
};

#endif
