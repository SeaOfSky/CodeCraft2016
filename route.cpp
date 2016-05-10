#include "route.h"
#include "lib_record.h"
#include "graph.h"
#include "ant.h"
#include "TSP.h"

#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <iostream>

int N_NODE_COUNT;       // 节点总数
clock_t start_time;     // 程序开始时间

std::vector<EdgeList> adj_vec;
std::vector<Demand> deman_vec;
std::vector<Edge> edge_vec;

Path NodenoIndexPath(Ant m_cBestAnt)
{
    Path best_path;
    for (int i = 1; i < m_cBestAnt.m_nMovedNodeCount; i++)
    {
        EdgeList::iterator adj_it;
        int m = m_cBestAnt.m_nPath[i];
        int n = m_cBestAnt.m_nPath[i - 1];
        best_path.cost = m_cBestAnt.m_dbPathLength;
        for (adj_it = adj_vec[n].begin(); adj_it != adj_vec[n].end(); adj_it++)
        {
            if (m == adj_it->to)
            {
                best_path.nodeID.push_back(m);
                best_path.edgeID.push_back(adj_it->id);
            }
        }
    }
    return best_path;
}

Path replacePath(Path & prevPath, Path & currPath, int ** repeat, int num_node)
{
    bool ** select = (bool **)malloc(sizeof(bool*)*num_node);
    for(int i = 0; i < num_node; i++)
        select[i] = (bool *)malloc(sizeof(bool) * num_node);

    for(int i = 0; i < num_node; i++)
    {
        for(int j = 0; j < num_node; j++)
        {
            select[i][j] = false;
        }
    }

    for(int i = 0; i < (int)prevPath.nodeID.size() - 1; i++)
    {
        select[prevPath.nodeID[i]][prevPath.nodeID[i+1]] = true;
    }

    for(int i = 0; i < (int)currPath.nodeID.size() - 1; i++)
    {
        int from = currPath.nodeID[i], to = currPath.nodeID[i+1];
        if(select[from][to] && repeat[from][to] >= 2)
        {
            for(int j = 0; j < (int)adj_vec[i].size(); j++)
                if(adj_vec[i][j].to == to)
                    currPath.edgeID[i] = adj_vec[i][j+1].id;
        }
    }
}

//你要完成的功能总入口
void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
    start_time = clock();                                               // 程序开始执行的时间，为了卡时间

    read_graph(topo, edge_num);         // 读取topo信息到邻接表
    read_demand(demand, demand_num);    // 读取必经节点信息
    std::vector<int> avoid_node;                                        // 需要避开的节点信息

    Path best_path[2];                                                  // 存储最优路径信息
    double time[2];                                                     // 时间片分配
    int num_node = (int)adj_vec.size();                                 // 节点数

    int ** repeat = (int **) malloc(sizeof(int *) * num_node);
    for (int i = 0; i < num_node; i++)
        repeat[i] = (int *) malloc(sizeof(int) * num_node);
    repeat_count(repeat, num_node);

    for(int index = 0; index < demand_num; index++)
    {
        double **g_Trial;                                               // 两两节点间信息素，即环境信息素
        double **g_Distance;                                            // 两两节点间距离
        g_Trial    = (double **) malloc(sizeof(double *) * num_node);
        g_Distance = (double **) malloc(sizeof(double *) * num_node);
        for (int i = 0; i < num_node; i++)
        {
            g_Trial[i]    = (double *) malloc(sizeof(double) * num_node);
            g_Distance[i] = (double *) malloc(sizeof(double) * num_node);
        }

        CTsp tsp;
        tsp.InitData(num_node, g_Distance, g_Trial, avoid_node, repeat); // 初始化
        tsp.TimeSplit(time, 9.5, deman_vec);                                      // 分配时间片
        tsp.Search(deman_vec[index], g_Distance, g_Trial, time[index]);  // 开始搜索
        best_path[index] = NodenoIndexPath(tsp.m_cBestAnt);              // 提取最优路径信息
        avoid_node = best_path[index].nodeID;
        update_cost(best_path[0]);

        for(int i = 0; i < num_node; i++)                                         // 释放资源
        {
            free(g_Trial[i]);
            free(g_Distance[i]);
        }
        free(g_Trial);
        free(g_Distance);
    }

//    replacePath(best_path[0], best_path[1], repeat, num_node);

    for(int i = 0 ; i < (int)best_path[0].edgeID.size(); i++)                     // 将最优路径信息写入文件
        record_result(WORK_PATH, (unsigned short)best_path[0].edgeID[i]);
    for(int i = 0 ; i < (int)best_path[1].edgeID.size(); i++)
        record_result(BACK_PATH, (unsigned short)best_path[1].edgeID[i]);

    std::cout << best_path[0].cost << "\t" << best_path[1].cost << endl;
}
