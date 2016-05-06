//
// Created by duanxu on 16-5-5.
//

#include "TSP.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

extern int N_CITY_COUNT; //城市数量
extern clock_t start_time;

//构造函数
CTsp::CTsp(void)
{
}

CTsp::~CTsp(void)
{
}


//初始化数据
void CTsp::InitData(int num_node, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial)
{
    //先把最优蚂蚁的路径长度设置成一个很大的值
    m_cBestAnt.m_dbPathLength = DB_MAX;
    N_CITY_COUNT = num_node;
    //计算两两城市间距离
    double dbTemp = 0.0;
    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        for (int j = 0; j<N_CITY_COUNT; j++)
        {
            g_Distance[i][j] = DB_MAX;
        }
    }
    EdgeList::iterator adj_it;
    for (int i = 0; i < num_node; i++)
    {
        for (adj_it = adj_vec[i].begin(); adj_it != adj_vec[i].end(); adj_it++)
        {
            g_Distance[i][adj_it->to] = adj_it->cost;
        }
    }

    //初始化环境信息素，先把城市间的信息素设置成一样
    //这里设置成1.0，设置成多少对结果影响不是太大，对算法收敛速度有些影响
    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        for (int j = 0; j<N_CITY_COUNT; j++)
        {
            g_Trial[i][j] = 1.0;
        }
    }

}

//更新环境信息素
void CTsp::UpdateTrial(double **g_Trial)
{
    //临时数组，保存各只蚂蚁在两两城市间新留下的信息素
    //double dbTempAry[node_num][node_num];
    double **dbTempAry = (double**)malloc(sizeof(double*)*N_CITY_COUNT);
    for (int i = 0; i<N_CITY_COUNT; i++)
        dbTempAry[i] = (double *)malloc(sizeof(double) * N_CITY_COUNT);
    //memset(dbTempAry, 0, sizeof(dbTempAry)); //先全部设置为0
    for (int i = 0; i < N_CITY_COUNT; i++)
        for (int j = 0; j < N_CITY_COUNT; j++)
            dbTempAry[i][j] = 0;

    //计算新增加的信息素,保存到临时数组里
    int m = 0;
    int n = 0;
    for (int i = 0; i<N_ANT_COUNT; i++) //计算每只蚂蚁留下的信息素
    {
        for (int j = 1; j<m_cAntAry[i].m_nMovedCityCount; j++)
        {
            m = m_cAntAry[i].m_nPath[j];
            n = m_cAntAry[i].m_nPath[j - 1];
            dbTempAry[n][m] = dbTempAry[n][m] + DBQ / m_cAntAry[i].m_dbPathLength;//城市n->城市m的信息素
        }
    }

    //==================================================================
    //更新环境信息素
    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        for (int j = 0; j<N_CITY_COUNT; j++)
        {
            g_Trial[i][j] = g_Trial[i][j] * ROU + dbTempAry[i][j]; //最新的环境信息素 = 留存的信息素 + 新留下的信息素
        }
    }

    for (int i = 0; i < N_CITY_COUNT; i++)
        free(dbTempAry[i]);
    free(dbTempAry);

}


void CTsp::Search(Demand & demand, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial, int index)
{
    clock_t end_time;

    //在迭代次数内进行循环
    for (int i = 0; i<N_IT_COUNT; i++)
    {
        int dead_ants = 0;//死掉的蚂蚁
        //每只蚂蚁搜索一遍
        for (int j = 0; j<N_ANT_COUNT; j++)
        {
            m_cAntAry[j].Search(demand, adj_vec, g_Distance, g_Trial);
            if (m_cAntAry[j].m_dbPathLength == DB_MAX)
                dead_ants++;
        }

        //保存最佳结果
        for (int j = 0; j<N_ANT_COUNT; j++)
        {
            if (m_cAntAry[j].m_dbPathLength < m_cBestAnt.m_dbPathLength)
            {
                m_cBestAnt = m_cAntAry[j];
            }
        }

        //更新环境信息素
        UpdateTrial(g_Trial);

        end_time = clock();
        if((double)(end_time - start_time)/CLOCKS_PER_SEC > (index + 1) * 5 - 1)
            break;
    }

//    printf("%lf\n", (double)(end_time - start_time)/CLOCKS_PER_SEC);

}
