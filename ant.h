//
// Created by duanxu on 16-5-5.
//

#ifndef FUTURE_NET_ANT_H
#define FUTURE_NET_ANT_H

#include <vector>
#include "graph.h"

const double ALPHA = 1.0;   // 启发因子，信息素的重要程度
const double BETA = 0.6;    // 期望因子，节点间距离的重要程度
const double ROU = 0.89;    // 信息素残留参数

const double DBQ = 50.0;    //总的信息素
const double DB_MAX = 10e9; //一个标志数，10的9次方

extern std::vector<EdgeList> adj_vec;
extern std::vector<Demand> deman_vec;
extern std::vector<Edge> edge_vec;
extern int ** repeat;
extern bool ** visitable;

//定义蚂蚁类
class Ant
{
public:
    Ant(void);
    ~Ant(void);

public:

    double m_dbPathLength;   // 蚂蚁走过的路径长度
    int * m_nDemanNode;      // 必须去的城市
    int * m_nPath;           // 蚂蚁走的路径
    int * m_nAllowedNode;    // 没去过的节点
    int m_nCurNodeNo;        // 当前所在节点编号
    int m_nMovedNodeCount;   // 已经去过的节点数量
    int start;               // 蚂蚁出发节点号
    int end;                 // 蚂蚁目的节点号

public:

    void Init(Demand & demand);                                          // 初始化
    int  ChooseNextNode(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count);  // 选择下一个节点
    bool Move(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count);            // 蚂蚁在节点间移动
    void Search(Demand & demand, double **g_Distance, double **g_Trial); // 搜索路径
    void CalPathLength(double **g_Distance);                                                            // 计算蚂蚁走过的路径长度
};


#endif //FUTURE_NET_ANT_H
