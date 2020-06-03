#include "nSysimp.h"
#include "nSystem.h"

void *nExchange(nTask task, void *msg, int timeout){
    START_CRITICAL();
    nTask this_task= current_task;
    this_task->task_ex=task;
    this_task->msg_ex=msg;
    if((task->status==WAIT_EXCHANGE || task->status == WAIT_EXCHANGE_TIMEOUT) && task->task_ex==this_task){ //task_ex 2
        if (task->waiting) {
            task->waiting = FALSE;
            this_task->waiting = FALSE;
        }
        if (task->status == WAIT_EXCHANGE_TIMEOUT) {
            CancelTask(task);
        }
        this_task->msg_ret= task->msg_ex;
        task->msg_ret = this_task->msg_ex;
        task->status=READY;
        PushTask(ready_queue,task);
    }
    else{ //task_ex 1
        if (timeout>0){
            this_task->status = WAIT_EXCHANGE_TIMEOUT;
            this_task->waiting = TRUE;
            ProgramTask(timeout);
        }else {
            this_task->status=WAIT_EXCHANGE;
        }
        ResumeNextReadyTask();
        if (this_task->waiting) {
            END_CRITICAL();
            return NULL;
        }
    }
    void *ret;
    ret=this_task->msg_ret;
    END_CRITICAL();
    return ret;
}

