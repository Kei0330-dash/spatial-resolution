#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <set>
#include <vector>
#include <iostream>
#include <RtypesCore.h>
//プロトタイプ宣言
class block;

// エイリアスの定義

/// @brief イベントの解析タイプを表す列挙型
enum AnalyzeType {
	/// @brief 単一イベント解析のフラグ
	ANALYZE_ONE_EVENT = 0x4,
	/// @brief 全クラスタ解析時に返る値
	ANALYZE_ALL_CLUSTERS = 0x2,
	/// @brief 解析できなかったときのエラー値
	ANALYZE_ERROR = 0x1,
	/// @brief 何も実行していないときの値
	NO_ACTION = 0x0
};

/// @brief 2次元のADC値を格納した配列
using ADC_DATA = std::vector<std::vector<int>>;
/// @brief 2次元の配列として、閾値を超えたところを'W'、超えていないところを'.'で表現したもの
using THRESHOLD_MAP = std::vector<std::vector<char>>;
/// @brief クラスターの塊の情報を格納したもの
using CLUSTER_DATA = std::vector<block>;
#endif