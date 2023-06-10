#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
void write_characters(int N, FILE *output)
{
    struct dirent *dirInfo;
    struct stat statbuf;
    DIR *d;
    FILE *regularFile;
    d = opendir(".");
    char *c;
    c = malloc(sizeof(char));
    int i = 0;
    // Leo el directorio hasta el final del archivo
    while ((dirInfo = readdir(d)) != NULL)
    {
        // Comparo el string del nombre con . y .. para ver si hay que ignorar el archivo
        if ((strcmp(dirInfo->d_name, ".") != 0) & (strcmp(dirInfo->d_name, "..") != 0))
        {
            lstat(dirInfo->d_name, &statbuf);
            switch (statbuf.st_mode & __S_IFMT)
            {
            case __S_IFREG:
                if ((regularFile = fopen(dirInfo->d_name, "r")) == NULL)
                    err(2, "The input file %s could not be opened", dirInfo->d_name);
                i = 0;
                while ((fread(c, 1, 1, regularFile) != 0) && i < N)
                {
                    // Escritura del archivo con fwrite en lugar de putc
                    // fwrite escribe la cantidad de chars del tamaño solicitado que se le pasan del puntero c en la salida
                    //(en este caso stdout->consola)
                    int ret = fwrite(c, 1, 1, output);
                    if (ret == 0)
                    {
                        fclose(regularFile);
                        err(3, "fwrite failed!!");
                    }
                    i++;
                }
                fclose(regularFile);
                break;
            default:
                break;
            }
        }
    }
    free(c);
    closedir(d);
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
    // Nombre del documento que hay que leer
    FILE *writeFile = stdout;

    while ((options = getopt(argc, argv, "n:o")) != -1)
    {
        switch (options)
        {
        case 'n':
            N = atoi(optarg);
            break;
        case 'o':
            if ((writeFile = fopen(optarg, "r")) == NULL)
                err(2, "The input file %s could not be opened", argv[1]);
            break;
        default:
            break;
        }
    }
    write_characters(N, writeFile);

    return 0;
}