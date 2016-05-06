//
// Created by duanxu on 16-5-5.
//

#ifndef FUTURE_NET_ANT_H
#define FUTURE_NET_ANT_H

#include <vector>
#include "../graph.h"

const double ALPHA = 1.0; //启发因子，信息素的重要程度
const double BETA = 0.6;   //期望因子，城市间距离的重要程度
const double ROU = 0.89; //信息素残留参数

const double DBQ = 50.0; //总的信息素
const double DB_MAX = 10e9; //一个标志数，10的9次方

extern int st, en;//起点和终点

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define ThresholdTime_ms 18000
#define ThresholdNode 101

//定义蚂蚁类
class CAnt
{
public:
    CAnt(void);
    ~CAnt(void);

public:

    //int m_nPath[MaxNode]; //蚂蚁走的路径
    double m_dbPathLength; //蚂蚁走过的路径长度
    //int m_nAllowedCity[MaxNode]; //没去过的城市
    int *m_nDemanCity; //必须去的城市
    int *m_nPath; //蚂蚁走的路径
    int *m_nAllowedCity; //没去过的城市
    int m_nCurCityNo; //当前所在城市编号
    int m_nMovedCityCount; //已经去过的城市数量
    //int CityCount;//城市个数，即节点个数

    int start, end;

public:

    int ChooseNextCity(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count); //选择下一个城市
    void Init(Demand & demand, std::vector<EdgeList> adj_vec); //初始化
    int Move(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count); //蚂蚁在城市间移动
    void Search(Demand & demand, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial); //搜索路径
    void CalPathLength(double **g_Distance); //计算蚂蚁走过的路径长度

};


#endif //FUTURE_NET_ANT_H
