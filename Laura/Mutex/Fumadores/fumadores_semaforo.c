#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int ingredientes[3];
sem_t ag;
sem_t faltan[3];
int ingAgente[2];

typedef enum
{
    PAPEL,
    TABACO,
    CERILLA
} info_fumar;

void *fumador(void *argc)
{
    while (1)
    {
        int ingrediente = *(int *)argc;
        int i1, i2;
        i1 = (ingrediente + 1) % 3;
        i2 = (ingrediente + 2) % 3;
        sem_wait(&faltan[ingrediente]);
        printf("Fumador con ingrediente %d fumando\n", ingrediente);
        sleep(2);
        sem_post(&ag);
    }
}
void *agente(void *argc)
{
    while (1)
    {
        // Sacar ingrediente aleatorio, avisar de que hay ingredientes
        sem_wait(&ag);
        int i1, i2, sobrante;
        i1 = rand() % 3;
        i2 = (i1 + 1) % 3;
        printf("Ingredientes: %d y %d en la mesa\n", i1, i2);
        sobrante = (i1 + 2) % 3;
        sem_post(&faltan[sobrante]);
    }
}
int main(int argc, char *argv[])
{
    pthread_t fumadores[3];
    pthread_t hiloAgente;
    int idFumador[3];
    sem_init(&ag, 0, 1);
    for (int i = 0; i < 3; i++)
    {
        sem_init(&faltan[i], 0, 0);
    }

    ingAgente[0] = 0;
    ingAgente[1] = 0;
    for (int i = 0; i < 3; i++)
    {
        idFumador[i] = i;
        pthread_create(&fumadores[i], NULL, fumador, &idFumador[i]);
    }
    pthread_create(&hiloAgente, NULL, agente, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(fumadores[i], NULL);
    }
    pthread_join(hiloAgente, NULL);

    return 0;
}