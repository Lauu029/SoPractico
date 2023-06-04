#include <stdio.h>
#include <stdlib.h>
#include <err.h>
// Inclusiones que pide el manual para open, write
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Copia el contenido del fichero con id fdo en el fichero con id fdd
int copy(int fdo, int fdd)
{
	// Buffer que almacena los chars que hay que copiar de un archivo al otro
	unsigned char buff[512];
	/*
	Devuelve el número de bytes que ha leido, si ha llegado al final del fichero devuelve 0, la posicion del archivo
	avanza el número de bytes leidos.
	Si el numero es menor que la cantidad de bytes que se le pide leer no da error
	*/
	// read lee el número solicitado de bytes del fichero que se le pasa y los guarda en el buffer
	// Esta variable sirve para indicarle a write cuantos caracteres debe escribir, teniendo en cuenta que al
	// final de los archivos no tiene por que leer 512 bytes
	int readBytes;
	while (readBytes = read(fdo, buff, sizeof(buff)))
	{
		// write funciona igual que read pero escribiendo
		if (write(fdd, buff, readBytes) == -1)
		{
			// Cierra los ficheros
			close(fdd);
			close(fdo);
			err(3, "putc() failed!!");
			return -1;
		}
	}
	if (readBytes == -1)
	{
		close(fdd);
		close(fdo);
		err(3, "putc() failed!!");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// Comprueba que la cantidad de argumentos que se le han pasado es la correcta
	// En este caso un archivo de lectura y uno de escritura
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	// Indicador del archivo abierto que devuelve open
	int readFile, writeFile;
	// open abre un archivo de la direccion que se le pase en los argumentos
	// Utiliza distintos flags (pongo los que el enunciado sugiere mirar):
	//		-O_RDONLY = Solo lee el archivo
	//		-O_WRONLY = Solo escrive en el archivo
	//		-O_CREAT = Si la dirección o nombre del archivo no existe, lo crea
	//		-O_TRUNC = Si el archivo ya existe y hay una flag de escritura, se corta a longitud 0

	//HAY QUE PONER SIEMPRE TODOS LOS PARENTESIS AL HACER IGUALDADES Y COMPROBAR QUE SI NO DAN ERRORES RAROS 
	if ((readFile = open(argv[1], O_RDONLY)) == -1)
	{
		// Devuelve error si no ha podido abrir el archivo
		err(2, "The input file %s could not be opened", argv[1]);
	}
	// Crea y abre el archivo de lectura
	// el flag -> da permisos -> ver manual para diferentes permisos
	if ((writeFile = open(argv[2], O_WRONLY | O_CREAT, 00700)) == -1)
	{
		err(2, "The input file %s could not be opened", argv[2]);
	}
	// Si no ha dado errores y ha cerrado el archivo, los cierra aquí
	if (copy(readFile, writeFile) == 0)
	{
		close(readFile);
		close(writeFile);
	}
	return 0;
}
