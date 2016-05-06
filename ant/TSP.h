//
// Created by duanxu on 16-5-5.
//

#ifndef FUTURE_NET_TSP_H
#define FUTURE_NET_TSP_H

#include "ant.h"

const int N_ANT_COUNT = 20; //蚂蚁数量
const int N_IT_COUNT = 20000; //迭代次数

//tsp类
class CTsp
{
public:
    CTsp(void);
    ~CTsp(void);

public:
    CAnt m_cAntAry[N_ANT_COUNT]; //蚂蚁数组
    CAnt m_cBestAnt; //定义一个蚂蚁变量，用来保存搜索过程中的最优结果
    //该蚂蚁不参与搜索，只是用来保存最优结果

public:

    //初始化数据
    void InitData(int num_node, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial);

    //开始搜索
    void Search(Demand & demand, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial);

    //更新环境信息素
    void UpdateTrial(double **g_Trial);


};


#endif //FUTURE_NET_TSP_H
