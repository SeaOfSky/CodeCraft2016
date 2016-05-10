//
// Created by duanxu on 16-5-5.
//

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "./lib/lib_io.h"

#include <vector>
#include <list>

#define MAX_NODE_NUM 2000               // 最大节点数量
#define MAX(x,y) ((x)>(y)?(x):(y))      // 求较大值
#define MIN(x,y) ((x)<(y)?(x):(y))      // 求较小值

using namespace std;

typedef struct Edge         // 每条边
{
    int id;                 // 边的 id
    int from;               // 边的起点
    int to;                 // 边的终点
    int cost;               // 边的权重
} Edge;

typedef struct Path         // 路径信息
{
    double cost;            // 路径长度
    vector<int> nodeID;     // 路径经过节点信息
    vector<int> edgeID;     // 路径经过边的信息

    friend bool operator< (Path p1, Path p2)
    {
        return p1.cost > p2.cost;
    }
} Path;

typedef struct Demand       // 必经节点信息
{
    int start, end;         // 起点和终点
    vector<int> pass;       // 必经节点
} Demand;

typedef vector<Edge> EdgeList;

extern std::vector<EdgeList> adj_vec;
extern std::vector<Demand> deman_vec;
extern std::vector<Edge> edge_vec;

extern void read_graph(char *topo[MAX_EDGE_NUM], int edge_num);

extern void read_demand(char *demand[MAX_DEMAND_NUM], int demand_num);

extern void repeat_count(int ** repeat, int num_node);

extern void update_cost(Path & path);

#endif //FUTURE_NET_GRAPH_H
