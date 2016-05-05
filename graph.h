//
// Created by duanxu on 16-5-5.
//

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "./lib/lib_io.h"

#include <vector>
#include <list>

#define MAX_NODE_NUM 5
#define MAX(x,y) ((x)>(y)?(x):(y))

using namespace std;

typedef struct Edge     // 每条边
{
    int id;     // 边的 id
    int from;   // 边的起点
    int to;     // 边的终点
    int cost;   // 边的权重
} Edge;

typedef struct Path
{
    double cost;
    vector<int> nodeID;
    vector<int> edgeID;

    friend bool operator< (Path p1, Path p2)
    {
        return p1.cost > p2.cost;
    }
} Path;

typedef struct Demand
{
    int start, end;
    vector<int> pass;
} Demand;

typedef vector<Edge> EdgeList;

extern vector<EdgeList> read_graph(char *topo[MAX_EDGE_NUM], int edge_num);

extern vector<Demand> read_demand(char *demand[MAX_DEMAND_NUM], int demand_num);

#endif //FUTURE_NET_GRAPH_H
