#include <nSystem.h>
#include <fifoqueues.h>
#include "batch.h"

FifoQueue queue;
nMonitor m;
nTask* jobs;
int task;
nCondition emptyQueue;
nCondition noEmptyQueue;
int submit;
int wait;
int finished;

struct job {
  JobFun fun;
  void* input;
  int finished;
  void* result;
  nCondition condition;
};

int doJob() {
  Job* job;
  while(TRUE){
    nEnter(m);
    while(EmptyFifoQueue(queue) && !finished){
      nWaitCondition(noEmptyQueue);
    }
    if (!EmptyFifoQueue(queue)) {
      job= (Job*)GetObj(queue);
    }
    nExit(m);
    if (finished) {
      break;
    }
    job->result = job->fun(job->input);
    job->finished=TRUE;
    nEnter(m);
    nSignalCondition(job->condition);
    nExit(m);
  }
  nEnter(m);
  nSignalCondition(noEmptyQueue);
  nExit(m);
  return 0;
}

void startBatch(int n) {
  task = n;
  int i;
  submit = 0;
  wait = 0;
  finished = 0;
  m = nMakeMonitor();
  emptyQueue = nMakeCondition(m);
  noEmptyQueue = nMakeCondition(m);
  queue = MakeFifoQueue();
  jobs = nMalloc(n*sizeof(nTask));
  for (i = 0; i < n; i++) {
    jobs[i] = nEmitTask(doJob);
  }
}

void stopBatch() {
  int i;
  nEnter(m);
  while(!EmptyFifoQueue(queue) || wait != submit) {
    nWaitCondition(emptyQueue);
  }
  finished = 1;
  nSignalCondition(noEmptyQueue); 
  nExit(m);
  for (i = 0; i < task; i++) {
    nWaitTask(jobs[i]);
  }
  nDestroyCondition(emptyQueue);
  nDestroyCondition(noEmptyQueue);
  DestroyFifoQueue(queue);
  nDestroyMonitor(m);
  nFree(jobs);
}

Job *submitJob(JobFun fun, void *input) {
  Job *job= (Job*)nMalloc(sizeof(Job));
  job->fun = fun;
  job->input = input;
  job->finished = FALSE;
  job->condition = nMakeCondition(m);
  nEnter(m);
  submit++;
  PutObj(queue, job);
  nSignalCondition(noEmptyQueue);
  nExit(m);
  return job;
}

void *waitJob(Job *job) {
  void* ret;
  nEnter(m);
  while(!job->finished) {
    nWaitCondition(job->condition);
  }
  nExit(m);
  ret = job->result;
  nEnter(m);
  wait++;
  if (wait == submit) {
    nSignalCondition(emptyQueue);
  }
  nExit(m);
  nDestroyCondition(job->condition);
  nFree(job);
  return ret;
}