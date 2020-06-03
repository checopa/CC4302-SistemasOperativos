#include "spooler.h"
#include "stdlib.h"

int servProc(Spooler *sp, int id);

Spooler *makeSpooler(int p) {
  int i;
  Spooler *sp= nMalloc(sizeof(Spooler));
  sp->p= p;
  sp->exit= FALSE;
  sp->mon= nMakeMonitor();
  sp->hiqueue= MakeFifoQueue();
  sp->loqueue= MakeFifoQueue();
  sp->servers= nMalloc(p*sizeof(nTask));
  for (i=0; i<p; i++)
    sp->servers[i]= nEmitTask(servProc, sp, i);
  return sp;
}

void destroySpooler(Spooler *sp) {
  int i;
  nEnter(sp->mon);
  sp->exit= TRUE;
  nNotifyAll(sp->mon);
  nExit(sp->mon);
  for (i= 0; i<sp->p; i++)
    nWaitTask(sp->servers[i]);
  nFree(sp->servers);
  DestroyFifoQueue(sp->hiqueue);
  DestroyFifoQueue(sp->loqueue);
  nDestroyMonitor(sp->mon);
  nFree(sp);
}

void printJob(Spooler *sp, char *txt) {
  Job job;
  job.finished= FALSE;
  job.txt= txt;
  
  nEnter(sp->mon);
  PutObj(sp->hiqueue, &job);
  nNotifyAll(sp->mon);
  while (!job.finished)
    nWait(sp->mon);
  nExit(sp->mon);
}

Job *submitJob(Spooler *sp, char *txt) {
  Job *pjob= (Job*)nMalloc(sizeof(Job));
  pjob->finished= FALSE;
  pjob->txt= txt;
  nEnter(sp->mon);
  PutObj(sp->loqueue, pjob);
  nNotifyAll(sp->mon);
  nExit(sp->mon);
  return pjob;
}

void waitJob(Spooler *sp, Job *pjob) {
  nEnter(sp->mon);
  if (QueryObj(sp->loqueue, pjob)) {
    DeleteObj(sp->loqueue, pjob);
    PutObj(sp->hiqueue, pjob);
  }
  while (!pjob->finished)
    nWait(sp->mon);
  nExit(sp->mon);
  nFree(pjob);
}

int servProc(Spooler *sp, int id) {
  Job *pjob;
  nSetTaskName("printer server");
  nPrintf("starting printer daemon %d\n", id);
  for (;;) {

    /* Get job */
    nEnter(sp->mon);
      while (!sp->exit && EmptyFifoQueue(sp->hiqueue) &&
                          EmptyFifoQueue(sp->loqueue) )
        nWait(sp->mon);
      pjob= (Job*)GetObj(sp->hiqueue);
      if (pjob==NULL)
        pjob= (Job*)GetObj(sp->loqueue);
    nExit(sp->mon);
    if (sp->exit)
      break;

    /* do job */
    seqPrint(pjob->txt, id);

    /* Notify job done */
    nEnter(sp->mon);
      pjob->finished=TRUE;
      nNotifyAll(sp->mon);
    nExit(sp->mon);
  }

  if (pjob!=NULL)
    nPrintf("Atencion: Quedaron jobs pendientes(%s)\n", pjob->txt);

  nPrintf("printer daemon %d finished\n", id);

  return 0;
}
