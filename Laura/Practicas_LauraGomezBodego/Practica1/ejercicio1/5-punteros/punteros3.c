#include <stdio.h>
#include <stdlib.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 *  1. ¿Por qué cambia el valor de ptr[13] tras la asignación ptr = &c;?
 * 		+ Porque con la asignación ptr apunta ahora a la dirección en la que se encuentra c y ha perdido la referencia
 * 		a las asignaciones que tenía anteriormente
 * 
 *  2. El código tiene (al menos) un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué?
 * 		+ El error se manifiesta en ejecución en la llamada a free porque intenta liberar un espacio de memoria 
 * que no ha sido asignado con malloc(asignación dinámica), ya que ptr apunta a c, no al malloc original
 *  3. ¿Qué ocurre con la zona reservada por malloc() tras a asignación ptr = &c;? ¿Cómo se puede acceder a ella?
 *     ¿Cómo se puede liberar dicha zona?
 * 		+ La zona reservada por malloc queda sin ninguna referencia y no se puede ni acceder a ella ni liberarla
 ************/

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	//para que no de error free debe estar antes de la reasignación de ptr
	free(ptr);
	ptr = &c;
	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);


}

