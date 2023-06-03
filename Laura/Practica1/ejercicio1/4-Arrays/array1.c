#include <stdio.h>

/************** TAREAS********
 * Compila y ejecuta el código. Contesta a las cuestiones
 * 1. ¿Por qué no es necesario escribir "&list" para obtener la dirección del array list?
 * 		+ Cuando se utiliza el nombre de un array en un contexto en el que se espera una
 * 		dirección de memoria, el nombre del array se convierte de forma implícita en un puntero al primer
 * 		elemento del array.
 * 
 * 2. ¿Qué hay almacenado en la dirección de "list"?
 * 		+ La dirección de list conytiene la ubicación de memoria del primer elemento del array.
 * 
 * 3. ¿Por qué es necesario pasar como argumento el tamaño del array en la función init_array?
 * 		+ Porque init_array recibe un puntero a un array y el tamaño de dicho array, pero el puntero no 
 * 		tiene información de cual es el tamaño del array, solo cual es su dirección de memoria
 * 
 * 4. ¿Por qué el tamaño (sizeof())  del array de la función "init_array" no coincide con el declarado en main()?
 * 		+ Porque sizeof(array) devuelve el tamaño del puntero al array
 * 
 * 5. ¿Por qué NO es necesario pasar como argumento el tamaño del array en la función init_array2?
 * 		+ Poruqe la función init_array2 obtiene como argumento un array de tamaño fijo N, por lo que se conoce
 * 		el tamaño del array en tiempo de compilación.
 * 
 * 6. ¿Coincide el  tamaño (sizeof())  del array de la función "init_array2" con el declarado en main()?
 * 		No, porque sizeof devuelve el tamaño del puntero al array
 ***************/

#define N 5

void init_array(int array[], int size) ;
void init_array2(int array[N]);

int main(void)
{
	int i,list[N];
	printf("Dir de list %p Dir de list[0]: %p  Dir de list[1]: %p. Sizeof list %lu \n",
			list, &list[0], &list[1], sizeof(list));

	init_array(list, N);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");

	init_array2(list);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");
}

void init_array(int array[], int size)
{
	//He cambiado el valor de sizeof de array a &array, que sigue devolviendo el valor del puntero de array
	//pero evita los warnings
	int i;
	printf("Direccion de array: %p Sizeof array %lu \n", array, sizeof(&array));
	for (i = 0; i < size; i++)
		array[i] = i;
	printf("Array initialized\n\n");
}

void init_array2(int array[N])
{
	int i;
	printf("Direccion de array: %p Sizeof array %lu \n", array, sizeof(&array));
	for (i = 0; i < N; i++)
		array[i] = i*2;
	printf("Array initialized\n\n");
}


