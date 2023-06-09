#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#define MAXPROC 16

struct task
{
    int valid;         // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;            // identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
};

void print_graph(struct task tasks[], int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        if (!tasks[i].valid)
            continue;
        printf("%d: ", tasks[i].id);
        for (j = 0; j < n; j++)
            if (tasks[i].next[j])
                printf("%d ", j);
        printf("\n");
    }
}
int main(int argc, char *argv[])
{
    struct task tasks[MAXPROC];

    // inicializa todo a cero
    for (int i = 0; i < MAXPROC; i++)
    {
        tasks[i].valid = 0;
        for (int j = 0; j < MAXPROC; j++)
        {
            tasks[i].next[j] = 0;
        }
    }

    FILE *file = NULL;
    int *c;
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

    char *token;
    token = (char *)malloc(sizeof(char) * MAXPROC);
    char *aux;
    aux = (char *)malloc(sizeof(char));
    int i = 0;
    int numParents;
    int padre;
    int j;
    while ((aux = fgets(token, MAXPROC + 1, file)) != NULL)
    {
        // variable auxiliar porque strsep hace cosas raras con el malloc
        char *lineaAux = strdup(token);
        tasks[i].id = atoi(strsep(&lineaAux, ","));
        tasks[i].valid = 1;
        numParents = atoi(strsep(&lineaAux, ","));
        for (j = 0; j < numParents; j++)
        {
            padre = atoi(strsep(&lineaAux, ","));
            tasks[padre].next[tasks[i].id] = 1;
        }
        i++;
        free(lineaAux);
    }
    print_graph(tasks, i);
    free(token);
    free(aux);
    return 0;
}