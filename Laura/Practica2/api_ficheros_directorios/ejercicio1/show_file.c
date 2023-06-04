#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void originalReadWrite(FILE *file, char c)
{
	// getc lee el siguiente caracter del fichero que se le pasa y lo devuelve como un char casteado a int
	// o EOF cuando acaba el archivo
	/* Read file byte by byte */

	while ((c = getc(file)) != EOF)
	{
		// Escribe un char c que sea un unsigned char en el archivo que se le pase
		//  Devuelve el caracter escrito o EOF
		/* Print byte to stdout */
		int ret;
		ret = putc((unsigned char)c, stdout);

		if (ret == EOF)
		{
			fclose(file);
			err(3, "putc() failed!!");
		}
	}
}
int main(int argc, char *argv[])
{
	FILE *file = NULL;
	int *c;
	//int charOriginal;
	int ret;

	// Devuelve error si no se detectan dos argumentos en la entrada a la ejecución
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	/* Open file */
	// Devuelve error si no ha podido abrir el archivo de lectura
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	// Mismo funcionamiento que getc pero implementado con fread
	// Como c tiene que ser un puntero para fread hay que reservarle y liberarle el espacio en memoria
	// Solo va a guardar un char asi que con eso es suficiente
	c = malloc(sizeof(char));

	while (fread(c, 1, 1, file) != 0)
	{
		// Escritura del archivo con fwrite en lugar de putc
		// fwrite escribe la cantidad de chars del tamaño solicitado que se le pasan del puntero c en la salida
		//(en este caso stdout->consola)
		ret = fwrite(c, 1, 1, stdout);
		if (ret == 0)
		{
			fclose(file);
			err(3, "putc() failed!!");
		}
	}
	free(c);
	fclose(file);
	return 0;
}
