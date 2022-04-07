#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// FCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    job_t* job;
    list_t* queue;
} scheduler_FCFS_t;

// Creates and returns scheduler specific info
void* schedulerFCFSCreate()
{
    scheduler_FCFS_t* info = malloc(sizeof(scheduler_FCFS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->queue = list_create(NULL);
    info->job = NULL;
    return info;
}

// Destroys scheduler specific info
void schedulerFCFSDestroy(void* schedulerInfo)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerFCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    //Check current job
    if(info->job == NULL){
        //Set the current job to the new job
        //Insert into the queue
        //Calculate the completion time, and schedule
        info->job = job;
        list_insert(info->queue, info->job);
        uint64_t jobCompletionTime = jobGetJobTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        //Just put the new job into the queue
        list_insert(info->queue, job);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerFCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    //Next job: remove the completed job to the list and move to next job.
    //if job is complete, check queue, assign the tail job to the current job
    //if nothing after current job in the queue, set to NULL
    //returns the job that is being completed
    job_t* temp = NULL;
    list_node_t* remove_node = list_find(info->queue, info->job);
    temp = info->job;
    list_remove(info->queue, remove_node);
    if(list_count(info->queue) != 0){
        info->job = list_tail(info->queue)->data;
        uint64_t jobCompletionTime = jobGetJobTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        info->job = NULL;
    }
    return temp;
}
