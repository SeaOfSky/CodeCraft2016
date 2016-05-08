//
// Created by duanxu on 16-5-5.
//

#include "ant.h"
#include <algorithm>

extern int N_NODE_COUNT; //节点数量

/*
 * 功能： 指定范围内的随机浮点数
 * 参数： dbLow： 随机数下限
 *       dbUpper： 随机数上限
 * 返回： 包括下限但不包括上限的随机数
 */
inline double rnd(double dbLow, double dbUpper)
{
    double dbTemp = rand() / ((double) RAND_MAX + 1.0);
    return dbLow + dbTemp * (dbUpper - dbLow);
}

/*
 * 功能： 构造函数
 *
 */
Ant::Ant()
{
}

/*
 * 功能： 析构函数
 *
 */
Ant::~Ant(void)
{
//    free(m_nPath);
//    free(m_nAllowedNode);
//    free(m_nDemanNode);
}

/*
 * 功能： 蚂蚁初始化函数
 * 参数： demand：  必经节点信息
 *       adj_vec： 图的邻接表
 * 返回： 空
 */
void Ant::Init(Demand &demand, std::vector<EdgeList> adj_vec)
{
    std::vector<int> deman_vec = demand.pass;

    m_nPath        = (int *) malloc(sizeof(int) * N_NODE_COUNT);
    m_nAllowedNode = (int *) malloc(sizeof(int) * N_NODE_COUNT);
    m_nDemanNode   = (int *) malloc(sizeof(int) * N_NODE_COUNT);

    start = demand.start;
    end = demand.end;

    for (int i = 0; i < N_NODE_COUNT; i++)
    {
        m_nAllowedNode[i] = 1;                      // 设置全部节点为没有去过
        m_nPath[i] = 0;                             // 蚂蚁走的路径全部设置为0
    }

    for (int i = 0; i < N_NODE_COUNT; i++)
    {
        if ((adj_vec[i].size() == 0) && (i != demand.end))
            m_nAllowedNode[i] = 0;                  // 设置陷阱为已经去过
    }

    for (int i = 0; i < N_NODE_COUNT; i++)
        m_nDemanNode[i] = 0;                        // 设置全部节点为非必须经过节点

    for (int i = 0; i < deman_vec.size(); i++)
        m_nDemanNode[deman_vec[i]] = 1;             // 设置为必须经过节点


    m_dbPathLength = 0.0;                           // 蚂蚁走过的路径长度设置为0
    //m_nCurNodeNo = rnd(0, N_NODE_COUNT);          // 随机选择一个出发节点
    m_nCurNodeNo = demand.start;                    // 每只蚂蚁从起点出发
    m_nPath[0] = m_nCurNodeNo;                      // 把出发节点保存入路径数组中
    m_nAllowedNode[m_nCurNodeNo] = 0;               // 标识出发节点为已经去过了
    m_nMovedNodeCount = 1;                          // 已经去过的节点数量设置为1

}

/*
 * 功能： 选择下一个节点
 * 参数： g_Distance：       两两节点间的距离
 *       g_Trial：          两两节点间的信息素
 *       deman_count：      当前剩余必经节点数
 *       deman_node_count： 必经节点数
 * 返回： 下一个节点编号
 */
int Ant::ChooseNextNode(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count)
{

    int nSelectedNode = -1;                                                         // 返回节点编号

    /*********     计算当前节点和没去过的节点之间的信息素总和   *************/
    double dbTotal = 0.0;
    double *prob = (double *) malloc(sizeof(double) * N_NODE_COUNT);                // 保存各个节点被选中的概率

    double Total = 0.0;

    for (int i = 0; i < N_NODE_COUNT; i++)
    {
        if ((m_nAllowedNode[i] == 1) && (g_Distance[m_nCurNodeNo][i] != DB_MAX))    // 可去的并且没有去过的节点
        {
            Total += pow(g_Trial[m_nCurNodeNo][i], ALPHA) * pow(1.0 / g_Distance[m_nCurNodeNo][i], BETA);
        }
    }

    for (int i = 0; i < N_NODE_COUNT; i++)
    {
        if ((m_nAllowedNode[i] == 1) && (g_Distance[m_nCurNodeNo][i] != DB_MAX))    // 可去的并且没有去过的节点
        {
            if (m_nDemanNode[i] == 1)                                       // 必经节点选择概率为 0.8
            {
                prob[i] = 0.8;
            }
            else                                                            // 非必经节点按照公式计算
            {
                prob[i] = pow(g_Trial[m_nCurNodeNo][i], ALPHA) *
                          pow(1.0 / g_Distance[m_nCurNodeNo][i], BETA);     // 该节点和当前节点间的信息素
            }


            if ((deman_count != deman_node_count) && (i == end))            // 若需要经过的点还没走完且选择的新节点是终点，则该点不走
                prob[i] = 0.0;

            dbTotal = dbTotal + prob[i];                                    // 累加信息素，得到总和
        }
        else                                                                // 如果节点去过了，则其被选中的概率值为0
        {
            prob[i] = 0.0;
        }
    }

    /**********************   进行轮盘选择   ******************************/
    double dbTemp = 0.0;
    if (dbTotal > 0.0)                                  // 总的信息素值大于0
    {
        dbTemp = rnd(0.0, dbTotal);                     // 取一个随机数

        for (int i = 0; i < N_NODE_COUNT; i++)
        {
                                                        // 可去的并且没有去过的节点
            if ((m_nAllowedNode[i] == 1) && (g_Distance[m_nCurNodeNo][i] != DB_MAX))
            {
                dbTemp = dbTemp - prob[i];              // 这个操作相当于转动轮盘，如果对轮盘选择不熟悉，仔细考虑一下
                if (dbTemp < 0.0)                       // 轮盘停止转动，记下节点编号，直接跳出循环
                {
                    nSelectedNode = i;
                    break;
                }
            }
        }
    }

    free(prob);

    /********************************************************/
    // 如果节点间的信息素非常小 ( 小到比double能够表示的最小的数字还要小 )
    // 那么由于浮点运算的误差原因，上面计算的概率总和可能为0
    // 会出现经过上述操作，没有节点被选择出来
    // 出现这种情况，就把第一个没去过的节点作为返回结果

    if (nSelectedNode == -1)
    {
        for (int i = 0; i < N_NODE_COUNT; i++)
        {
            if ((m_nAllowedNode[i] == 1) && (g_Distance[m_nCurNodeNo][i] != DB_MAX)) //可去的并且没有去过的节点
            {
                nSelectedNode = i;
                break;
            }
        }
    }

    return nSelectedNode;
}

/*
 * 功能： 蚂蚁在节点间移动
 * 参数： g_Distance：       两两节点间的距离
 *       g_Trial：          两两节点间的信息素
 *       deman_count：      当前剩余必经节点数
 *       deman_node_count： 必经节点数
 * 返回： 是否移动成功
 */
bool Ant::Move(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count)
{
    int nNodeNo = ChooseNextNode(g_Distance, g_Trial, deman_count, deman_node_count); // 选择下一个节点

    if (nNodeNo == -1) return false;            // 如果节点号为 -1 直接返回，蚂蚁无路可走

    m_nPath[m_nMovedNodeCount] = nNodeNo;   // 保存蚂蚁走的路径
    m_nAllowedNode[nNodeNo] = 0;            // 把这个节点设置成已经去过了
    m_nCurNodeNo = nNodeNo;                 // 改变当前所在节点为选择的节点
    m_nMovedNodeCount++;                    // 已经去过的节点数量加1

    return true;
}

/*
 * 功能： 蚂蚁进行搜索一次
 * 参数： demand：           必经节点信息
 *       adj_vec：          图信息
 *       g_Distance：       两两节点间的距离
 *       g_Trial：          两两节点间的信息素
 * 返回： 空
 */
void Ant::Search(Demand &demand, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial)
{
    Init(demand, adj_vec);                          // 蚂蚁搜索前，先初始化

    std::vector<int> deman_vec = demand.pass;

    //如果蚂蚁去过的节点未经过所有必经节点，且当前节点不是最终节点，就继续移动
    int deman_node_count = (int) deman_vec.size();
    int deman_count = 0;
    while ((m_nCurNodeNo != end) || (deman_count != deman_node_count))
    {
        bool flag = Move(g_Distance, g_Trial, deman_count, deman_node_count);
        if (!flag)
        {
            std::vector<int>::iterator pre_it = find(deman_vec.begin(), deman_vec.end(), m_nCurNodeNo);
            if (pre_it != deman_vec.end())
            {
                m_dbPathLength = DB_MAX;

                //输出路径
                //printf("%d\n", m_nCurNodeNo);
                //for (int i = 0; i < m_nMovedNodeCount; i++)
                //	printf("%d->", m_nPath[i]);
                //printf("\n\n");

                return;
            }
            m_nCurNodeNo = m_nPath[m_nMovedNodeCount - 2];
            m_nPath[m_nMovedNodeCount - 1] = 0;
            m_nMovedNodeCount--;
            std::vector<int>::iterator it = find(deman_vec.begin(), deman_vec.end(), m_nCurNodeNo);
            if (it != deman_vec.end())
            {
                deman_count--;
            }
            continue;
        }

        std::vector<int>::iterator it = find(deman_vec.begin(), deman_vec.end(), m_nCurNodeNo);
        if (it != deman_vec.end())
        {
            deman_count++;
        }
        //if ((deman_count != demaN_NODE_COUNT) && (m_nCurNodeNo == en))
        //{
        //	m_dbPathLength = DB_MAX;
        //	return;
        //}
    }

    //完成搜索后计算走过的路径长度
    CalPathLength(g_Distance);
}


/*
 * 功能： 计算蚂蚁走过的路径长度
 * 参数： g_Distance： 两两节点间的距离
 * 返回： 路径长度
 */
void Ant::CalPathLength(double **g_Distance)
{

    m_dbPathLength = 0.0; //先把路径长度置0
    int m = 0;
    int n = 0;

    for (int i = 1; i < m_nMovedNodeCount; i++)
    {
        m = m_nPath[i];
        n = m_nPath[i - 1];
        m_dbPathLength = m_dbPathLength + g_Distance[n][m];
    }
}