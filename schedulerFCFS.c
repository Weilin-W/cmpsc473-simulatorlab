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
    int time;
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
    info->time = 0;
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
    //Fetched list, solve issue at which the job already being run
    info->job = job;
    uint64_t jobCompletionTime = jobGetJobTime(info->job)+currentTime;
    scheduler->schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    info->time = currentTime;
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
    if(info->time == currentTime){
        scheduler->schedulerCancelNextCompletion(currentTime);
    }
    list_next(info->job);
    return (job_t* )info->job;
}
