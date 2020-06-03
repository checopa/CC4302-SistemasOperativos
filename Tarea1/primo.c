/*
#include <stdlib.h>
#include <math.h>
#include <nSystem.h>


typedef unsigned long long ulonglong;
typedef unsigned int uint;



uint buscarFactor(ulonglong x, uint i, uint j) {
  for (uint k= i; k<=j; k++) {
    if (x % k == 0)
      return k;
  }
  return 0;
}

int buscar(ulonglong x, uint i, uint j) {
  int res = (int) buscarFactor(x,i,j);
  return res;
}

int nMain(int argc, char **argv) {
  nSetTimeSlice(1);
  ulonglong x= atoll(argv[1]);
  ulonglong y= atoll(argv[2]);
  uint raiz_x= (uint)sqrt((double)x);
  int step;
  if (y > raiz_x) {
    y = raiz_x;
  }
  step = (raiz_x - 2 + y) / y;
  nTask tareas[y];
  nPrintf("x=%lld raiz entera=%u\n", x, raiz_x);
  for (int i = 0; i < y; i++) {
    tareas[i] = nEmitTask(buscar, x, 2 + step*i, 2 + step*(i+1) - 1) ;
  }
  int f = 0;
  int result;
  for (uint i = 0; i < y; i++) {
    result = nWaitTask(tareas[i]);
    if (result != 0 && f == 0) {
      f = result;
    }
  }
  if (f==0)
    nPrintf("primo\n");
  else
    nPrintf("no es primo: factor=%u\n", f);
  return 0;
}
*/

#include <stdlib.h>
#include <math.h>
#include <nSystem.h>

/* Esta parte es dada */

typedef unsigned long long ulonglong;
typedef unsigned int uint;
int nBuscarFactor(ulonglong x, uint i, uint j, uint *factor);


uint buscarFactor(ulonglong x, uint i, uint j) {
  for (uint k= i; k<=j; k++) {
    if (x % k == 0){
        return k;
    }
  }
  return 0;
}

int nBuscarFactor(ulonglong x, uint i, uint j, uint *factor){
  if (*factor)
      return *factor;
  int r = (int)buscarFactor(x,i,j);
  if (r > 1){
      *factor = r;
      return r;
  }
  return 0;
}

/* Esta parte es dada */

int nMain(int argc, char **argv) {
  nSetTimeSlice(1);
  ulonglong x= atoll(argv[1]);
  uint p = atoi(argv[2]);
  uint raiz_x= (uint)sqrt((double)x);
  nPrintf("x=%lld raiz entera=%u\n", x, raiz_x);
  uint f= 0;
  nTask tareas[p];
  uint r1, r2;
  for (uint i=0; i<p; i++){
      if(raiz_x < 2){
        r1 = (uint)(raiz_x*i)/p + 1;
        r2 = (uint)(raiz_x*(i+1))/p + 1;
        }
      else{
        r1 = (uint)((raiz_x - 2)*i)/p+2;
        r2 = (uint)((raiz_x - 2)*(i+1))/p+2;
      }

      tareas[i] = nEmitTask(nBuscarFactor, x, r1, r2, &f);
  }
  for (uint i=0;i<p;++i){
        nWaitTask(tareas[i]);
    }
  if (f==0)
    nPrintf("primo\n");
  else
    nPrintf("no es primo: factor=%u\n", f);
  return 0;
}

