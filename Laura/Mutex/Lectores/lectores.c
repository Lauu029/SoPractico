#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NESCRITORES 2
#define NLECTORES 4

int recurso;

pthread_mutex_t m;
pthread_cond_t escribe;

int lectores;

typedef struct
{
    int id;
    int miRecurso;
} info_escritor;
void leer(int id)
{
    printf("Lector %d leyendo %d ...\n", id, recurso);
}
void *thread_escritores(void *args)
{
    info_escritor *escritor = (info_escritor *)args;
    int miRecurso = escritor->miRecurso;
    int id = escritor->id;
    printf("Entrando escritor %d\n", id);
    while (1)
    {
        pthread_mutex_lock(&m);
        while (lectores != 0)
            pthread_cond_wait(&escribe, &m);

        recurso += miRecurso;
        printf("Escritor %d escribiendo %d, escritura total: %d\n", id, miRecurso, recurso);
        pthread_mutex_unlock(&m);
        sleep(1);
    }
}

void *thread_lectores(void *args)
{
    int id;
    id = *(int *)args;
    printf("Entrando lector %d\n", id);
    while (1)
    {
        pthread_mutex_lock(&m);
        lectores++;
        pthread_mutex_unlock(&m);

        leer(id);

        pthread_mutex_lock(&m);
        lectores--;
        pthread_cond_signal(&escribe);
        pthread_mutex_unlock(&m);

        sleep(2);
    }
}

int main(int argc, char *argv[])
{
    pthread_t hilosLectores[NLECTORES];
    pthread_t hilosEscritores[NESCRITORES];
    info_escritor escritores[NESCRITORES];
    int id[NLECTORES];
    for (int i = 0; i < NLECTORES; i++)
    {
        id[i] = i;
        pthread_create(&hilosLectores[i], NULL, thread_lectores, &id[i]);
    }
    for (int i = 0; i < NESCRITORES; i++)
    {
        escritores[i].id = i;
        escritores[i].miRecurso = i + 3;
        pthread_create(&hilosEscritores[i], NULL, thread_escritores, &escritores[i]);
    }
    for (int i = 0; i < NLECTORES; i++)
    {
        pthread_join(hilosLectores[i], NULL);
    }
    for (int i = 0; i < NESCRITORES; i++)
    {
        pthread_join(hilosLectores[i], NULL);
    }
    return 0;
}