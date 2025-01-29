#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <set>
#include <vector>
#include <iostream>
#include <RtypesCore.h>
//プロトタイプ宣言
class block;

// エイリアスの定義

/// @brief 2次元のADC値を格納した配列
using ADC_DATA = std::vector<std::vector<UShort_t>>;
/// @brief 2次元の配列として、閾値を超えたところを'W'、超えていないところを'.'で表現したもの
using THRESHOLD_MAP = std::vector<std::vector<char>>;
/// @brief クラスターの塊の情報を格納したもの
using CLUSTER_DATA = std::vector<block>;
#endif