#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "alias.hpp"
#include <set>

class block{
public:
    bool flag = false;
    std::set<std::pair<int, int>> place;
    int Get_pixel_count();
    double Get_xcenter();
    double Get_ycenter();
    double Get_ADCsum();
	double Get_xmin();
	double Get_xmax();
	double Get_ymin();
	double Get_ymax();
    std::pair<double, double> center_of_gravity(ADC_DATA &weight);
	void Calculate_min_max();
    void Print_NormalDistribution(ADC_DATA &weight);

private:
    int i;
    double x_g, y_g, ADCsum;
	int x_min = 0, x_max = 128, y_min = 0, y_max = 128;
};

#endif
