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
int compare_PS_Job(void* data1, void* data2){
    if(jobGetRemainingTime((job_t*)data1) > jobGetRemainingTime((job_t*)data2)){
        return 1;
    }else if(jobGetRemainingTime((job_t*)data1) == jobGetRemainingTime((job_t*)data2)){
        if(jobGetId((job_t*)data1) > jobGetId((job_t*)data2)){
            return 1;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}
// PS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    job_t* job;
    list_t* queue;
    uint64_t remainder_unaccounted_time;
    //uint64_t arrive_timestamp;
} scheduler_PS_t;

//Previous Job arrival time
uint64_t prev_job_time = 0;

// Creates and returns scheduler specific info
void* schedulerPSCreate()
{
    scheduler_PS_t* info = malloc(sizeof(scheduler_PS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->queue = list_create(compare_PS_Job);
    info->job = NULL;
    return info;
}

// Destroys scheduler specific info
void schedulerPSDestroy(void* schedulerInfo)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(list_count(info->queue) == 0){
        //Update the current job, calculate the completion time and schedule
        info->job = job;
        list_insert(info->queue, info->job);
        prev_job_time = jobGetArrivalTime(info->job);
        uint64_t jobCompletionTime = currentTime + jobGetJobTime(info->job);
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        //Calculate the time that the job remains, cancel the next completion
        //Update current job, and put into queue
        //uint64_t adjust_time = currentTime - info->arrive_timestamp;
        list_node_t* head_node = list_head(info->queue);
        //Current time minus the start time of the job
        uint64_t adjust_time =  currentTime - jobGetArrivalTime(info->job);
        uint64_t unaccounted_time = (info->remainder_unaccounted_time + (adjust_time)) / list_count(info->queue);
        uint64_t jobRemainingTime = jobGetRemainingTime(info->job);
        while(head_node != NULL){
            //info->arrive_timestamp = currentTime;
            jobSetRemainingTime(info->job, jobRemainingTime - prev_job_time);
            prev_job_time = jobGetArrivalTime(info->job);
            info->remainder_unaccounted_time += unaccounted_time;
            head_node = head_node->next;
        }
        unaccounted_time = (unaccounted_time + (currentTime - prev_job_time)) % list_count(info->queue);
        if(info->job != NULL){
            schedulerCancelNextCompletion(scheduler);
        }
        list_insert(info->queue, job);
        info->job = list_tail(info->queue)->data;
        uint64_t jobCompletionTime = jobGetRemainingTime(list_tail(info->queue)->data) * list_count(info->queue) + (currentTime - unaccounted_time);
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
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
        info->job = list_tail(info->queue)->data;
        //info->arrive_timestamp = currentTime;
        list_node_t* head_node = list_head(info->queue);
        uint64_t adjust_time =  currentTime - jobGetArrivalTime(info->job);
        uint64_t unaccounted_time = (info->remainder_unaccounted_time + (adjust_time)) / list_count(info->queue);
        uint64_t jobRemainingTime = jobGetRemainingTime(info->job);
        while(head_node != NULL){
            //info->arrive_timestamp = currentTime;
            jobSetRemainingTime(info->job, jobRemainingTime - prev_job_time);
            prev_job_time = jobGetArrivalTime(info->job);
            info->remainder_unaccounted_time += unaccounted_time;
            head_node = head_node->next;
        }
        unaccounted_time = (unaccounted_time + (currentTime - prev_job_time)) % list_count(info->queue);
        uint64_t jobCompletionTime = jobGetRemainingTime(list_tail(info->queue)->data) * list_count(info->queue) + (currentTime - unaccounted_time);
        schedulerScheduleNextCompletion(scheduler, jobCompletionTime);
    }else{
        info->job = NULL;
        //info->arrive_timestamp = 0;
    }
    //Returns the job that is being completed
    return temp;
}
