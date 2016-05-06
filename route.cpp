#include "route.h"
#include "lib_record.h"
#include "graph.h"
#include "ant.h"
#include "TSP.h"

#include <stdio.h>
#include <time.h>
#include <algorithm>

int N_CITY_COUNT; //城市数量
clock_t start_time;

void demanNoTuba(std::vector<EdgeList> adj_vec, Demand & demand, std::vector<int> &TubaVec, int num_node)
{
    vector<int> deman_vec = demand.pass;
    while (1) {
        int count = 0;
        for (int i = 0; i < deman_vec.size(); i++) {
            if (adj_vec[deman_vec[i]].size() == 1) {
                count++;
                EdgeList::iterator adj_it = adj_vec[deman_vec[i]].begin();
                int NodeNo = adj_it->to;
                TubaVec.push_back(deman_vec[i]);
                TubaVec.push_back(NodeNo);
                deman_vec[i] = NodeNo;
            }
        }
        if (count == 0)
            return;
    }
}

void pre_process(std::vector<EdgeList> *adj_vec, int num_node, std::vector<int> TubaVec)
{
    for (int i = 0; i < TubaVec.size(); i += 2) {
        int OutNo = TubaVec[i];//必经节点
        int InNo = TubaVec[i + 1];//去掉该点的其他入度

        for (int j = 0; j < num_node; j++) {
            if (j == OutNo)
                continue;

            EdgeList::iterator adj_it = (*adj_vec)[j].begin();
            while (adj_it != (*adj_vec)[j].end()) {
                if (adj_it->to == InNo) {
                    adj_it = (*adj_vec)[j].erase(adj_it);
                }
                else {
                    adj_it++;
                }
            }
        }
    }
}


Path NodenoIndexPath(std::vector<EdgeList> adj_vec, CAnt m_cBestAnt)
{
    Path best_path;
    for (int i = 1; i < m_cBestAnt.m_nMovedCityCount; i++) {
        EdgeList::iterator adj_it;
        int m = m_cBestAnt.m_nPath[i];
        int n = m_cBestAnt.m_nPath[i - 1];
        best_path.cost = m_cBestAnt.m_dbPathLength;
        for (adj_it = adj_vec[n].begin(); adj_it != adj_vec[n].end(); adj_it++) {
            if (m == adj_it->to) {
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
//    unsigned short result1[] = {0, 1, 2};//P'路径
//    unsigned short result2[] = {5, 6, 2};//P''路径
//
//    for (int i = 0; i < 3; i++)
//    {
//        record_result(WORK_PATH, result1[i]);
//        record_result(BACK_PATH, result2[i]);
//    }


    start_time = clock();

    /*----------建立邻接链表------*/
    int num_node = 0;
    std::vector<EdgeList> adj_vec = read_graph(topo, edge_num);

    /*----------分析需求点集------*/
    std::vector<Demand> deman_vec = read_demand(demand, demand_num);

    num_node = adj_vec.size();

    /*--------------预处理---------*/
//
//    std::vector<int> TubaVec;
//    demanNoTuba(adj_vec, deman_vec[0], TubaVec, num_node);
//    pre_process(&adj_vec, num_node, TubaVec);


    /*************** NA ******************/
//    for (int i = 0; i < num_node; i++)
//    {
//        if (adj_vec[i].size() == 0)
//        {
//            std::vector<Demand>::iterator it = find(deman_vec.begin(), deman_vec.end(), i);
//            if (it != deman_vec.end())
//            {
//                printf("NA\n");
//                return;
//            }
//        }
//    }

    Path best_path[2];

    for(int index = 0; index < demand_num; index++)
    {
        /*方式二-----------蚁群寻路---------*/
        double **g_Trial; //两两城市间信息素，就是环境信息素
        g_Trial = (double **) malloc(sizeof(double *) * num_node);
        for (int i = 0; i < num_node; i++)
            g_Trial[i] = (double *) malloc(sizeof(double) * num_node);

        double **g_Distance; //两两城市间距离
        g_Distance = (double **) malloc(sizeof(double *) * num_node);
        for (int i = 0; i < num_node; i++)
            g_Distance[i] = (double *) malloc(sizeof(double) * num_node);


        CTsp tsp;
        tsp.InitData(num_node, adj_vec, g_Distance, g_Trial); //初始化
        tsp.Search(deman_vec[index], adj_vec, g_Distance, g_Trial, index); //开始搜索

        best_path[index] = NodenoIndexPath(adj_vec, tsp.m_cBestAnt);
    }

    for(int i = 0 ; i < best_path[0].edgeID.size(); i++)
        record_result(WORK_PATH, (unsigned short)best_path[0].edgeID[i]);
    for(int i = 0 ; i < best_path[1].edgeID.size(); i++)
        record_result(BACK_PATH, (unsigned short)best_path[1].edgeID[i]);
}
