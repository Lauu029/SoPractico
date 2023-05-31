#include <stdio.h>
/**** TAREAS ********
 * Compila el codigo, ejecutalo y responde a las cuestiones
 * 1. ¿Por qué el primer printf() imprime valores distintos para 'a'?
 		+ Se imprimen valores diferentes porque en el printf se indica que se deben imprimir diferentes tipos de variable
		%d indica que se debe imprimir un entero
		%c indica que se debe imprimir un char
		122 equivale al char 'z'.

 * 2. ¿Cuánto ocupa un tipo de datos char?
		+ Un char ocupa 1 byte como se indica en la cuarta linea que imprime el ejecutable

 * 3. ¿Por qué el valor de 'a' cambia tanto al incrementarlo en 6? (la  respuesta está relacionada con la cuestión anterior)
		+ El cambio se debe a que un char ocupa 1 byte que corresponde a 127 posibles valores positivos, por lo que al cambiar a 
		a 128 no encuentra ningún valor de char correspondiente.
 * 
 * 4. Si un "long" y un "double" ocupan lo mismo, ¿por qué hay 2 tipos de datos diferentes?
 * 		+ Son diferentes porque long se refiere a valores de tipo entero que pueden ir de negativos enteros muy pequeños a 
 * 		positivos muy grandes; en cambio double se refiere a variables de tipo decimal más preciso que float.
 */

char a = 122;
int b = 41;

int main()
{
	printf("a = %d a = %c \n", a, a);
	a += 7;
	printf("a = %d a = %c b=%d  b=%c\n", a, a, b, b);
	printf("Size of int: %lu\n", sizeof(int) );
	printf("Size of char: %lu\n", sizeof(char) );
	printf("Size of float: %lu\n", sizeof(float) );
	printf("Size of double: %lu\n", sizeof(double) );
	printf("Size of long: %lu\n", sizeof(long) );
	printf("Size of short: %lu\n", sizeof(short) );
	printf("Size of void*: %lu\n", sizeof(void*) );

}
