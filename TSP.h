//
// Created by duanxu on 16-5-5.
//

#ifndef FUTURE_NET_TSP_H
#define FUTURE_NET_TSP_H

#include "ant.h"

const int N_ANT_COUNT = 20;     // 蚂蚁数量
const int N_IT_COUNT = 5000;    // 迭代次数

//tsp类
class CTsp
{
public:
    CTsp(void);
    ~CTsp(void);

public:
    Ant m_cAntAry[N_ANT_COUNT]; // 蚂蚁数组
    Ant m_cBestAnt;             // 定义一个蚂蚁变量，用来保存搜索过程中的最优结果
                                // 该蚂蚁不参与搜索，只是用来保存最优结果

public:

    void InitData(int num_node, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial);              // 初始化数据
    void Search(Demand & demand, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial, int index);  // 搜索路径
    void UpdateTrial(double **g_Trial);     // 更新信息素
};


#endif //FUTURE_NET_TSP_H
