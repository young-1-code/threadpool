#include "manange_threadpool.h"

/**
 * @brief:创建线程池组管理句柄
 * @tp_nums:线程池组中线程池个数
 * @thread_num:单个线程池中线程个数
 * @max_ts_n:单个线程池中最大的任务数量
*/
manange_thpool_t* create_group_threadpool(int tp_nums, int thread_num, int max_ts_n)
{
    manange_thpool_t* mtp = (manange_thpool_t*)malloc(sizeof(manange_thpool_t));
    if(!mtp) return NULL;
    memset(mtp, 0, sizeof(manange_thpool_t));
    mtp->thpool_nums = tp_nums;

    for(int i=0; i<tp_nums; ++i){
        mtp->thpools[i] = create_threadpool(thread_num, max_ts_n);
    }

    return mtp;
}

/**
 * @brief:往线程池组中添加任务
 * @mtp:线程池组句柄
 * @func:任务函数
 * @args:任务函数的参数
 * @priority: 优先级 1:优先处理 其他:依次处理
 * @return: 0:ok 其他:err
*/
int add_task_group_threadpool(manange_thpool_t* mtp, task_func_t func, void *args, int priority)\
{
    int ts_num= INT_MAX;
    threadpool_t *tp=NULL;
    int index=0;

    for(register int i=0; i<mtp->thpool_nums; ++i){
        if(mtp->thpools[i]->curr_ts_num < ts_num){
            ts_num = mtp->thpools[i]->curr_ts_num;
            tp = mtp->thpools[i];
            index=i;
        }
    }

    if(!tp){
        tp = mtp->thpools[0];
    }
    return add_task_threadpool(tp, func, args, priority);
}

/**
 * @brief:释放线程池组函数
 * @tp: 线程池组句柄
 * @return:none
*/
void destory_group_threadpool(manange_thpool_t* tp)
{
    if(!tp) return;
    
    for(int i=0; i<tp->thpool_nums; ++i){
        if(tp->thpools[i]) destory_threadpool(tp->thpools[i]);
    }
}