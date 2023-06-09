#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAXPROC 16
int inde = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[MAXPROC] = {PTHREAD_COND_INITIALIZER};
int tareashechas[MAXPROC] = {0};
struct task
{
    int valid;         // booleano, indica si la entrada es válida (1) o no (0)
    int next[MAXPROC]; // array de booleanos para sucesores: next[i]=1 si sale una arista hacia el nodo i
    int id;            // identificador/nombre de la tarea, que corresponde a su posición en el array de nodos
    int sizePredecesor;
    int predecesors[MAXPROC];
};
struct task tasks[MAXPROC];

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
    printf("termine");
}
int comprueba(int id)
{
    // printf("%d ",id);
    for (int i = 0; i < MAXPROC; i++)
    {
        // printf("%d,",tasks[id].predecesors[i]);
        if (tasks[id].predecesors[i] == 1) // tiene un redecesor
        {
            if (tareashechas[i] == 0)
                return 0;
        }
        // if(tareashechas[tasks[id].predecesors[i]]==0&&tasks[i].valid==1)return 0;
    }
    return 1;
}
void wait_for_predecesors(int id)
{
    // printf("soy el nodo %d ->",id);
    pthread_mutex_lock(&mutex);
    while (comprueba(id) == 0)
    {
        pthread_cond_wait(&cond[id], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}
void notify_successors(int id)
{
    pthread_mutex_lock(&mutex);
    tareashechas[id] = 1;
    for (int i = 0; i < MAXPROC; i++)
    {
        if (tasks[id].next[i] == 1)
        {
            pthread_cond_signal(&cond[i]);
        }
    }

    pthread_mutex_unlock(&mutex);
}
void *task_body(void *arg)
{
    int id = *(int *)arg;
    // printf("heyeyeh\n");
    wait_for_predecesors(id);        // bloqueante, el hilo espera a que sus predecesores le notifiquen su finalización
    printf("Task %d running\n", id); // cuerpo real de la tarea
    notify_successors(id);           // sincronización, aviso a los sucesores de que esta tarea ha terminado
    return NULL;
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        perror("error de argumetos");
        return -1;
    }
    FILE *fil;
    if ((fil = fopen(argv[1], "r")) == NULL)
    {
        perror("error al abrir el fichero");
        return -1;
    }

    for (int i = 0; i < MAXPROC; i++)
    {
        tasks[i].valid = 0;

        for (int j = 0; j < MAXPROC; j++)
        {
            tasks[i].next[j] = 0;
            tasks[i].predecesors[j] = -1;
        }
    }

    int num, size;
    char token;
    int n = 0;

    while (fscanf(fil, "%d,%d", &num, &size) != EOF)
    {

        tasks[num].sizePredecesor = size;
        tasks[num].valid = 1;
        tasks[num].id = num;
        int number;

        for (int i = 0; i < size; i++)
        {
            fscanf(fil, ",%d", &number);
            tasks[num].predecesors[number] = 1;
            tasks[number].valid = 1;
            tasks[number].next[num] = 1;
            tasks[number].id = number;
        }
        // lecutra de /n
        char aux;
        fread(&aux, sizeof(char), 1, fil);

        n++;
    }
    print_graph(tasks, MAXPROC);
    for (int i = 0; i < MAXPROC; i++)
        pthread_cond_init(&cond[i], NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_t *pt = malloc(sizeof(pthread_t) * n);
    for (int i = 0; i < n; i++)
    {
        // printf("holaaa");
        pthread_create(&pt[i], NULL, task_body, &tasks[i].id);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(pt[i], NULL);
    }
}
