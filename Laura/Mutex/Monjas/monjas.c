#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 10

int ticket, turno, numSentadas;

pthread_mutex_t m;
pthread_cond_t come, sal;

int entrar_comedor_y_sentarse(int id)
{
    pthread_mutex_lock(&m);
    int miTurno = ticket;
    ticket = (ticket + 1) % N;
    numSentadas++;
    // Avisa de que se ha sentado
    pthread_cond_broadcast(&come);
    printf("Monja %d sentada, hay %d monjas sentadas\n", id, numSentadas);
    while (numSentadas < N)
        pthread_cond_wait(&come, &m);
    printf("Monja %d comiendo...\n", id);
    pthread_mutex_unlock(&m);
    return miTurno;
}
void salir_comedor(int miTurno, int id)
{
    pthread_mutex_lock(&m);
    while (miTurno != turno)
        pthread_cond_wait(&sal, &m);

    turno = (turno + 1) % N;
    numSentadas--;
    printf("Monja %d que habia llegado la numero %d se ha levantado de la mesa...\n", id, miTurno + 1);
    pthread_cond_broadcast(&sal);
    pthread_mutex_unlock(&m);
}
void *thread_monja(void *args)
{
    int id = *(int *)args;
    int turno_salida;
    while (1)
    {
        //<< labor_en_cuestion >>
        printf("Monja %d haciendo sus labores...\n", id);
        turno_salida = entrar_comedor_y_sentarse(id);
        sleep(2);
        //<< comer >>
        salir_comedor(turno_salida, id);
        sleep(5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t monjas[N];
    int idMonjas[N];
    turno = 0;
    numSentadas = 0;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&sal, NULL);
    pthread_cond_init(&come, NULL);

    for (int i = 0; i < N; i++)
    {
        idMonjas[i] = i;
        pthread_create(&monjas[i], NULL, thread_monja, &idMonjas[i]);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(monjas[i], NULL);
    }
    return 0;
}