#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

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
#include <TH1D.h>
#include <TMath.h>
#include <TRegexp.h>
#include <iostream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <algorithm>


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
	int cluster_found;
	std::vector<std::vector<double>> thresholds;
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
	/// @brief この関数を起動すると、クラスターの外側を囲います(現在非推奨)。
	/// @param lines このclassに外側の線が入っている。
	void highlight(std::vector<TLine*> &lines);
	/// @brief この関数を起動すると、クラスターの外側を囲います。
	/// @param lines このclassに外側の線の情報が格納されている。
	void highlightv2(std::vector<TLine*> &lines);
	/// @brief ヒストグラムのビンを整数し、かつ0.5の位置に整数を配置する
	/// @param hist 対象のヒストグラム
	/// @param step 指定したステップごとにラベルを表示するデフォルト引数は1
	void AdjustBinsToIntegers(TH1D* histgram, Int_t step = 1);
	/// @brief 必要なステップ数を計算する
	/// @param min x軸のビンが入っている最小値 
	/// @param max x軸のビンが入っている最大値
	/// @return ステップ数を返す
	int make_step(int min, int max);
	/// @brief 
	/// @param c1 
	/// @param path 
	/// @param customInfo 
	void SaveCanvasWithHVPart(TCanvas* c1, const TString& path, const TString& customInfo);
	/// @brief ある1イベントをその1イベントの閾値で作成し可視化する関数
	void AnalyzeAndVisualizeClusters();
	/// @brief ある1イベントを全てのイベントの同じ座標のピクセルを用いて閾値を作成し、可視化する関数
	/// @param myobj このクラスを用いて、全イベント分のデータを読み込むため必要。
	void AnalyzeAndVisualizeClusters(MyClass* myobj);
	/// @brief 左上からそれぞれクラスター数（青），クラスターサイズ（緑），10イベントごとのクラスター数（マゼンタ），クラスターサイズが一番大きかったイベントをプロットしたものを出力
	/// @param myobj このクラスを用いて、全イベント分のデータを読み込むため必要。
	void Find_AutoCluster(MyClass* myobj);
	/// @brief 位置分解能を評価するための関数
	void Position_Resolution();
	/// @brief 入力パラメータを読み込む
	void read_param(param params);
	/// @brief 出力パラメータを書き込む
	void write_param(param &params);
	/// @brief 1イベントのADC値を基に閾値を作成する
	/// @param mean 平均値
	/// @param stddev 標準偏差
	/// @return 閾値を返す
	double create_threshold(double mean, double stddev);

	public:
	/// @brief コンストラクタ。特に処理はなし。なんか追加するようであれば。追加する
	analysis();
	/// @brief デストラクタ。ポインタを消去し、ファイルを閉じる。
	~analysis();
	AnalyzeType runMyClass(param &params);
	void closefile();
	void clear_pointer();
	void init_DataStructure();
};

// ========================================================================================================

#endif