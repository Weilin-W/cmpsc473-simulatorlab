#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// PLCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    job_t* job;
    list_t* queue;
    uint64_t arrive_timestamp;
} scheduler_PLCFS_t;

// Creates and returns scheduler specific info
void* schedulerPLCFSCreate()
{
    scheduler_PLCFS_t* info = malloc(sizeof(scheduler_PLCFS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->queue = list_create(NULL);
    info->job = NULL;
    info->arrive_timestamp = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerPLCFSDestroy(void* schedulerInfo)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->queue);
    info->arrive_timestamp = 0;
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPLCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(info->job == NULL){
        info->job = job;
        info->arrive_timestamp = currentTime;
        list_insert(info->queue, info->job);
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        uint64_t adjust_time = currentTime - info->arrive_timestamp;
        uint64_t jobRemainingTime = jobGetRemainingTime(info->job) - adjust_time;
        info->arrive_timestamp = currentTime;
        jobSetRemainingTime(info->job, jobRemainingTime);
        schedulerCancelNextCompletion(scheduler);
        info->job = job;
        list_insert(info->queue, info->job);
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPLCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    //Using cancelComplete
    //If job currently running, lastest on preemptes the current job
    job_t* temp = NULL;
    list_node_t* remove_node = list_find(info->queue, info->job);
    temp = info->job;
    list_remove(info->queue, remove_node);
    if(list_count(info->queue) != 0){
        info->job = list_head(info->queue)->data;
        info->arrive_timestamp = currentTime;
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        info->job = NULL;
    }
    return temp;
}
