#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

//Job compare function:
//Compares job data1 and job data2
//Return 1, if job data1 time is greater than the job data 2 time
//If equal; check job ID
//Else Return -1
int compare_PJob(void* data1, void* data2){
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
// PSJF scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    job_t* job;
    list_t* queue;
    uint64_t arrive_timestamp;
} scheduler_PSJF_t;

// Creates and returns scheduler specific info
void* schedulerPSJFCreate()
{
    scheduler_PSJF_t* info = malloc(sizeof(scheduler_PSJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->queue = list_create(compare_PJob);
    info->job = NULL;
    info->arrive_timestamp = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerPSJFDestroy(void* schedulerInfo)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    //Check the current job
    if(info->job == NULL){
        //Set the new job to the current job, update the arrive time
        //Put the current job into the queue
        info->job = job;
        info->arrive_timestamp = currentTime;
        list_insert(info->queue, info->job);
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        //Calculate the time that the job remains, cancel the next completion
        //Update current job, and put into queue
        uint64_t adjust_time = currentTime - info->arrive_timestamp;
        uint64_t jobRemainingTime = jobGetRemainingTime(info->job) - adjust_time;
        info->arrive_timestamp = currentTime;
        jobSetRemainingTime(info->job, jobRemainingTime);
        if(info->job != NULL){
            schedulerCancelNextCompletion(scheduler);
        }
        list_insert(info->queue, job);
        info->job = list_head(info->queue)->data;
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* temp = NULL;
    temp = info->job;
    if(info->job != NULL){
        list_node_t* infoJob_node = list_find(info->queue, info->job);
        list_remove(info->queue, infoJob_node);
    }
    if(list_count(info->queue) != 0){
        //Set the current job to the head job of the queue
        //Remove head node of the queue
        //Calculate the Job completion time, and schedule
        info->job = list_head(info->queue)->data;
        info->arrive_timestamp = currentTime;
        uint64_t jobCompletionTime = jobGetRemainingTime(info->job)+currentTime;
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        info->job = NULL;
        info->arrive_timestamp = 0;
    }
    //Returns the job that is being completed
    return temp;
}
