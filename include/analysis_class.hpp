#ifndef ANALYSISCLASS_HPP
#define ANALYSISCLASS_HPP

#include "alias.hpp"
#include "MyClass.hpp"
#include "block.hpp"
#include "param.hpp"
#include "mem_root.hpp"
#include <TApplication.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TF2.h>
#include <TLine.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <algorithm>

// ========================================================================================================
// プロトタイプ宣言
// ========================================================================================================
class analysis{
	private:
	const int x_min = 0, x_max = 128;
	const int y_min = 0, y_max = 128;
	TFile *file = nullptr;
	mem_root p;
	//ユーザーによって設定された値
	bool opt_Red, opt_subtract, opt_Fitting, opt_AutoCluster;
	double num_sigma;
	int FilterSIZE;
	int event_num;
	TString path;
	// ------------------------------
	ADC_DATA weight;
	THRESHOLD_MAP origin_map;
	CLUSTER_DATA cluster;
	double threshold;
	/// @brief 深さ優先探索を実行してクラスターの塊を走査する。ただし、直接呼ばずint call_dfs()を経由して呼び出すこと。
	/// @param x ピクセルのx軸
	/// @param y ピクセルのy軸
	/// @param map dfsで既にいったことがあるかを確認する。
	/// @return クラスターの塊としての情報を返す。
	block dfs(int x, int y, THRESHOLD_MAP &map);
	/// @brief 1次元のヒストグラムを作る。
	/// @param h1 動的に確保したROOTの1次元ヒストグラム
	void Fill_1Dhist(TH1D* &h1);
	/// @brief 2次元のヒストグラムを作る。
	/// @param h2 動的に確保したROOTの2次元ヒストグラム
	void Fill_2Dhist(TH2D* &h2);
	/// @brief ペデスタル減算をする。
	/// @return 減算後の2次元の配列値を出力する。
	ADC_DATA pedestal_subtract();
	/// @brief 深さ優先探索をするためのmapを作成する関数。
	/// @return 閾値が超えた情報を格納した2次元の配列を返す。
	THRESHOLD_MAP create_map();
	/// @brief 深さ優先探索を呼び出す関数。これを使うことでそのイベントのクラスターの数がわかる。
	/// @return そのイベントのクラスターの数を出力する。
	int call_dfs();
	/// @brief この関数を起動すると、クラスターを強調表示します。
	/// @param box このclassに強調表示の情報がはいっています。
	void highlight(TBox* &box);
	/// @brief この関数を起動すると、クラスターの外側を囲います。
	/// @param lines このclassに外側の線が入っている。
	void highlight(std::vector<TLine*> &lines);
	/// @brief ある1イベントを可視化する関数
	void AnalyzeAndVisualizeClusters();
	void Find_AutoCluster(MyClass* myobj);
	/// @brief 位置分解能を評価するための関数
	void Position_Resolution();
	//入力パラメータを読み込む
	void read_param(param params);
	void write_param(param &params);
	public:
	analysis();
	~analysis();
	AnalyzeType runMyClass(param params);
	void closefile();
	void clear_pointer();
};

// ========================================================================================================

#endif