#include <stdio.h>
#include <string.h>

/********* TAREAS ************
 * Compila y ejecuta el código. Responde a las siguientes cuestiones:
 * 1. El código contiene un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué se produce?
 * 		+ Se manifiesta un error en ejecución  no se ha puesto un caracter de finalización al mensaje by y
 * 		está escribiendo valores sin asignar del puntero msg.
 *
 * Soluciona el error comentando la(s) línea(s) afectadas. Vuelve a compilar y ejecutar.(añadido msg[2]='\0';)
 * 
 * 2. ¿En qué dirección está la letra 'B' de la cadena Bonjour? ¿Y la de la la letra 'j'?
 * 		+ La b, al ser la primera letra del puntero está en la dirección a la que apunta, es decir 0x55c1d2fe7008,
 * 		la letra j está sumando 3 posiciones o bytes(ya que cada char ocupa un byte) a la posición a la que apunta p
 * 
 * 3. Tras la asignación p=msg2;, ¿cómo podemos recuperar la dirección de la cadena "Bonjour"?
 *		+ No se puede recuperar, es memoria perdida.

 * 4. ¿Por qué la longitud de las cadenas p y msg2 es 2 tras la línea 30?
 *    Se asignan 3 bytes a 'p' que modifican a ambos, pero luego la longitud es sólo 2.
 * 		+ El último byte ('\0') es un byte nulo que no cuenta al coger la longitud de la palabra, ya que solo se 
 * 		utiliza para indicar el final de la palabra.
 * 
 * 5. ¿Por qué strlen() devuelve un valor diferente a sizeof()?
 * 		+ Porque strlen devuelve la cantidad de letras que tiene la palabra y sizeof su tamaño en bytes en memoria.
 ************** */
int main()
{
	char msg[10];			 /* array of 10 chars */
	char *p;				 /* pointer to a char */
	char msg2[28] = "Hello"; /* msg2 = 'H' 'e' 'l' 'l' 'o' '\0' */

	p = "Bonjour";
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p = msg2;
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p[0] = 'H', p[1] = 'i', p[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y', msg[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}
