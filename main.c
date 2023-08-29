#include <stdio.h>
#include <unistd.h>
#include "list.h"
#include "threadpool.h"
#include "manange_threadpool.h"

//任务传递的参数
typedef struct info_t
{
    int times;
    char buffer[32];
}info_t;


void task1(void *args)
{
    info_t *info = (info_t*)args;
    printf("handle task1 pid=%lld times=%d buffer=%s\n", pthread_self(), info->times, info->buffer);
    free(args);
}

void task2(void *args)
{
    info_t *info = (info_t*)args;
    printf("handle task2 pid=%lld times=%d buffer=%s\n", pthread_self(), info->times, info->buffer);
    free(args);
}

void task3(void *args)
{
    info_t *info = (info_t*)args;
    printf("handle task3 pid=%lld times=%d buffer=%s\n", pthread_self(), info->times, info->buffer);
    free(args);
}

//------------split-----------------

void test_threadpool(void)
{
    threadpool_t* tp = create_threadpool(4, 128);

    info_t *info;

    for(int t=0; t<10; ++t){
        for(int i=0; i<32; ++i){
            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test ThreadPool task1 info...");
            add_task_threadpool(tp, task1, info, 1); //往线程池组添加任务

            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test ThreadPool task2 info...");
            add_task_threadpool(tp, task2, info, 0);

            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test ThreadPool task3 info...");
            add_task_threadpool(tp, task3, info, 0);
        }
        sleep(1);
    }

    destory_threadpool(tp);
    printf("Test ThreadPool Finish...\n");
}

void test_manange_threadpool(void)
{
    //创建线程池组句柄，有4个线程池，每个线程池使用4线程，每个线程池最大的任务数是32
    manange_thpool_t* mtp = create_group_threadpool(4, 4, 128);
    info_t *info;

    for(int t=0; t<10; ++t){
        for(int i=0; i<32; ++i){
            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test task1 info...");
            add_task_group_threadpool(mtp, task1, info, 1); //往线程池组添加任务

            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test task2 info...");
            add_task_group_threadpool(mtp, task2, info, 0);

            info = (info_t *)malloc(sizeof(info_t));
            info->times=i;
            sprintf(info->buffer, "Test task3 info...");
            add_task_group_threadpool(mtp, task3, info, 0);
        }
        sleep(1);
    }

    //释放线程池组资源
    destory_group_threadpool(mtp);
    printf("Test Manage ThreadPool Finish...\n");
}

int main(void)
{

#if 0 //测试单个的线程池功能
    test_threadpool();
#else //测试线程池组功能
    test_manange_threadpool();
#endif

    return 0;
}