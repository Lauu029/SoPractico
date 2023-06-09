#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXPROC 16

pthread_cond_t c[MAXPROC];
pthread_mutex_t m;

int nNodos;
int barrera;
struct task
{
    int valid;         // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;            // identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
    int numParents;    // Para saber en el ejercicio de los hilos cuantos padres tiene una tarea
};
struct task tasks[MAXPROC];

void wait_for_predecesors(int id)
{
    pthread_mutex_lock(&m);
    while (tasks[id].numParents > 0)
    {
        pthread_cond_wait(&c[id], &m);
    }
}
void notify_successors(int id)
{
    // señaliza a todos los hijos que se debej ejecutar
    for (int i = 0; i < MAXPROC; i++)
    {
        if (tasks[id].next[i])
        {
            pthread_cond_signal(&c[i]);
            tasks[i].numParents--;
        }
    }
    pthread_mutex_unlock(&m);
}
void *task_body(void *arg)
{
    int id = *(int *)arg;
    wait_for_predecesors(id);        // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %d running\n", id); // cuerpo real de la tarea
    notify_successors(id);           // sincronización, aviso a los sucesores de que esta tarea ha terminado

    return NULL;
}

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
    // int charOriginal;
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
    int nNodos = 0; // numero de nodos del grafo
    int padre;
    int j;
    while ((aux = fgets(token, MAXPROC + 1, file)) != NULL)
    {
        // variable auxiliar porque strsep hace cosas raras con el malloc
        char *lineaAux = strdup(token);
        tasks[nNodos].id = atoi(strsep(&lineaAux, ","));
        tasks[nNodos].valid = 1;
        tasks[nNodos].numParents = atoi(strsep(&lineaAux, ","));
        for (j = 0; j < tasks[nNodos].numParents; j++)
        {
            padre = atoi(strsep(&lineaAux, ","));
            tasks[padre].next[tasks[nNodos].id] = 1;
        }
        nNodos++;
        free(lineaAux);
    }
    print_graph(tasks, nNodos);
    free(token);
    free(aux);

    // Parte de los hilos

    pthread_t nodos[nNodos];
    for (int i = 0; i < nNodos; i++)
    {
        pthread_create(&nodos[i], NULL, task_body, &tasks[i].id);
        // // Inicializo un semáforo por tarea
        // int valSemaforo = 1 - tasks[i].numParents;
        // sem_init(&s[i], 0, valSemaforo);

        // printf("\n%d: hilo num %d, numero de padres %d, valor del semaforo %d\n\n", i, tasks[i].id, tasks[i].numParents, valSemaforo);
    }
    for (int i = 0; i < nNodos; i++)
    {
        pthread_join(nodos[i], NULL);
    }

    return 0;
}