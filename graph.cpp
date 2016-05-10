//
// Created by duanxu on 16-5-5.
//

#include <stdlib.h>
#include <string.h>
#include "graph.h"

using namespace std;

void read_graph(char * topo[MAX_EDGE_NUM], int edge_num)
{
    vector<EdgeList> graph(MAX_NODE_NUM);
    int max_node = -1;

    for(int i = 0; i < edge_num; i++)
    {
        Edge edge_temp;
        int edge_info[4];

        char * str = strtok(topo[i], ",");
        for(int j = 0; j < 4; j++)
        {
            edge_info[j] = atoi(str);
            str = strtok(NULL, ",");
        }

        edge_temp.id = edge_info[0];
        edge_temp.from = edge_info[1];
        edge_temp.to = edge_info[2];
        edge_temp.cost = edge_info[3];
        edge_vec.push_back(edge_temp);

        max_node = MAX(max_node, edge_temp.from);

        EdgeList::iterator it_list;
        for(it_list = graph[edge_temp.from].begin(); it_list != graph[edge_temp.from].end(); it_list++)
        {
            // 排序按照边的 cost 排序，cost 相同时按照节点编号从小到大排序。
            if(it_list->cost > edge_temp.cost)
            {
                it_list = graph[edge_temp.from].insert(it_list, edge_temp);
                break;
            }
            else if(it_list->cost == edge_temp.cost && it_list->to > edge_temp.to)
            {
                it_list = graph[edge_temp.from].insert(it_list, edge_temp);
                break;
            }
        }
        if(it_list == graph[edge_temp.from].end())
        {
            it_list = graph[edge_temp.from].insert(it_list, edge_temp);
            continue;
        }

    }

    graph.resize((size_t)max_node + 1);
    adj_vec = graph;
}

void read_demand(char * demand[MAX_DEMAND_NUM], int demand_num)
{
    for(int i = 0; i < demand_num; i++)
    {
        Demand temp;

        char * split[4];
        char * str = strtok(demand[i], ",");
        for(int j = 0; j < 4; j++)
        {
            split[j] = str;
            str = strtok(NULL, ",");
        }

        temp.start = atoi(split[1]);
        temp.end = atoi(split[2]);

        str = strtok(split[3], "|");
        while(str)
        {
            temp.pass.push_back(atoi(str));
            str = strtok(NULL, "|");
        }

        deman_vec.push_back(temp);
    }
}

void repeat_count(int ** repeat, int num_node)
{
    for(int i = 0; i < num_node; i++)
    {
        for(int j = 0; j < num_node; j++) repeat[i][j] = 0;

        EdgeList & edgeList = adj_vec[i];
        int curr = -1;
        for(int j = 0; j < (int)edgeList.size(); j++)
        {
            if(edgeList[j].to != curr)
            {
                repeat[i][edgeList[j].to] = 1;
                curr = edgeList[j].to;
            }
            else repeat[i][edgeList[j].to]++;
        }
    }

}

void update_cost(Path & path)
{
    for(int i = 0; i < (int)path.edgeID.size(); i++)
    {
        int from = edge_vec[path.edgeID[i]].from;
        int to = edge_vec[path.edgeID[i]].to;
        EdgeList & list = adj_vec[from];

        int max_cost = 0;
        for(int j = 0; j < (int)list.size(); j++)
            max_cost = MAX(list[j].cost, max_cost);

        for(int j = 0; j < (int)list.size(); j++)
        {
            if(list[j].to == to)
            {
                list[j].cost = max_cost;
                if(j+1 < (int)list.size() && list[j+1].to == to)
                {
                    Edge temp = list[j];
                    list[j] = list[j+1];
                    list[j+1] = temp;
                }
                break;
            }
        }
    }
}