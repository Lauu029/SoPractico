#include <stdio.h>


/******** TAREAS *****
 * Compila el código y trata de ejecutarlo. Contesta a las cuestiones
 * 1. ¿Tenemos un problema de compilación o de ejecución?
 * 		+ Hay un problema de compilación, ya que un error en el código impide que se generen los archivos de compilación
 
 * 2. ¿Por qué se da el problema?. Soluciónalo, compila y ejecuta de nuevo.
		+ El problema se da porque hay un array inicializado con el tamaño de una variable no constante, que además cambia en el código.
		Para arreglarlo, he puesto directamente el valor de la variable a al inicializar el vector array2
		int array2[a] -> int array2[7]

 * 3. a,b,c, y x están declaradas de forma consecutiva. ¿Sus direcciones son concesutivas?
	+ Las direcciones son no consecutivas, como se puede observar en la salida de la ejecución.
	Por ejemplo la dirección de a dista en 8 bytes de la de a, pero a solo ocupa 4 bytes.

 * 4. ¿Qué significa el modificador "%lu" en printf()?
	+ %lu se utiliza para imprimir el valor de una variable de tipo long int 

 * 5. ¿A qué dirección apunta "pc"? ¿Coincide con la de alguna variable anteriormente declarada? Si es así,
 * ¿Coinciden los tamaños de ambas?
 * 	+ pc apunta a la dirección 0x55e482c69088, que es la dirección en la que está almacenada la variable x,
 *  pero esta tiene tamaño 1 y pc tiene tamaño 8
 * 
 * 6. ¿Coincide el valor del tamaño de "array1" con el número de elementos del array? ¿Por qué
 * 		+ Los valores no coinciden porque el tamaño del array se refiere a la cantidad absoluta de 
 * 		bytes que almacena, como se puede observar en el código, el array tiene tamaño 10 y almacena variables
 * 		de tipo int, que ocupan 4 bytes, por tanto el tamaño total es de 40 bytes.
 * 
 * 7. ¿Coinciden las direcciones a la que apunta cadena1 con la de cadena2?
 * 		+ No, apuntan a direcciones completamente diferentes
 * 
 * 8. ¿Por qué los tamaños (según sizeof()) de cadena1 y cadena2 son diferentes?
 * 		+ Cadena1 se declara como un puntero a una cadena y cadena2 se declara directamente como array, 
 * 		por lo que sizeof de la primera cadena devuelve el tamaño del puntero y sizeof de la segunda devuelve el tamaño del array
***************/

#define ARRAY_SIZE  10

int a = 7;
unsigned long b = 8;
short c;
char x;
char* pc;

int array1[ARRAY_SIZE];
int array2[7];

char* cadena1 = "CADENA DE CARACTERES";
char cadena2[] = "CADENA DE CARACTERES";
int main()
{
	pc = &x;
	printf("Direccion de a: %p Tam: %lu \n",&a,sizeof(a));
	printf("Direccion de b: %p Tam: %lu \n",&b,sizeof(b));
	printf("Direccion de c: %p Tam: %lu \n",&c,sizeof(c));
	printf("Direccion de x: %p Tam: %lu \n",&x,sizeof(x));
	printf("Direccion de pc: %p Direccion a la que apunta pc: %p Tam: %lu \n",
			&pc, pc, sizeof(pc));
	printf("Direccion de array: %p Direccion del elem 0: %p Tam de array: %lu \n",
			array1, &array1[0], sizeof(array1));
	printf("Direccion de cadena1: %p Direccion a la que apunta: %p Tam: %lu \n",
			&cadena1, cadena1, sizeof(cadena1));
	printf("Direccion de cadena2: %p Direccion a la que apunta: %p Tam: %lu \n",
			&cadena2, cadena2, sizeof(cadena2));
    return 0;
}
