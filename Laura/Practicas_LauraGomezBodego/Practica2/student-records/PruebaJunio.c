#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
// struct de opciones para la lectura del fichero y ejecucion del programa
typedef struct
{
    FILE *file;
    int mode;
    int search;
    int fileMode;
    int idFind;

} options_t;
// enum para las opciones del programa
typedef enum
{
    CREATE_MODE,
    LIST_MODE,
    SEARCH_MODE
} output_t;

typedef enum
{
    ID_SEARCH,
    NIF_SEARCH
} search_t;

typedef enum
{
    CREATE,
    ADDEXISTING
} fileMode_t;
/*
    coge un string caracter a caracter hasta que llega a '\0'
    resetea el puntero del archivo con la longitud que ha leído
    reserva el espacio en memoria
    lee los caracteres correspondientes y devuelve el array de chars creado
*/
char *loadstr(FILE *file)
{
    int nCaracters = 0;
    char *name;
    char token;
    do
    {
        token = getc(file);
        nCaracters++;
    } while (token != '\0');

    name = malloc(nCaracters * sizeof(char));

    // devuelve el puntero al principio de la cadena de caracteres
    fseek(file, -nCaracters, SEEK_CUR);

    fread(name, sizeof(char), nCaracters, file);
    return name;
}
/*
Esta función acepta como parámetro el listado de registros en formato ASCII
pasados como argumentro al programa en la línea de comando(records)
así como el número de registros (nr_records), y devuelve la representación
binaria en memoria de los mismos. Esta representación será un array de estructuras
*/
student_t *parse_records(char *records[], int nr_records)
{
    // Struct que se va a devolver
    student_t *parsedRecords;
    parsedRecords = malloc(sizeof(student_t) * nr_records);
    char *token;
    for (int i = 0; i < nr_records; i++)
    {
        token = strsep(&records[i], ":");
        // sscanf(token, "%d", &parsedRecords[i].student_id);
        parsedRecords[i].student_id = atoi(token);
        token = strsep(&records[i], ":");
        strcpy(parsedRecords[i].NIF, token);
        token = strsep(&records[i], ":");
        parsedRecords[i].first_name = strdup(token);
        token = strsep(&records[i], ":");
        parsedRecords[i].last_name = strdup(token);
    }
    return parsedRecords;
}

int dump_entries(student_t *entries, int nr_entries, FILE *students)
{
    for (int i = 0; i < nr_entries; i++)
    {
        // Comprobacion de si da problemas al escribir las cosas
        if (fwrite(&entries[i].student_id, sizeof(int), 1, students) == 0 ||
            fwrite(entries[i].NIF, strlen(entries[i].NIF), 1, students) == 0 ||
            fwrite(entries[i].first_name, strlen(entries[i].first_name) + 1, 1, students) == 0 ||
            fwrite(entries[i].last_name, strlen(entries[i].last_name) + 1, 1, students) == 0)
        {

            perror("Fallo en la escritura\n");
            return -1;
        }
    }
    return nr_entries;
}

student_t *read_student_file(FILE *students, int *nr_entries)
{
    // Lee la cabecera para saber cuantas entradas tiene que leer

    if (fread(nr_entries, sizeof(int), 1, students) <= 0)
        printf("Error en la lectura de la cabecera\n");
    student_t *infoStudents;
    infoStudents = malloc(sizeof(student_t) * *nr_entries);
    printf("%d nEntries\n", *nr_entries);
    for (int i = 0; i < *nr_entries; i++)
    {
        // lee los dos primeros ints de forma normal
        if (fread(&infoStudents[i].student_id, sizeof(int), 1, students) <= 0)
            printf("Error en la lectura del id\n");
        if (fread(infoStudents[i].NIF, sizeof(char), MAX_CHARS_NIF + 1, students) <= 0)
            printf("Error en la lectura del NIF\n");
        // Para los chars recurro al método anterior loadchar que guarda la memoria
        infoStudents[i].first_name = loadstr(students);
        infoStudents[i].last_name = loadstr(students);
    }
    return infoStudents;
}

int main(int argc, char *argv[])
{
    int opt;
    options_t executeOptions;
    char *fileRoute = NULL;
    executeOptions.file = NULL;
    executeOptions.mode = 0;
    executeOptions.search = 0;
    executeOptions.fileMode = 0;
    /* Parsea las opciones que se han puesto en la linea de comandos */
    // El parámetro optstring sirve para indicar de forma compacta a getopt() cuáles son las opciones que el programa acepta
    while ((opt = getopt(argc, argv, "f:hlcaqi:n:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, "Usage: ./%s -f file [ -h | -l | -c | -a | -q [-i|-n ID]][ list of records ]\n", argv[0]);
            exit(0);
        case 'f':
            fileRoute = optarg;
            break;
        case 'l':
            executeOptions.mode = LIST_MODE;
            break;
        case 'c':
            executeOptions.fileMode = CREATE;
            executeOptions.mode = CREATE_MODE;
            break;
        case 'a':
            executeOptions.fileMode = ADDEXISTING;
            executeOptions.mode = CREATE_MODE;
            break;
        case 'q':
            executeOptions.mode = SEARCH_MODE;
            break;
        case 'i':
            executeOptions.search = ID_SEARCH;
            executeOptions.idFind = atoi(optarg);
            break;
        case 'n':
            executeOptions.search = NIF_SEARCH;
            executeOptions.idFind = atoi(optarg);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // Primero abro el fichero segun lo que se pase por parametro
    switch (executeOptions.fileMode)
    {
    case CREATE:
        executeOptions.file = fopen(fileRoute, "w+");
        break;
    case ADDEXISTING:
        executeOptions.file = fopen(fileRoute, "r+");
        break;
    default:
        executeOptions.file = fopen(fileRoute, "r+");
        break;
    }
    // Parseo de errores al abrir o crear el archivo
    if (executeOptions.file == NULL)
    {
        fprintf(stderr, "No se pudo abrir o crear en archivo%s", fileRoute);
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // Ejecuto el programa según las opciones que se me hayan pedido
    switch (executeOptions.mode)
    {
    // Modo de búsqueda de records
    case SEARCH_MODE:
    {
        // Guardo todos los estudiantes que haya almacenados en un string de students
        int i = 1;
        int nEntries;
        student_t *students;
        students = read_student_file(executeOptions.file, &nEntries);
        printf("%d id\n", executeOptions.idFind);
        switch (executeOptions.search)
        {
        case ID_SEARCH:
            // Busca por los id de todos los records hasta que lo encuentra o llega al final
            while (i < nEntries && students[i].student_id != executeOptions.idFind)
                i++;
            break;
        case NIF_SEARCH:
            while (i < nEntries && atoi(students[i].NIF) != executeOptions.idFind)
                i++;
            break;
        default:
            printf("No se ha facilitado ningun tipo de búsqueda\n");
            break;
        }
        if (i < nEntries)
        {
            printf("[Entry #%d\n", i);
            printf("\tstudent_id = %d\n\tNIF = %s\n\tfirst_name = %s\n\tlast_name = %s\n",
                   students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
        }
        else
            printf("No entry was found\n");
        free(students);
    }
    break;
    // Listado de los alumnos del student records
    case LIST_MODE:
    {
        int nEntries;
        student_t *students;
        students = read_student_file(executeOptions.file, &nEntries);
        for (int i = 0; i < nEntries; i++)
        {
            printf("[Entry #%d\n", i);
            printf("\tstudent_id = %d\n\tNIF = %s\n\tfirst_name = %s\n\tlast_name = %s\n",
                   students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
        }
        free(students);
    }
    break;
    // Inserta nuevos datos en el archivo
    case CREATE_MODE:
        if (executeOptions.fileMode == CREATE)
        {

            // Primero, si el archivo está vacío, pone arriba el número de records que se van a insertar
            int num = argc - optind; // Numero de argumentos menos opciones de ejecucion
            fseek(executeOptions.file, 0, SEEK_SET);
            fwrite(&(num), sizeof(int), 1, executeOptions.file);
            // Escribe a todos los estudiantes que se le vayan pasando
            student_t *students;
            students = parse_records(&argv[optind], argc - optind);
            int nRecords = dump_entries(students, argc - optind, executeOptions.file);
            printf("%d records written succesfully!\n", nRecords);
            free(students);
        }
        else
        {
            // Similar al anterior pero comprobando si hay repetidos
            int nStudents;
            int num = argc - optind;
            // Control de elementos duplicados
            // int dup = 0;
            // Para ver cuantos archivos se han introducido nuevos
            int new = 0;
            // Para comprobar los repetidos
            student_t *students;
            // Para añadir los nuevos
            student_t *newStudents = parse_records(&argv[optind], num);
            students = read_student_file(executeOptions.file, &nStudents);
            // printf("ID1 : %d ID2 : %d\n", newStudents[0].student_id, students[0].student_id);
            // printf("%d newstudents, %d students\n", num, nStudents);
            fseek(executeOptions.file, 0, SEEK_END);
            // for (int i = 0; i < num; i++)
            // {
            //     dup = 0;
            //     for (int j = 0; j < nStudents; j++)
            //     {
            //         // Comparamos los ID porque son únicos de cada estudiante
            //         if (newStudents[i].student_id == students[j].student_id)
            //         {
            //             dup = 1;
            //             printf("Found duplicate student_id %d\n", students[i].student_id);
            //         }
            //     }
            //     // Si no hay ningun duplicado se añade el record
            //     if (dup == 0)
            //     {
            //         dump_entries(&newStudents[i], 1, executeOptions.file);
            //         new ++;
            //     }
            // }
            fseek(executeOptions.file, 0, SEEK_SET);
            nStudents += num;
            fwrite(&nStudents, sizeof(int), 1, executeOptions.file);
            dump_entries(newStudents, num, executeOptions.file);
            if (new)
                printf("%d records written succesfully!\n", new);
            printf("%d\n", nStudents);
            // Se pone el puntero al inicio para escribir el numero de records del fichero
            free(newStudents);
            free(students);
        }
        break;
    }
    // Cierro el archivo
    fclose(executeOptions.file);
}
