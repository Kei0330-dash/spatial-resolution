#ifndef PARAM_HPP
#define PARAM_HPP
#include <utility>

class param{
	private:
	double threshold;
	int cluster_count;
	int ADC_sum;
	std::pair<double, double> Center_of_Gravity;
	public:
	double Get_threshold() const;
	int Get_Cluster_count() const;
	int Get_ADC_sum() const;
	std::pair<double, double> Get_Center_of_Gravity() const;
	void Set_threshold(double input_threshold);
	void Set_Cluster_count(int input_count);
	void Set_ADC_sum(int input_ADC_sum);
	void Set_Center_of_Gravity(std::pair<double, double> input_Center_of_Gravity);
};

#endif