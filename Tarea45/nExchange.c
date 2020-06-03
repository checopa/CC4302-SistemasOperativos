#include "nSysimp.h"
#include "nSystem.h"

void* nExchange(nTask task, void* msg, int timeout) {
    START_CRITICAL();
    nTask this_task = current_task;
    this_task->task_ex = task;
    this_task->msg_ex = msg;
    if (task->status !=WAIT_EXCHANGE) {
        this_task->status = WAIT_EXCHANGE;
        ResumeNextReadyTask();
        msg = task->msg_ex;
    } else {
        while(this_task != task->task_ex) {
            this_task->status = WAIT_EXCHANGE;
            ResumeNextReadyTask();
        }
        msg = task->msg_ex;
        if (task->status == WAIT_EXCHANGE) {
            task->status = READY;
            PushTask(ready_queue,task);
        }
    }
    END_CRITICAL();
    return msg;
}