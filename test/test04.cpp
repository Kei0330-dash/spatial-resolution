#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// 平均を計算する関数
double CalculateMean(const std::vector<double>& data)
{
    double sum = 0.0;
    for(double value : data)
    {
        sum += value;
    }
    return sum / data.size();
}

// 分散を計算する関数
double CalculateVariance(const std::vector<double>& data, double mean)
{
    double sum = 0.0;
    for(double value : data)
    {
        sum += (value - mean) * (value - mean);
    }
    return sum / (data.size() - 1);
}

// シャピローウィルク検定の関数
double ShapiroWilkTest(const std::vector<double>& data)
{
    int n = data.size();
    if (n < 3 || n > 5000) {
        std::cerr << "Sample size out of range for Shapiro-Wilk test" << std::endl;
        return 0.0;
    }

    std::vector<double> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());

    // 平均と分散を計算
    double mean = CalculateMean(sorted_data);
    double variance = CalculateVariance(sorted_data, mean);

    // W統計量の計算
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += (sorted_data[i] - mean) * (sorted_data[n - i - 1] - mean);
    }
    double W = sum * sum / (variance * (n - 1) * variance);

    return W;
}

int main()
{
    std::vector<double> data = {0.5, 0.7, 0.8, 1.2, 1.5, 1.8, 2.0, 2.3, 2.5, 2.8};
    double W = ShapiroWilkTest(data);
    std::cout << "Shapiro-Wilk W statistic: " << W << std::endl;

    return 0;
}
