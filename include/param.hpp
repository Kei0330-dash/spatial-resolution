#ifndef PARAM_HPP
#define PARAM_HPP
#include "alias.hpp"
#include <TString.h>
#include <utility>

class inparam{
	public:
	double num_sigma;
	int filter_clusterSize;
	bool opt_Red, opt_Subtract, opt_Fitting;
	bool opt_meanSubtract;
	bool AutoCluster;
	int num_entry;
	TString Enter_path;
	public:
    // コンストラクタ
    inparam(double sigma, int clusterSize, bool red, bool subtract, bool meansubtract, bool fitting, bool autoCluster, int entry, TString path) 
	: num_sigma(sigma), filter_clusterSize(clusterSize), opt_Red(red), opt_Subtract(subtract), opt_meanSubtract(meansubtract), opt_Fitting(fitting), AutoCluster(autoCluster), num_entry(entry), Enter_path(path) {		
	}
};

class outparam{
	public:
	double threshold;
	int cluster_found;
	CLUSTER_DATA cluster;
};

class param{
	public:
	inparam in;
	outparam out;
	param(double sigma, int clusterSize, bool red, bool subtract, bool mean,bool fitting, bool autoCluster, int entry, TString path)
	:in(sigma, clusterSize, red, subtract, mean, fitting, autoCluster, entry, path) {
	}
};
#endif