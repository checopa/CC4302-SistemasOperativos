#include <stdlib.h>
#include <math.h>
#include <nSystem.h>

#define FALSE 0
#define TRUE  1

/* Esta parte es dada */

typedef unsigned long long ulonglong;
typedef unsigned int uint;

uint buscarFactor(ulonglong x, uint i, uint j) {
  for (uint k= i; k<=j; k++) {
    if (x % k == 0)
      return k;
  }
  return 0;
}

int buscarF(ulonglong x, uint i, uint j) {
  return (int) buscarFactor(ulonglong x, uint i, uint j);
}

/* Esta parte es dada */

int nMain(int argc, char **argv) {
  nSetTimeSlice(1);

  ulonglong x = atoll(argv[1]);
  uint raiz_x = (uint)sqrt((double)x);
  int p = atoi(argv[2]);

  nPrintf("x=%lld raiz entera=%u\n", x, raiz_x);

  nTask minions[ p ];

  int casos = raiz_x - 1;
  int cantidadPorMinion = casos / p;
  int marca = TRUE;

  if ( cantidadPorMinion == 0 )
  {
    for ( int i = 0; i < casos; i++ )
    {
      minions[ i ] = nEmitTask( buscarF, x, i + 2, i + 2 );
    }
    
    for ( int i = 0; i < casos; i++ )
    {
      int r = nWaitTask( minions[ i ] );

      if ( r != 0 && marca )
      {
        nPrintf("no es primo: factor=%u\n", r);
        marca = FALSE;
      }
    }
    if ( marca )
    {
      nPrintf("primo\n");
    }
  }
  else
  {
    int cont = 2;
    for ( int i = 0; i < p; i++ )
    {
      if ( i == p - 1 )
      {
        minions[ i ] = nEmitTask( buscarF, x, cont, raiz_x );
      }
      else
      {
        minions[ i ] = nEmitTask( buscarF, x, cont, cont + cantidadPorMinion - 1 );
        cont = cont + cantidadPorMinion; 
      }
    }

    for ( int i = 0; i < p; i++ )
    {
      int r = nWaitTask( minions[ i ] );

      if ( r != 0 && marca )
      {
        nPrintf("no es primo: factor=%u\n", r);
        marca = FALSE;
      }
    }
    if ( marca )
    {
      nPrintf("primo\n");
    }
  }
  
    
  return 0;
}

