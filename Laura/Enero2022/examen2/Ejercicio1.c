#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

    if (argc < 1)
    {
        printf("Faltan argumentos en la llamada\n");
        exit(1);
    }
    FILE *file = NULL;
    int N = 0;
    // Nombre del documento que hay que leer
    char *readFile = "*";
    char options;
    // getopt parsea los comandos que se le piden al ejecutar un programa, se le pasan los argumentos
    // de main y un string con el tipo de chars que acepta. Si el comando requiere tambien un valor detras
    // se pone ':'. Por ejemplo, -n requiere un numero detras asi que se pone'n:'
    // Devuelve -1 cuando los ha parseado todos
    while ((options = getopt(argc, argv, "n:o:h")) != -1)
    {
        switch (options)
        {
        case 'h':
            printf("Usage: ./cuestion1 -n <numBytes> [-o <outFileName]\n");
            break;
        case 'n':
            // Para convertir el argumento en un int hay que hacer atoi
            N = atoi(optarg);
            break;
        case 'o':
            readFile = optarg;
            break;
        default:
            break;
        }
    }
    /* Open file */
    // Devuelve error si no ha podido abrir el archivo de lectura
    if ((file = fopen(readFile, "r")) == NULL)
        err(2, "The input file %s could not be opened", argv[1]);

    // Para comprobar el tipo de fichero
    struct stat statbuf;
    if (lstat(readFile, &statbuf) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
    if ((statbuf.st_mode & __S_IFMT) != __S_IFREG)
    {
        printf("Formato del fichero equivocado\n");
        exit(EXIT_FAILURE);
    }
    char *c;
    c = (char *)malloc(sizeof(char));
    int charactersRead = 0;
    while (charactersRead < N && (fread(c, 1, 1, file) != 0))
    {
        // Escritura del archivo con fwrite en lugar de putc
        // fwrite escribe la cantidad de chars del tamaño solicitado que se le pasan del puntero c en la salida
        //(en este caso stdout->consola)
        int ret = fwrite(c, 1, 1, stdout);
        if (ret == 0)
        {
            fclose(file);
            err(3, "putc() failed!!");
        }
        charactersRead++;
    }
    return 0;
}
