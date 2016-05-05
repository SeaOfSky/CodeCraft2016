//
// Created by duanxu on 16-5-5.
//

#include "ant.h"
#include <algorithm>

//返回指定范围内的随机整数
int rnd(int nLow, int nUpper)
{
    return nLow + (nUpper - nLow)*rand() / (RAND_MAX + 1);
}

//返回指定范围内的随机浮点数
double rnd(double dbLow, double dbUpper)
{
    double dbTemp = rand() / ((double)RAND_MAX + 1.0);
    return dbLow + dbTemp*(dbUpper - dbLow);
}

//构造函数
CAnt::CAnt()
{
}

//析构函数
CAnt::~CAnt(void)
{
    //free(m_nPath);
    //free(m_nAllowedCity);
}

//初始化函数，蚂蚁搜索前调用
void CAnt::Init(std::vector<int> deman_vec, std::vector<EdgeList> adj_vec)
{
    m_nPath = (int*)malloc(sizeof(int)*N_CITY_COUNT);
    m_nAllowedCity = (int*)malloc(sizeof(int)*N_CITY_COUNT);
    m_nDemanCity = (int*)malloc(sizeof(int)*N_CITY_COUNT);

    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        m_nAllowedCity[i] = 1; //设置全部城市为没有去过
        m_nPath[i] = 0; //蚂蚁走的路径全部设置为0
    }

    for (int i = 0; i < N_CITY_COUNT; i++)
    {
        if((adj_vec[i].size()==0)&&(i!=en))
            m_nAllowedCity[i] = 0;//设置陷阱为已经去过
    }

    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        m_nDemanCity[i] = 0; //设置全部城市为非必须经过城市
    }
    for (int i = 0; i < deman_vec.size(); i++)
    {
        m_nDemanCity[deman_vec[i]] = 1; //设置为必须经过城市
    }

    //蚂蚁走过的路径长度设置为0
    m_dbPathLength = 0.0;

    //随机选择一个出发城市
    //m_nCurCityNo = rnd(0, N_CITY_COUNT);
    m_nCurCityNo = st;

    //把出发城市保存入路径数组中
    m_nPath[0] = m_nCurCityNo;

    //标识出发城市为已经去过了
    m_nAllowedCity[m_nCurCityNo] = 0;

    //已经去过的城市数量设置为1
    m_nMovedCityCount = 1;

}

//选择下一个城市
//返回值 为城市编号
int CAnt::ChooseNextCity(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count)
{

    int nSelectedCity = -1; //返回结果，先暂时把其设置为-1

    //==============================================================================
    //计算当前城市和没去过的城市之间的信息素总和

    double dbTotal = 0.0;
    //double prob[MaxNode]; //保存各个城市被选中的概率
    double *prob = (double*)malloc(sizeof(double)*N_CITY_COUNT);

    double Total = 0.0;

    for (int i = 0; i < N_CITY_COUNT; i++)
    {
        if ((m_nAllowedCity[i] == 1) && (g_Distance[m_nCurCityNo][i] != DB_MAX)) //可去的并且没有去过的城市
        {
            Total += pow(g_Trial[m_nCurCityNo][i], ALPHA)*pow(1.0 / g_Distance[m_nCurCityNo][i], BETA);
        }
    }

    for (int i = 0; i<N_CITY_COUNT; i++)
    {
        if ((m_nAllowedCity[i] == 1) && (g_Distance[m_nCurCityNo][i] != DB_MAX)) //可去的并且没有去过的城市
        {
            if (m_nDemanCity[i] == 1)//若为必经城市，则直接选择该城市走
            {
                prob[i] = 0.8;
            }

            else
            {
                prob[i] = pow(g_Trial[m_nCurCityNo][i], ALPHA)*pow(1.0 / g_Distance[m_nCurCityNo][i], BETA); //该城市和当前城市间的信息素
                //prob[i] = prob[i] / Total;
            }


            if ((deman_count != deman_node_count) && (i == en))//若需要经过的点还没走完且选择的新节点是终点，则该点不走
                prob[i] = 0.0;

            dbTotal = dbTotal + prob[i]; //累加信息素，得到总和
        }
        else //如果城市去过了，则其被选中的概率值为0
        {
            prob[i] = 0.0;
        }
    }
    //==============================================================================
    //进行轮盘选择
    double dbTemp = 0.0;
    if (dbTotal > 0.0) //总的信息素值大于0
    {
        dbTemp = rnd(0.0, dbTotal); //取一个随机数

        for (int i = 0; i<N_CITY_COUNT; i++)
        {
            if ((m_nAllowedCity[i] == 1) && (g_Distance[m_nCurCityNo][i] != DB_MAX)) //可去的并且没有去过的城市
            {
                dbTemp = dbTemp - prob[i]; //这个操作相当于转动轮盘，如果对轮盘选择不熟悉，仔细考虑一下
                if (dbTemp < 0.0) //轮盘停止转动，记下城市编号，直接跳出循环
                {
                    nSelectedCity = i;
                    break;
                }
            }
        }
    }

    free(prob);
    //==============================================================================
    //如果城市间的信息素非常小 ( 小到比double能够表示的最小的数字还要小 )
    //那么由于浮点运算的误差原因，上面计算的概率总和可能为0
    //会出现经过上述操作，没有城市被选择出来
    //出现这种情况，就把第一个没去过的城市作为返回结果

    //题外话：刚开始看的时候，下面这段代码困惑了我很长时间，想不通为何要有这段代码，后来才搞清楚。
    if (nSelectedCity == -1)
    {
        for (int i = 0; i<N_CITY_COUNT; i++)
        {
            if ((m_nAllowedCity[i] == 1) && (g_Distance[m_nCurCityNo][i] != DB_MAX)) //可去的并且没有去过的城市
            {
                nSelectedCity = i;
                break;
            }
        }
    }

    //==============================================================================

    //返回结果，就是城市的编号
    return nSelectedCity;
}


//蚂蚁在城市间移动
int CAnt::Move(double **g_Distance, double **g_Trial, int deman_count, int deman_node_count)
{
    int nCityNo = ChooseNextCity(g_Distance, g_Trial, deman_count, deman_node_count); //选择下一个城市

    if (nCityNo == -1)
    {
        return 0;
    }

    m_nPath[m_nMovedCityCount] = nCityNo; //保存蚂蚁走的路径
    m_nAllowedCity[nCityNo] = 0;//把这个城市设置成已经去过了
    m_nCurCityNo = nCityNo; //改变当前所在城市为选择的城市
    m_nMovedCityCount++; //已经去过的城市数量加1
    return 1;
}

//蚂蚁进行搜索一次
void CAnt::Search(std::vector<int> deman_vec, std::vector<EdgeList> adj_vec, double **g_Distance, double **g_Trial)
{
    Init(deman_vec, adj_vec); //蚂蚁搜索前，先初始化

    //如果蚂蚁去过的城市未经过所有必经城市，且当前城市不是最终城市，就继续移动
    int deman_node_count = deman_vec.size();
    int deman_count = 0;
    while ((m_nCurCityNo != en) || (deman_count != deman_node_count))
    {
        int rt = Move(g_Distance, g_Trial, deman_count, deman_node_count);
        if (rt == 0)
        {
            std::vector<int>::iterator pre_it = find(deman_vec.begin(), deman_vec.end(), m_nCurCityNo);
            if (pre_it != deman_vec.end())
            {
                m_dbPathLength = DB_MAX;

                //输出路径
                //printf("%d\n", m_nCurCityNo);
                //for (int i = 0; i < m_nMovedCityCount; i++)
                //	printf("%d->", m_nPath[i]);
                //printf("\n\n");

                return;
            }
            m_nCurCityNo = m_nPath[m_nMovedCityCount - 2];
            m_nPath[m_nMovedCityCount-1] = 0;
            m_nMovedCityCount--;
            std::vector<int>::iterator it = find(deman_vec.begin(), deman_vec.end(), m_nCurCityNo);
            if (it != deman_vec.end())
            {
                deman_count--;
            }
            continue;

            //m_dbPathLength = DB_MAX;
            //return;
        }

        std::vector<int>::iterator it = find(deman_vec.begin(), deman_vec.end(), m_nCurCityNo);
        if (it != deman_vec.end())
        {
            deman_count++;
        }
        //if ((deman_count != deman_node_count) && (m_nCurCityNo == en))
        //{
        //	m_dbPathLength = DB_MAX;
        //	return;
        //}
    }

    //完成搜索后计算走过的路径长度
    CalPathLength(g_Distance);
}


//计算蚂蚁走过的路径长度
void CAnt::CalPathLength(double **g_Distance)
{

    m_dbPathLength = 0.0; //先把路径长度置0
    int m = 0;
    int n = 0;

    for (int i = 1; i<m_nMovedCityCount; i++)
    {
        m = m_nPath[i];
        n = m_nPath[i - 1];
        m_dbPathLength = m_dbPathLength + g_Distance[n][m];
    }
}