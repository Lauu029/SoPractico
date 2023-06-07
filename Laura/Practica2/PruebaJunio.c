// // #include <stdio.h>
// // #include <unistd.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include "defs.h"
// // // struct de opciones para la lectura del fichero y ejecucion del programa
// // typedef struct
// // {
// //     FILE *file;
// //     int mode;
// //     int search;
// //     int fileMode;
// //     int idFind;

// // } options_t;
// // // enum para las opciones del programa
// // typedef enum
// // {
// //     CREATE_MODE,
// //     LIST_MODE,
// //     SEARCH_MODE
// // } output_t;

// // typedef enum
// // {
// //     ID_SEARCH,
// //     NIF_SEARCH
// // } search_t;

// // typedef enum
// // {
// //     CREATE,
// //     ADDEXISTING
// // } fileMode_t;
// // /*
// //     coge un string caracter a caracter hasta que llega a '\0'
// //     resetea el puntero del archivo con la longitud que ha leído
// //     reserva el espacio en memoria
// //     lee los caracteres correspondientes y devuelve el array de chars creado
// // */
// // char *loadstr(FILE *file)
// // {
// //     int nCaracters = 0;
// //     char *name;
// //     char token;
// //     do
// //     {
// //         token = getc(file);
// //         nCaracters++;
// //     } while (token != '\0');

// //     name = malloc(nCaracters * sizeof(char));

// //     // devuelve el puntero al principio de la cadena de caracteres
// //     fseek(file, -nCaracters, SEEK_CUR);

// //     fread(name, sizeof(char), nCaracters, file);
// //     return name;
// // }
// // /*
// // Esta función acepta como parámetro el listado de registros en formato ASCII
// // pasados como argumentro al programa en la línea de comando(records)
// // así como el número de registros (nr_records), y devuelve la representación
// // binaria en memoria de los mismos. Esta representación será un array de estructuras
// // */
// // student_t *parse_records(char *records[], int nr_records)
// // {
// //     // Struct que se va a devolver
// //     student_t *parsedRecords;
// //     parsedRecords = malloc(sizeof(student_t) * nr_records);
// //     char *token;
// //     for (int i = 0; i < nr_records; i++)
// //     {
// //         token = strsep(&records[i], ":");
// //         // sscanf(token, "%d", &parsedRecords[i].student_id);
// //         parsedRecords[i].student_id = atoi(token);
// //         token = strsep(&records[i], ":");
// //         strcpy(parsedRecords[i].NIF, token);
// //         token = strsep(&records[i], ":");
// //         parsedRecords[i].first_name = strdup(token);
// //         token = strsep(&records[i], ":");
// //         parsedRecords[i].last_name = strdup(token);
// //     }
// //     return parsedRecords;
// // }

// // int dump_entries(student_t *entries, int nr_entries, FILE *students)
// // {
// //     for (int i = 0; i < nr_entries; i++)
// //     {
// //         // Comprobacion de si da problemas al escribir las cosas
// //         if (fwrite(&entries[i].student_id, sizeof(int), 1, students) == 0 ||
// //             fwrite(entries[i].NIF, strlen(entries[i].NIF), 1, students) == 0 ||
// //             fwrite(entries[i].first_name, strlen(entries[i].first_name) + 1, 1, students) == 0 ||
// //             fwrite(entries[i].last_name, strlen(entries[i].last_name) + 1, 1, students) == 0)
// //         {

// //             perror("Fallo en la escritura\n");
// //             return -1;
// //         }
// //     }
// //     return nr_entries;
// // }

// // student_t *read_student_file(FILE *students, int *nr_entries)
// // {
// //     // Lee la cabecera para saber cuantas entradas tiene que leer

// //     if (fread(nr_entries, sizeof(int), 1, students) <= 0)
// //         printf("Error en la lectura de la cabecera\n");
// //     student_t *infoStudents;
// //     infoStudents = malloc(sizeof(student_t) * *nr_entries);
// //     printf("%d nEntries\n", *nr_entries);
// //     for (int i = 0; i < *nr_entries; i++)
// //     {
// //         // lee los dos primeros ints de forma normal
// //         if (fread(&infoStudents[i].student_id, sizeof(int), 1, students) <= 0)
// //             printf("Error en la lectura del id\n");
// //         if (fread(infoStudents[i].NIF, sizeof(char), MAX_CHARS_NIF + 1, students) <= 0)
// //             printf("Error en la lectura del NIF\n");
// //         // Para los chars recurro al método anterior loadchar que guarda la memoria
// //         infoStudents[i].first_name = loadstr(students);
// //         infoStudents[i].last_name = loadstr(students);
// //     }
// //     return infoStudents;
// // }

// // int main(int argc, char *argv[])
// // {
// //     int opt;
// //     options_t executeOptions;
// //     char *fileRoute = NULL;
// //     executeOptions.file = NULL;
// //     executeOptions.mode = 0;
// //     executeOptions.search = 0;
// //     executeOptions.fileMode = 0;
// //     /* Parsea las opciones que se han puesto en la linea de comandos */
// //     // El parámetro optstring sirve para indicar de forma compacta a getopt() cuáles son las opciones que el programa acepta
// //     while ((opt = getopt(argc, argv, "f:hlcaqi:n:")) != -1)
// //     {
// //         switch (opt)
// //         {
// //         case 'h':
// //             fprintf(stderr, "Usage: ./%s -f file [ -h | -l | -c | -a | -q [-i|-n ID]][ list of records ]\n", argv[0]);
// //             exit(0);
// //         case 'f':
// //             fileRoute = optarg;
// //             break;
// //         case 'l':
// //             executeOptions.mode = LIST_MODE;
// //             break;
// //         case 'c':
// //             executeOptions.fileMode = CREATE;
// //             executeOptions.mode = CREATE_MODE;
// //             break;
// //         case 'a':
// //             executeOptions.fileMode = ADDEXISTING;
// //             executeOptions.mode = CREATE_MODE;
// //             break;
// //         case 'q':
// //             executeOptions.mode = SEARCH_MODE;
// //             break;
// //         case 'i':
// //             executeOptions.search = ID_SEARCH;
// //             executeOptions.idFind = atoi(optarg);
// //             break;
// //         case 'n':
// //             executeOptions.search = NIF_SEARCH;
// //             executeOptions.idFind = atoi(optarg);
// //             break;
// //         default:
// //             exit(EXIT_FAILURE);
// //         }
// //     }

// //     // Primero abro el fichero segun lo que se pase por parametro
// //     switch (executeOptions.fileMode)
// //     {
// //     case CREATE:
// //         executeOptions.file = fopen(fileRoute, "w+");
// //         break;
// //     case ADDEXISTING:
// //         executeOptions.file = fopen(fileRoute, "r+");
// //         break;
// //     default:
// //         executeOptions.file = fopen(fileRoute, "r+");
// //         break;
// //     }
// //     // Parseo de errores al abrir o crear el archivo
// //     if (executeOptions.file == NULL)
// //     {
// //         fprintf(stderr, "No se pudo abrir o crear en archivo%s", fileRoute);
// //         perror(NULL);
// //         exit(EXIT_FAILURE);
// //     }

// //     // Ejecuto el programa según las opciones que se me hayan pedido
// //     switch (executeOptions.mode)
// //     {
// //     // Modo de búsqueda de records
// //     case SEARCH_MODE:
// //     {
// //         // Guardo todos los estudiantes que haya almacenados en un string de students
// //         int i = 1;
// //         int nEntries;
// //         student_t *students;
// //         students = read_student_file(executeOptions.file, &nEntries);
// //         printf("%d id\n", executeOptions.idFind);
// //         switch (executeOptions.search)
// //         {
// //         case ID_SEARCH:
// //             // Busca por los id de todos los records hasta que lo encuentra o llega al final
// //             while (i < nEntries && students[i].student_id != executeOptions.idFind)
// //                 i++;
// //             break;
// //         case NIF_SEARCH:
// //             while (i < nEntries && atoi(students[i].NIF) != executeOptions.idFind)
// //                 i++;
// //             break;
// //         default:
// //             printf("No se ha facilitado ningun tipo de búsqueda\n");
// //             break;
// //         }
// //         if (i < nEntries)
// //         {
// //             printf("[Entry #%d\n", i);
// //             printf("\tstudent_id = %d\n\tNIF = %s\n\tfirst_name = %s\n\tlast_name = %s\n",
// //                    students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
// //         }
// //         else
// //             printf("No entry was found\n");
// //         free(students);
// //     }
// //     break;
// //     // Listado de los alumnos del student records
// //     case LIST_MODE:
// //     {
// //         int nEntries;
// //         student_t *students;
// //         students = read_student_file(executeOptions.file, &nEntries);
// //         for (int i = 0; i < nEntries; i++)
// //         {
// //             printf("[Entry #%d\n", i);
// //             printf("\tstudent_id = %d\n\tNIF = %s\n\tfirst_name = %s\n\tlast_name = %s\n",
// //                    students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
// //         }
// //         free(students);
// //     }
// //     break;
// //     // Inserta nuevos datos en el archivo
// //     case CREATE_MODE:
// //         if (executeOptions.fileMode == CREATE)
// //         {

// //             // Primero, si el archivo está vacío, pone arriba el número de records que se van a insertar
// //             int num = argc - optind; // Numero de argumentos menos opciones de ejecucion
// //             fseek(executeOptions.file, 0, SEEK_SET);
// //             fwrite(&(num), sizeof(int), 1, executeOptions.file);
// //             // Escribe a todos los estudiantes que se le vayan pasando
// //             student_t *students;
// //             students = parse_records(&argv[optind], argc - optind);
// //             int nRecords = dump_entries(students, argc - optind, executeOptions.file);
// //             printf("%d records written succesfully!\n", nRecords);
// //             free(students);
// //         }
// //         else
// //         {
// //             // Similar al anterior pero comprobando si hay repetidos
// //             int nStudents;
// //             int num = argc - optind;
// //             // Control de elementos duplicados
// //             // int dup = 0;
// //             // Para ver cuantos archivos se han introducido nuevos
// //             int new = 0;
// //             // Para comprobar los repetidos
// //             student_t *students;
// //             // Para añadir los nuevos
// //             student_t *newStudents = parse_records(&argv[optind], num);
// //             students = read_student_file(executeOptions.file, &nStudents);
// //             // printf("ID1 : %d ID2 : %d\n", newStudents[0].student_id, students[0].student_id);
// //             // printf("%d newstudents, %d students\n", num, nStudents);
// //             fseek(executeOptions.file, 0, SEEK_END);
// //             // for (int i = 0; i < num; i++)
// //             // {
// //             //     dup = 0;
// //             //     for (int j = 0; j < nStudents; j++)
// //             //     {
// //             //         // Comparamos los ID porque son únicos de cada estudiante
// //             //         if (newStudents[i].student_id == students[j].student_id)
// //             //         {
// //             //             dup = 1;
// //             //             printf("Found duplicate student_id %d\n", students[i].student_id);
// //             //         }
// //             //     }
// //             //     // Si no hay ningun duplicado se añade el record
// //             //     if (dup == 0)
// //             //     {
// //             //         dump_entries(&newStudents[i], 1, executeOptions.file);
// //             //         new ++;
// //             //     }
// //             // }
// //             fseek(executeOptions.file, 0, SEEK_SET);
// //             nStudents += num;
// //             fwrite(&nStudents, sizeof(int), 1, executeOptions.file);
// //             dump_entries(newStudents, num, executeOptions.file);
// //             if (new)
// //                 printf("%d records written succesfully!\n", new);
// //             printf("%d\n", nStudents);
// //             // Se pone el puntero al inicio para escribir el numero de records del fichero
// //             free(newStudents);
// //             free(students);
// //         }
// //         break;
// //     }
// //     // Cierro el archivo
// //     fclose(executeOptions.file);
// // }

/////----------------CÓDIGO ENERO---------------------------
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>

char *loadstr(FILE *file)
{
    char aux;
    int cont = 0;

    do
    {
        fread(&aux, sizeof(char), 1, file);
        cont++;
    } while (aux != '\0');
    char *salida = malloc(cont);
    fseek(file, -cont, SEEK_CUR);
    fread(salida, sizeof(char), cont, file);
    return salida;
}

student_t *parse_records(char *records[], int nr_records)
{
    student_t *binaryStudents = (student_t *)malloc(nr_records * sizeof(student_t));

    char *aux;
    for (int i = 0; i < nr_records; i++)
    {
        // mejor strsep porque devuelve la cadena y luego le escribe \0 directamente, le pasa & a records porque es el contenido del puntero

        aux = strsep(&records[i], ":");
        binaryStudents[i].student_id = atoi(aux);

        aux = strsep(&records[i], ":");
        strcpy(binaryStudents[i].NIF, aux);

        aux = strsep(&records[i], ":");
        binaryStudents[i].first_name = aux;

        aux = strsep(&records[i], ":");
        binaryStudents[i].last_name = aux;
    }
    return binaryStudents;
}

int dump_entries(student_t *entries, int nr_entries, FILE *students)
{
    for (int i = 0; i < nr_entries; i++)
    {
        fwrite(&entries[i].student_id, sizeof(int), 1, students);
        // suma 1 a los tamaños para tener en cuenta el '\0'
        fwrite(entries[i].NIF, strlen(entries[i].NIF) + 1, 1, students);
        fwrite(entries[i].first_name, strlen(entries[i].first_name) + 1, 1, students);
        fwrite(entries[i].last_name, strlen(entries[i].last_name) + 1, 1, students);
    }
    return 0;
}

student_t *read_student_file(FILE *students, int *nr_entries)
{
    fread(nr_entries, sizeof(int), 1, students);
    student_t *readStudents = (student_t *)malloc(*nr_entries * sizeof(student_t));
    for (int i = 0; i < *nr_entries; i++)
    {
        fread(&readStudents[i].student_id, sizeof(int), 1, students);
        strcpy(readStudents[i].NIF, loadstr(students));
        readStudents[i].first_name = loadstr(students);
        readStudents[i].last_name = loadstr(students);
    }
    return readStudents;
}

int main(int argc, char *argv[])
{
    int opt;
    FILE *alumnos = NULL;
    char *name;
    // Lee los flags que se le meten por consola
    while ((opt = getopt(argc, argv, "hlq:f:c:a:i:n:")) != -1)
    {

        if (opt == 'h')
        {
            fprintf(stderr, "Usage:%s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n", argv[0]);
            exit(0);
        }
        else if (opt == 'f')
        {
            name = optarg;
        }
        else if (opt == 'l')
        {
            // imprime el archivo de estudiantes
            if ((alumnos = fopen(name, "r")) == NULL)
            {
                fprintf(stderr, "Non existing file\n");
                exit(1);
            }
            int nr_students = 0;
            student_t *Students = read_student_file(alumnos, &nr_students);
            for (int i = 0; i < nr_students; i++)
            {
                printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n", i,
                       Students[i].student_id, Students[i].NIF, Students[i].first_name, Students[i].last_name);
            }
            free(Students);
            free(alumnos);
            exit(0);
        }
        else if (opt == 'c')
        {
            // crea el archivo de estudiantes
            if ((alumnos = fopen(name, "w")) == NULL)
            {
                exit(1);
            }
            // parsear los alumnos que se le pasan por argv
            int nalumnos = argc - (optind - 1);
            char **nuevosAlumnos = malloc(sizeof(char *) * nalumnos);
            for (int i = 0; i < nalumnos; i++)
            {
                nuevosAlumnos[i] = argv[i + optind - 1];
            }
            student_t *s = parse_records(nuevosAlumnos, nalumnos);
            // Escribe los alumnos en un archivo
            fwrite(&nalumnos, sizeof(int), 1, alumnos);
            dump_entries(s, nalumnos, alumnos);

            printf("%d records written succesfully\n", nalumnos);

            // free
            free(s);
            free(nuevosAlumnos);
            fclose(alumnos);
            exit(0);
        }

        else if (opt == 'a')
        {
            // añade un nuevo estudiante
            if ((alumnos = fopen(name, "r")) == NULL)
            {
                perror("error en la lectura");
                exit(-1);
            }
            // lee a los estudiantes que ya están
            int num_entries = 0;
            student_t *st = read_student_file(alumnos, &num_entries);

            // lee la entrada
            int nalumnos = argc - (optind - 1);
            char **nuevosAlumnos = malloc(sizeof(char *) * nalumnos);
            for (int i = 0; i < nalumnos; i++)
            {
                nuevosAlumnos[i] = argv[i + optind - 1];
            }
            student_t *s = parse_records(nuevosAlumnos, nalumnos);
            // Comprueba si existen y si no  los añade
            int nuevosAlumnosDentro = num_entries;
            for (int i = 0; i < nalumnos; i++)
            {
                int j = 0;
                while (j < num_entries && s[i].student_id != st[j].student_id)
                    j++;

                if (j == num_entries)
                {
                    nuevosAlumnosDentro++;
                    dump_entries(&s[i], 1, alumnos);
                }
                else
                    printf("Found duplicate student_id %d\n", s[i].student_id);
            }
            printf("%d records written succesfully\n", nuevosAlumnosDentro - num_entries);

            // write new number of entries
            fseek(alumnos, 0, SEEK_SET);
            fwrite(&nuevosAlumnosDentro, sizeof(int), 1, alumnos);

            // free
            free(st);
            fclose(alumnos);
            exit(0);
        }
        else if (opt == 'q')
        {
            // busca un estudiante por numero de registro
            if ((alumnos = fopen(name, "r")) == NULL)
            {
                perror("error en la lectura");
                exit(-1);
            }
            if (opt == 'i')
            {
                //
                if (alumnos == NULL)
                {
                    fprintf(stderr, "Non existing file\n");
                    exit(1);
                }
                int student_id;
                sscanf(optarg, "%d", &student_id);

                // read students already existing
                int num_entries = 0;
                student_t *students = read_student_file(alumnos, &num_entries);

                // check if anyone has id = student_id
                int i = 0;
                while (i < num_entries && student_id != students[i].student_id)
                    i++;

                // no entry found
                if (i == num_entries)
                    printf("No entry was found\n");
                else
                    printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                           i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);

                // free
                fclose(alumnos);
                free(students);

                exit(0);
            }
            else if (opt == 'n')
            {
                //
                if (alumnos == NULL)
                {
                    fprintf(stderr, "Non existing file\n");
                    exit(1);
                }

                char *NIF;
                NIF = optarg;

                // read students already existing
                int num_entries = 0;
                student_t *students = read_student_file(alumnos, &num_entries);

                // check if anyone has NIF = NIF
                int i = 0;
                while (i < num_entries && strcmp(NIF, students[i].NIF) != 0)
                    i++;

                // no entry found
                if (i == num_entries)
                    printf("No entry was found\n");
                else
                    printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                           i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);

                // free
                fclose(alumnos);
                free(students);

                exit(0);
            }
        }
    }
    return 0;
}