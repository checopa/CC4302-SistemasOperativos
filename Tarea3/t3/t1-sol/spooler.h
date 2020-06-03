#include "nSystem.h"
#include "fifoqueues.h"

/* Defina aqui las estructuras de datos para Spooler y Job */

typedef struct {
  int p, exit;
  nMonitor mon;
  FifoQueue hiqueue, loqueue;
  nTask *servers;
}
  Spooler;

typedef struct {
  int finished;
  char *txt;
}
  Job;

/* En spooler.c Ud. debe implementar los siguientes procedimientos: */

Spooler *makeSpooler(int p);
void destroySpooler(Spooler *sp);
void printJob(Spooler *sp, char *txt);
Job *submitJob(Spooler *sp, char *txt);
void waitJob(Spooler *sp, Job *pjob);

/* Este procedimiento esta implementado en testspooler.c (no lo modifique): */

void seqPrint(char *txt, int id);
