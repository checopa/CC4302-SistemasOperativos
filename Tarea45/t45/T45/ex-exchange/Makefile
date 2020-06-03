# Instrucciones de uso:
# - Primero defina la variable NSYSTEM con la ubicacion de nSystem
#   en su computador.
# - Compile con: 
#   % make test-t4
#   % make test-t5
#   ...
# - Ejecute con:
#   % ./test-t4
#   % ./test-t5
# 
# No entregue en U-cursos tareas que no pasan todos los tests.
# 
# El programa test-t5 mide tiempos y por lo tanto pueden
# fallar en una maquina con mucha carga.  En ese caso repita
# la ejecucion en una maquina con poca carga.
# Si el error persiste, es su implementacion la que tiene problemas.


LIBNSYS= $(NSYSTEM)/lib/libnSys.a

CFLAGS= -ggdb -Wall -pedantic -std=c99 -I$(NSYSTEM)/include -I$(NSYSTEM)/src
LFLAGS= -ggdb

all: test-t4 test-t5

.SUFFIXES:
.SUFFIXES: .o .c .s

.c.o .s.o:
	gcc -c $(CFLAGS) $<

test-t4: test-t4.o $(LIBNSYS)
	gcc $(LFLAGS) test-t4.o -o $@ $(LIBNSYS)

test-t5: test-t5.o $(LIBNSYS)
	gcc $(LFLAGS) test-t5.o -o $@ $(LIBNSYS)

clean:
	rm -f *.o *~

cleanall:
	rm -f *.o *~ test-t4 test-t5
