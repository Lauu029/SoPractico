#include <stdio.h>

/******** TAREAS *****
* 1. Compila el siguiente código y ejecútalo
    + Para compilar: g++ -o (nombre del ejecutable) <nombre del archivo>.c

* 2. Posteriormente, obtén la salida de la etapa de pre-procesado (opción -E o
*    la opción --save-temps para obtener la salida de todas las etapas
*    intermedias) y en un fichero hello2.i
    + -E lo escribe todo en la consola, --save-temps lo genera en archivos .i .o .s

* 3. ¿Qué ha ocurruido con la "llamada a min()" en hello2.i?
    + Ha cambiado la llamada a min() por su definición que está en la segunda línea del archivo 
    min()->  (a<b)?a:b 

* 4. ¿Qué efecto ha tenido la directiva #include <stdio.h>?
    + Se ha copiado todo el código del archivo stdio.h al archivo que lo incluye. Se ha puesto todo el código
    encima del código de hello2.c
*****************/

#define N 5

#define min(x,y) ( (x<y)?x:y )
int a = 7;
int b = 9;
int main() {

//Cambiado a const char para evitar el warning:  warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]
const char* cad = "Hello world";
 int i;

 for (i=0;i<N;i++) {
   printf("%s \t a= %d b= %d\n",cad,a,b);
   a++;
   a = min(a,b);
 }
 return 0;
}
