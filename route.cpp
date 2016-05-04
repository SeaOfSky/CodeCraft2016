#include "route.h"
#include "lib_record.h"
#include <stdio.h>

//你要完成的功能总入口
void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
    unsigned short result1[] = {0, 1, 2};//P'路径
    unsigned short result2[] = {5, 6, 2};//P''路径

    for (int i = 0; i < 3; i++)
    {
        record_result(WORK_PATH, result1[i]);
        record_result(BACK_PATH, result2[i]);
    }
}
