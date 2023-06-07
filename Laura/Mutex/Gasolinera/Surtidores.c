#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NCLIENTES 10
#define NSURTIDORES 2
// Una condicion por surtidor
pthread_cond_t surtidorLibre;

pthread_mutex_t m;
// Tickets/turnos
int ticket, turno;
int surtidores[NSURTIDORES];

int getSurtidorLibre()
{
    if (surtidores[0] == 0)
        return 0;

    if (surtidores[1] == 0)
        return 1;

    return -1;
}
int getUnusedPump(int * myticket)
{
    pthread_mutex_lock(&m);
    *myticket = ticket;
    ticket++;
    int libre = -1;
    while (*myticket != turno || (libre = getSurtidorLibre()) == -1)
        pthread_cond_wait(&surtidorLibre, &m);
    turno++;
    surtidores[libre] = 1;
    // mando broadcast por si hay algun surtidor más libre
    pthread_cond_broadcast(&surtidorLibre);
    pthread_mutex_unlock(&m);
    return libre;
}
void PumpFuel(int pump, int id)
{
    printf("Cliente %d cogiendo gasolina del surtidor %d\n", id, pump);
    sleep((rand() % 3) + 1);
}

void releasePump(int pump)
{
    pthread_mutex_lock(&m);
    surtidores[pump] = 0;
    printf("Surtidor %d libre\n", pump);

    pthread_cond_broadcast(&surtidorLibre);
    pthread_mutex_unlock(&m);
}
void *cliente(void *argc)
{
    int pump;
    int myticket=0;
    pump = getUnusedPump(&myticket);
    /* Uso del surtidor */
    PumpFuel(pump, myticket);

    /* Deja de usar el surtidor */
    releasePump(pump);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t clientes[NCLIENTES];
    ticket = 0;
    turno = 0;
    surtidores[0] = 0;
    surtidores[1] = 0;
    for (int i = 0; i < NCLIENTES; i++)
    {
        pthread_create(&clientes[i], NULL, cliente, NULL);
    }
    for (int i = 0; i < NCLIENTES; i++)
    {
        pthread_join(clientes[i], NULL);
    }

    return 0;
}