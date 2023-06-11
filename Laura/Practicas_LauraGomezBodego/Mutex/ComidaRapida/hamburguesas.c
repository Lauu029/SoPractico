#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NCLIENTES 10
#define N 5
#define HAMBURGUESA 0
#define PERRITO 1
pthread_mutex_t m;
pthread_cond_t cocina[2];
pthread_cond_t come[2];

int tipoComida[2];

void conseguirPlato(int id, int tipoPlato)
{
    pthread_mutex_lock(&m);
    while (tipoComida[tipoPlato] == 0)
    {
        pthread_cond_broadcast(&cocina[tipoPlato]);
        pthread_cond_wait(&come[tipoPlato], &m);
    }
    tipoComida[tipoPlato]--;
    pthread_mutex_unlock(&m);
}
void comer(int id, int tipoPlato)
{
    if (tipoPlato)
        printf("Cliente %d comiendo PERRITO...\n", id);
    else
        printf("Cliente %d comiendo HAMBURGUESA...\n", id);
    sleep((rand() % 5) + 1);
}
void *Cliente(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        int tipoPlato = rand() % 2;
        conseguirPlato(id, tipoPlato);
        comer(id, tipoPlato);
    }
}

void servirHamburguesa()
{
    pthread_mutex_lock(&m);
    while (tipoComida[HAMBURGUESA] > 0)
        pthread_cond_wait(&cocina[HAMBURGUESA], &m);
    tipoComida[HAMBURGUESA] = N;
    printf("\nCocinero preparando hamburguesas\n\tDisponibles:%d\n\n", tipoComida[HAMBURGUESA]);
    pthread_cond_broadcast(&come[HAMBURGUESA]);

    pthread_mutex_unlock(&m);
}

void servirPerrito()
{
    pthread_mutex_lock(&m);
    while (tipoComida[PERRITO] > 0)
        pthread_cond_wait(&cocina[PERRITO], &m);
    tipoComida[PERRITO] = N;
    printf("\nCocinero preparando perritos...\n\tDisponibles:%d\n\n", tipoComida[PERRITO]);
    pthread_cond_broadcast(&come[PERRITO]);
    pthread_mutex_unlock(&m);
}
void *Cocinero_hamburguesa(void *arg)
{
    while (1)
    {
        servirHamburguesa();
    }
}
void *Cocinero_perrito(void *arg)
{
    while (1)
    {
        servirPerrito();
    }
}
int main(int argc, char *argv[])
{
    pthread_t hilosClientes[NCLIENTES];
    int idClientes[NCLIENTES];
    // Crear el thread Autobus
    pthread_t cocineros[4];

    tipoComida[HAMBURGUESA] = 0;
    tipoComida[PERRITO] = 0;

    // Crea a los cocineros de perritos y hamburguesas
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&cocineros[i], NULL, Cocinero_hamburguesa, NULL);
    }
    for (int i = 2; i < 4; i++)
    {
        pthread_create(&cocineros[i], NULL, Cocinero_perrito, NULL);
    }
    for (int i = 0; i < NCLIENTES; i++)
    {
        idClientes[i] = i;
        pthread_create(&hilosClientes[i], NULL, Cliente, &idClientes[i]);
    }

    for (int i = 0; i < NCLIENTES; i++)
    {
        pthread_join(hilosClientes[i], NULL);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(cocineros[i], NULL);
    }

    return 0;
}