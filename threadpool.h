#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <limits.h>
#include "list.h"

/**
 * 定义的回调函数
*/
typedef void (*task_func_t)(void *args);

/**
 * 定义的任务节点结构体
*/
typedef struct task_t
{
    void             *args;
    task_func_t      func;
    struct list_head node;
}task_t;

/**
 * 线程池信息
*/
typedef struct threadpool_t
{
    struct list_head hlist;       //任务链表
    int              thread_num;  //线程池数量
    int              max_ts_num;  //最大任务数量
    volatile int     curr_ts_num; //当前线程池存在的任务数
    volatile int     is_exit;     //是否退出线程池标志
    pthread_mutex_t  mutex;       //互斥锁
    pthread_cond_t   cond;        //条件变量
    pthread_t        *ths;        //线程id数组
}threadpool_t;

/**
 * @brief:创建一个线程池
 * @thread_nums: 线程池中的线程数量
 * @max_ts_num: 线程池中最大任务数量
 * @return: 返回一个线程池句柄
*/
threadpool_t* create_threadpool(int thread_nums, int max_ts_num);

/**
 * @brief:获取线程池中当前存在的任务数量
 * @tp: 线程池句柄
 * @return: 当前任务数量
*/
int get_ts_num_threadpool(threadpool_t* tp);

/**
 * @brief:往线程池中添加任务
 * @tp: 线程池句柄
 * @func:任务处理函数指针
 * @args:传入任务参数
 * @priority: 优先级 1:优先处理 其他:添加到尾部
 * @return: 返回状态 0:ok
*/
int add_task_threadpool(threadpool_t* tp, task_func_t func, void *args, int priority);

/**
 * @brief:释放线程池资源
 * @tp:线程池句柄
 * @return: 0:ok
*/
int destory_threadpool(threadpool_t* tp);


#endif // !__THREADPOOL_H__
