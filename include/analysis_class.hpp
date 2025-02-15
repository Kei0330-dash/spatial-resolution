#ifndef ANALYSISCLASS_HPP
#define ANALYSISCLASS_HPP

#include "alias.hpp"
#include "MyClass.hpp"
#include "block.hpp"
#include "param.hpp"
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
	//ユーザーによって設定された値
	bool opt_Red, opt_subtract, opt_Fitting, opt_AutoCluster;
	double threshold;
	int event_num;
	TString path;
	// ------------------------------
	ADC_DATA weight;
	THRESHOLD_MAP origin_map;
	CLUSTER_DATA cluster;
	/// @brief 深さ優先探索を実行してクラスターの塊を走査する。ただし、直接呼ばずint call_dfs()を経由して呼び出すこと。
	/// @param x ピクセルのx軸
	/// @param y ピクセルのy軸
	/// @param map dfsで既にいったことがあるかを確認する。
	/// @return クラスターの塊としての情報を返す。
	block dfs(int x, int y, THRESHOLD_MAP map);
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

	public:
	analysis(param params);
	AnalyzeType runMyClass(param params);
	void closefile();
};



/// @param weight ADC値
/// @param threshold 閾値
/// @param opt_sub オプションで、ADC値から閾値を引くかどうかを指定する。閾値より小さい場合は0にする。
THRESHOLD_MAP create_map(ADC_DATA &weight, double threshold, bool opt_sub);

/// @param map 閾値を超えた後のmapであり、閾値が超えたところを'W',それ以外を'.'で表現する。
/// @param cluster クラスターの塊の配列で、dfsで走査された結果がこの中に格納される。
/// @param weight クラスターが単体だった場合その重みのデータは削除される。
/// @param opt_sub ペデスタル減算をするかどうかのフラグ。
/// @param event_num
int  call_dfs(THRESHOLD_MAP &map, CLUSTER_DATA &cluster, ADC_DATA &weight, const bool opt_sub, int event_num);

/// @param map クラスタの分布を引数とする
void highlight(TBox* &box, THRESHOLD_MAP map);


/// @param map クラスタの分布を引数とする
void highlight(std::vector<TLine*> &lines, THRESHOLD_MAP map);

/// @brief 
/// @param weight 
/// @param opt_Red 
/// @param opt_sub 
/// @param opt_fit 
void AnalyzeAndVisualizeClusters(ADC_DATA weight, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false);

void Position_Resolution(CLUSTER_DATA data);

/// @brief root -l analysis.cppをターミナルでやって読み込んだ時、まずこの関数を呼び出します。
/// @param event_num eventのエントリー数を選ぶ
/// @param opt_Red クラスターを強調表示するかを選ぶ。
/// @param opt_sub ペデスタルを減算するかを選ぶ。
/// @param opt_fit フィッティングをするかを選ぶ。
/// @param opt_AutoCluster オートでクラスターがあるかどうかを判定します。他の引数に左右されません
/// @param path データのパスを入力します。デフォルトで選ばれている引数を変更して自分の環境に合わせたパスを入力してください。
/// @return どの解析を行ったかを返します。
AnalyzeType runMyClass(Int_t event_num, bool opt_Red = false, bool opt_sub = false, bool opt_fit = false, bool opt_AutoCluster = false, TString path = "/home/otokun241/newRepository/data/SOFIST3_DATA_HV130_chip1_alpha_241009.root");

void closefile();


// ========================================================================================================

#endif