#include "../include/param.hpp"
#include <utility>

double outparam::Get_threshold() const{
	return threshold;
}

int outparam::Get_Cluster_count() const{
	return cluster_count;
}

int outparam::Get_ADC_sum() const{
	return ADC_sum;
}

std::pair<double, double> outparam::Get_Center_of_Gravity() const{
	return Center_of_Gravity;
}

void outparam::Set_threshold(double input_threshold){
	threshold = input_threshold;
}

void outparam::Set_Cluster_count(int input_count){
	cluster_count = input_count;
}

void outparam::Set_ADC_sum(int input_ADC_sum){
	ADC_sum = input_ADC_sum;
}

void outparam::Set_Center_of_Gravity(std::pair<double, double> input_Center_of_Gravity){
	Center_of_Gravity = input_Center_of_Gravity;
}