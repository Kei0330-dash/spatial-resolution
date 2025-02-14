#ifndef PARAM_HPP
#define PARAM_HPP
#include <utility>

class inparam{
	public:
	double num_sigma;
	int filter_clusterSize;
	bool opt_Red, opt_subtract, opt_Fitting;
	bool AutoCluster;
	int num_entry;
	public:
    // コンストラクタ
    inparam(double sigma, int clusterSize, bool red, bool subtract, bool fitting, bool autoCluster, int entry) 
	: num_sigma(sigma), filter_clusterSize(clusterSize), opt_Red(red), opt_subtract(subtract), opt_Fitting(fitting), AutoCluster(autoCluster), num_entry(entry) {		
	}
};

class outparam{
	public:
	double threshold;
	int cluster_size;
	int ADC_sum;
	std::pair<double, double> Center_of_Gravity;
};

class param{
	public:
	inparam in;
	int cluster_count;
	outparam out;
	param(double sigma, int clusterSize, bool red, bool subtract, bool fitting, bool autoCluster, int entry)
	:in(sigma, clusterSize, red, subtract, fitting, autoCluster, entry), cluster_count(0){
	}
};
#endif