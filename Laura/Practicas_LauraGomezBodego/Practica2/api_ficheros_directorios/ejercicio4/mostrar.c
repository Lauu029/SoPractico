#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
void read_file(char *file, int offset, int start)
{
	int fdo;
	if ((fdo = open(file, O_RDONLY)) == -1)
	{
		err(2, "The input file %s could not be opened", file);
	}

	// Desplazo el puntero del fichero el offset necesario
	lseek(fdo, offset, start);
	
	// lectura normal del resto del fichero y muestra por pantalla
	int readBytes;
	char *buff;
	buff = malloc(sizeof(char));
	while (readBytes = read(fdo, buff, sizeof(buff)))
	{
		// Para que write escriba en la linea de comandos se le pasa 1 en el descriptor del fichero destino
		write(1, buff, readBytes);
	}
	close(fdo);
	free(buff);
}
int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		printf("Faltan argumentos en la llamada\n");
		exit(1);
	}
	int N = 0;
	char options;
	// flag para determinar si el offset del fichero es desde el comienzo o el final del archivo
	int front = SEEK_SET;
	// Nombre del documento que hay que leer
	char *readFile = argv[1];

	// getopt parsea los comandos que se le piden al ejecutar un programa, se le pasan los argumentos
	// de main y un string con el tipo de chars que acepta. Si el comando requiere tambien un valor detras
	// se pone ':'. Por ejemplo, -n requiere un numero detras asi que se pone'n:'
	// Devuelve -1 cuando los ha parseado todos
	while ((options = getopt(argc, argv, "en:")) != -1)
	{
		switch (options)
		{
		case 'e':
			// Se le indica al fichero que el offset es desde el final del archivo(por eso tambien se pone negativo
			//  para que el offset vaya hacia atras)
			front = SEEK_END;
			N = -N;
			break;
		case 'n':
			// Para convertir el argumento en un int hay que hacer atoi
			N = atoi(optarg);
			break;
		case 'i':

			break;
		default:
			break;
		}
	}

	read_file(readFile, N,front);
	return 0;
}
