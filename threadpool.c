#include "threadpool.h"

/**
 * @brief:线程处理任务函数
 * @args: 传入的参数
 * @return: NULL
*/
static void* _process_task_thread(void *args)
{
    threadpool_t* tp = (threadpool_t*)args;
    struct list_head *pos = NULL;
    task_t *task=NULL;

    if(!args) return NULL;

    while(1)
    {
        pthread_mutex_lock(&tp->mutex);
        while(list_empty(&tp->hlist) && !tp->is_exit){
            pthread_cond_wait(&tp->cond, &tp->mutex);
        }
        if(tp->is_exit){        //判断释放退出线程池
            pthread_mutex_unlock(&tp->mutex);
            break;
        }
        pos = tp->hlist.next;   //从任务链表取出头节点
        list_del(pos);          //从链表中删除节点
        --tp->curr_ts_num;      //更新任务数
        pthread_mutex_unlock(&tp->mutex);

        task = list_entry(pos, task_t, node); //从链表节点推出任务节点
        task->func(task->args); //执行任务
        free(task);             //释放任务内存
    }

    return NULL;
}

/**
 * @brief:创建一个线程池
 * @thread_nums: 线程数量
 * @max_ts_num:线程池中最大的任务数量
 * @return: 线程池句柄
*/
threadpool_t* create_threadpool(int thread_nums, int max_ts_num)
{
    if(thread_nums <= 0) return NULL;

    threadpool_t* tp = (threadpool_t*)malloc(sizeof(threadpool_t));
    memset(tp, 0, sizeof(threadpool_t));
    INIT_LIST_HEAD(&tp->hlist);

    tp->is_exit = 0;
    tp->curr_ts_num = 0;
    tp->thread_num = thread_nums;
    tp->max_ts_num = max_ts_num;

    tp->ths = (pthread_t*)malloc(sizeof(pthread_t)*thread_nums);
    pthread_mutex_init(&tp->mutex, NULL);
    pthread_cond_init(&tp->cond, NULL);

    for(int i=0; i<tp->thread_num; ++i){
        pthread_create(&(tp->ths[i]), NULL, _process_task_thread, tp);
    }

    return tp;
}   

/**
 * @brief:往线程池中添加任务
 * @tp: 线程池句柄
 * @func:任务处理函数指针
 * @args:传入任务参数
 * @priority: 优先级 1:优先处理 其他:添加到尾部
 * @return: 返回状态 0:ok
*/
int add_task_threadpool(threadpool_t* tp, task_func_t func, void *args, int priority)
{
    if(!tp) return -1;
    if(!func) return -2;
    if(tp->curr_ts_num > tp->max_ts_num) return -3;

    task_t *task = (task_t*)malloc(sizeof(task_t)); //申请任务节点内存
    task->func = func; //给函数指针赋值
    task->args = args; //保持参数指针

    pthread_mutex_lock(&tp->mutex);
    if(priority==1)    //高优先级,添加到头部
        list_add(&task->node, &tp->hlist);
    else               //添加到尾部
        list_add_tail(&task->node, &tp->hlist);
    ++tp->curr_ts_num; //更新任务数
    pthread_mutex_unlock(&tp->mutex);

    pthread_cond_signal(&tp->cond); //通知线程取任务

    return 0;
}

/**
 * @brief:获取线程池中当前存在的任务数量
 * @tp: 线程池句柄
 * @return: 当前任务数量
*/
int get_ts_num_threadpool(threadpool_t* tp)
{
    return tp ? tp->curr_ts_num : -1;
}

/**
 * @brief:释放线程池资源
 * @tp:线程池句柄
 * @return: 0:ok
*/
int destory_threadpool(threadpool_t* tp)
{
    if(!tp) return -1;
    while(!list_empty(&tp->hlist)){  //等待线程池执行完链表中的任务
        continue;   
    }
    tp->is_exit = 1;                  //更新标志,退出线程池
    pthread_cond_broadcast(&tp->cond);//通知所有线程函数

    for(int i=0; i<tp->thread_num; ++i){//等待所有线程函数结束
        pthread_join(tp->ths[i], NULL);
    }

    pthread_mutex_destroy(&tp->mutex); //释放资源
    pthread_cond_destroy(&tp->cond);
    
    free(tp->ths);

    free(tp);
    tp = NULL;

    return 0;
}