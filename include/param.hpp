#ifndef PARAM_HPP
#define PARAM_HPP
#include <TString.h>
#include <utility>

class inparam{
	public:
	double num_sigma;
	int filter_clusterSize;
	bool opt_Red, opt_Subtract, opt_Fitting;
	bool AutoCluster;
	int num_entry;
	TString Enter_path;
	public:
    // コンストラクタ
    inparam(double sigma, int clusterSize, bool red, bool subtract, bool fitting, bool autoCluster, int entry, TString path) 
	: num_sigma(sigma), filter_clusterSize(clusterSize), opt_Red(red), opt_Subtract(subtract), opt_Fitting(fitting), AutoCluster(autoCluster), num_entry(entry), Enter_path(path) {		
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
	param(double sigma, int clusterSize, bool red, bool subtract, bool fitting, bool autoCluster, int entry, TString path)
	:in(sigma, clusterSize, red, subtract, fitting, autoCluster, entry, path), cluster_count(0){
	}
};
#endif