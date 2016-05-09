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

Path NodenoIndexPath(std::vector<EdgeList> adj_vec, Ant m_cBestAnt)
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


//你要完成的功能总入口
void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
    start_time = clock();                                               // 程序开始执行的时间，为了卡时间

    std::vector<EdgeList> adj_vec = read_graph(topo, edge_num);         // 读取topo信息到邻接表
    std::vector<Demand> deman_vec = read_demand(demand, demand_num);    // 读取毕竟节点信息

    Path best_path[2];                                                  // 存储最优路径信息
    int num_node = (int)adj_vec.size();                                 // 节点数

    for(int index = 0; index < demand_num; index++)
    {
        double **g_Trial;                                               // 两两城市间信息素，即环境信息素
        double **g_Distance;                                            // 两两城市间距离
        g_Trial    = (double **) malloc(sizeof(double *) * num_node);
        g_Distance = (double **) malloc(sizeof(double *) * num_node);
        for (int i = 0; i < num_node; i++)
        {
            g_Trial[i]    = (double *) malloc(sizeof(double) * num_node);
            g_Distance[i] = (double *) malloc(sizeof(double) * num_node);
        }


        CTsp tsp;
        tsp.InitData(num_node, adj_vec, g_Distance, g_Trial);               // 初始化
        tsp.Search(deman_vec[index], adj_vec, g_Distance, g_Trial, index);  // 开始搜索
        best_path[index] = NodenoIndexPath(adj_vec, tsp.m_cBestAnt);        // 提取最优路径信息


        for(int i = 0; i < num_node; i++)                                   // 释放资源
        {
            free(g_Trial[i]);
            free(g_Distance[i]);
        }
        free(g_Trial);
        free(g_Distance);
    }

    for(int i = 0 ; i < (int)best_path[0].edgeID.size(); i++)               // 将最优路径信息写入文件
        record_result(WORK_PATH, (unsigned short)best_path[0].edgeID[i]);
    for(int i = 0 ; i < (int)best_path[1].edgeID.size(); i++)
        record_result(BACK_PATH, (unsigned short)best_path[1].edgeID[i]);

    std::cout << best_path[0].cost << "\t" << best_path[1].cost << endl;
}
