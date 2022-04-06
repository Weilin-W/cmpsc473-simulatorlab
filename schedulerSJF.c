#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// SJF scheduler info

//Job compare function:
int compare_Job(void* data1, void* data2){
    if(jobGetJobTime((job_t*)data1) > jobGetJobTime((job_t*)data2)){
        return 1;
    }else if(jobGetJobTime((job_t*)data1) == jobGetJobTime((job_t*)data2)){
        if(jobGetId((job_t*)data1) > jobGetId((job_t*)data2)){
            return 1;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}

typedef struct {
    /* IMPLEMENT THIS */
    job_t* job;
    list_t* queue;
} scheduler_SJF_t;

// Creates and returns scheduler specific info
void* schedulerSJFCreate()
{
    scheduler_SJF_t* info = malloc(sizeof(scheduler_SJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->queue = list_create(compare_Job);
    info->job = NULL;
    return info;
}

// Destroys scheduler specific info
void schedulerSJFDestroy(void* schedulerInfo)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(info->job == NULL){
        info->job = job;
        list_insert(info->queue, info->job);
        uint64_t jobCompletionTime = jobGetJobTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        list_insert(info->queue, job);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* temp = NULL;
    list_node_t* remove_node = list_find(info->queue, info->job);
    temp = info->job;
    list_remove(info->queue, remove_node);
    if(list_count(info->queue) != 0){
        /*
        / Implementation for the [info->job = ?]
        / Linked list sorts, getting short set equal the current job
        /
        */
        uint64_t jobCompletionTime = jobGetJobTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        info->job = NULL;
    }
    return temp;
}
