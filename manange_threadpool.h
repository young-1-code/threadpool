#ifndef __MANANGE_THREADPOOL_H__
#define __MANANGE_THREADPOOL_H__

#include "threadpool.h"

#define MAX_THREADPOOL_NUMS (32U) //最大的线程池个数

/**
 * @brief:管理线程池组结构体
*/
typedef struct manange_thpool_t
{
    int thpool_nums;                            //线程池个数
    threadpool_t *thpools[MAX_THREADPOOL_NUMS]; //线程池结构体
}manange_thpool_t;

/**
 * @brief:创建线程池组管理句柄
 * @tp_nums:线程池组中线程池个数
 * @thread_num:单个线程池中线程个数
 * @max_ts_n:单个线程池中最大的任务数量
*/
manange_thpool_t* create_group_threadpool(int tp_nums, int thread_num, int max_ts_n);

/**
 * @brief:往线程池组中添加任务
 * @tp:线程池组句柄
 * @func:任务函数
 * @args:任务函数的参数
 * @priority: 优先级 1:优先处理 其他:依次处理
 * @return: 0:ok 其他:err
*/
int add_task_group_threadpool(manange_thpool_t* tp, task_func_t func, void *args, int priority);

/**
 * @brief:释放线程池组函数
 * @tp: 线程池组句柄
 * @return:none
*/
void destory_group_threadpool(manange_thpool_t* tp);


#endif // !__MANANGE_THREADPOOL_H__
