#include <stdio.h>
#include <stdlib.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 *  1. ¿Cuántos bytes se reservan en memoria con la llamada a malloc()?
 * 		+ Se reservan nelem(127) * el tamaño de un int que son 4 bytes.
 * 
 *  2. ¿Cuál es la dirección del primer y último byte de dicha zona reservada?
 * 		+ La dirección del primer byte es 0x55741a0a62a0 ya que el puntero apunta a el primer elemento del array
 * 		y la úlima sería 0x55741a0a62a0 + 127*4 que es el tamaño que se ha reservado para ese puntero.
 * 
 *  3. ¿Por qué el contenido de la dirección apuntada por "ptr" es 7 y no 5 en el primer printf()?
 * 		+ Al asignar la posicion 0 del array a 7, se sobreescribe el 5 que se había asignado ya que hacer
 * 		"*ptr=" y "ptr[0]=" es lo mismo
 * 
 *  4. ¿Por qué se modfica el cnotenido de ptr[1] tras la sentencia *ptr2=15;?
 * 		+ Porque al hacer ptr2= ptr, ambos punteros apuntan a la misma dirección de memoria, por tanto cuando
 * 		se cambia el valor en uno de los dos, esta cambiando un valor común.
 * 
 *  5. Indica dos modos diferentes de escribir el valor 13 en la dirección correspondiente a ptr[100].
 * 		+ ptr[100] = 13
 * 		+ ptr += 100; *ptr = 13;
 *  6. Hay un error en el código. ¿Se manifiesta en compilación o en ejecución?
 *     Aunque no se manifieste, el error está. ¿Cuál es?
 * 		+ El error no se manifiesta pero sería en ejecución , pero sucede porque se llama a *ptr=3 después de haber liberado la memoria del 
 * 		puntero
 *  ***********/
int nelem;

int main(void)
{
	int *ptr;
	int * ptr2;

	nelem = 127;
	ptr = (int*) malloc(nelem*sizeof(int));
	*ptr = 5;
	ptr[0] = 7;
	ptr2 = ptr;

	printf("Direccion a la que apunta ptr %p. Contenido de esa direccion: %d \n",
			ptr, *ptr);

	ptr[1] = 10;
	printf("Direccion a la que apunta ptr[1] %p. Contenido de esa direccion: %d \n",
			&ptr[1], ptr[1]);

	ptr2++;
	*ptr2 = 15;
	printf("Direccion a la que apunta ptr[1] %p. Contenido de esa direccion: %d \n",
			&ptr[1], ptr[1]);

	free(ptr);
	*ptr = 3;
	printf("Direccion a la que apunta ptr %p. Contenido de esa direccion: %d \n",
			ptr, *ptr);
	return 0;
}

