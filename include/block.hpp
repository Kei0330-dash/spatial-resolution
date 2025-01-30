#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "alias.hpp"
#include <set>

class block{
public:
    bool flag = false;
    std::set<std::pair<int, int>> place;
    int Get_pixel_count() const;
	void   Set_eventnum(int input);
	int    Get_eventnum() const;
    double Get_xcenter() const;
    double Get_ycenter() const;
    double Get_ADCsum() const;
	double Get_xmin() const;
	double Get_xmax() const;
	double Get_ymin() const;
	double Get_ymax() const;
    std::pair<double, double> center_of_gravity(ADC_DATA &weight);
	void Calculate_min_max();
    void Print_NormalDistribution(ADC_DATA &weight);
	// ソートのために"<"演算子をオーバーロード
    bool operator<(const block& other) const;

private:
    int i;
    double x_g, y_g, ADCsum;
	int x_min = 0, x_max = 128, y_min = 0, y_max = 128;
	int event_num;
};

#endif
