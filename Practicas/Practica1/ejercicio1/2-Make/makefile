#VARIABLES
#Variable del compilador
CC = gcc
#Opciones del compilador
CFLAGS = -Wall -g
#librerías que tiene que enlazar el programa
LIBS = -lm

#OBJETIVOS Y REGLAS
#Indica como compilar un archivo .o a partir del archivo fuente
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
# Se ejecuta cuando se pone solo make, compila todo y lo enlaza al ejecutable prueba
all : aux.o init.o aux.h
	gcc $(CFLAGS) -o prueba aux.o init.o $(LIBS)

.PHONY: clean
#Elimina los archivos .o y el ejecutable cuando se pone make clean
clean:
	-rm *.o prueba
