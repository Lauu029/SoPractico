#include <stdio.h>
#include <string.h>
/************ TAREAS
 * Compila y ejecuta el código. Contesta a las cuestiones:
 *  1. ¿La copia del array se realiza correctamente? ¿Por qué?
 * 		+ No se copia el array de forma correcta porque la función que "copia los arrays" simplemente
 * 		se está modificando la dirección a la que apunta dst.
 *
 *  2. Si no es correcto, escribe un código que sí realice la copia correctamente.
 *		+ Hay dos formas: con memcpy o con un for que recorra los elementos y los iguale uno a uno

 *  3. Descomenta la llamada a la fución "tmo" en la función main(). Compila de nuevo y ejecuta.
 *  4. El problema que se produce, ¿es de compilación o de ejecución? ¿Por qué se produce?
 * 		+ Se produce un error de ejecución porque intenta acceder a posiciones fuera de los límites del array
 * 
 *  5. Encuentra un valor de MAXVALID superior a 4 con el que no se dé el problema. ¿Se está escribiendo
 *     más allá del tamaño del array? Si es así, ¿por qué funciona el código?
 * 		+ El valor máximo para que no de error es 6, está sobreescribiendo posiciones de memoria que están fuera del límite del array, 
 * 		en este caso no da problemas porque son posiciones de memoria que están dentro de la pila, por tanto existen en el contexto 
 * 		de la ejecución.
 *******************/

#define N 10
#define MAXELEM 5000
#define MAXVALID 6

// funcion que imprime por pantalla el conteniod del array v de tam. size
void imprimeArray(int v[], int size)
{
	int i;
	printf("-------------------\n");
	for (i = 0; i < size; i++)
		printf("%d ", v[i]);
	printf("\n\n");
}

// funcion que copia el contenido de un array en otro
void copyArray(int src[], int dst[], int size)
{
	// Este código no realiza la copia correctamente: dst = src;
	// Código corregido:
	/*for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}*/

	// También se puede utilizar con memcpy, pero hay que añadir la biblioteca string:
	memcpy(dst, src, size * sizeof(int));
}

void tmo()
{
	int x = -1;
	int a[4] = {0, 1, 2, 3};
	int b = 10000;
	int c = -1;
	int i;

	for (i = 4; i < MAXVALID; i++)
		a[i] = i;

	printf("x %d b %d c %d\n", x, b, c);
}

int main()
{
	int A[N] = {4, 3, 8, 5, 6, 9, 0, 1, 7, 2};
	int B[N];

	tmo();
	copyArray(A, B, N);
	imprimeArray(B, N);
}
