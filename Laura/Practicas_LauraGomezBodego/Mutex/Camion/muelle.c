#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NP 10 // Capacidad m´axima de cami´on
#define NWORKERS 5
#define NTRUCKS 3
int trucksInAlley, workersLoading;
int cargaCamion;
pthread_mutex_t m, mTruck;

pthread_cond_t salir, cargar;

// Mensaje en consola de que se está cogiendo un paquete(worker)
void get_parcel(int id)
{
    printf("Trabajador %d cogiendo paquete...\n", id);
    sleep((rand() % 3) + 1);
}
// Mensaje en consola de que se está cargando un paquete(worker)
void load_parcel(int id)
{
    cargaCamion++;
    printf("Trabajador %d cargando paquete... paquetes cargados,%d\n", id, cargaCamion);
    sleep((rand() % 3) + 1);
}

void enter_dock(int id)
{
    printf("Camión %d entrando en el muelle...\n", id);
}

void delivery(int id)
{
    printf("Camión %d repartiendo paquetes...\n", id);
    sleep((rand() % 3) + 1);
}
void *truck(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        // esperar a poder entrar
        pthread_mutex_lock(&m);
        while (trucksInAlley != 0)
        {
            pthread_cond_wait(&cargar, &m);
        }
        trucksInAlley++;
        enter_dock(id);
        pthread_cond_broadcast(&cargar);
        pthread_mutex_unlock(&m);
        // esperar a camión cargado
        pthread_mutex_lock(&mTruck);
        while (cargaCamion < NP)
        {
            printf("Camion %d esperando paquete...\n", id);
            pthread_cond_wait(&salir, &mTruck);
        }
        trucksInAlley--;

        pthread_mutex_lock(&m);
        cargaCamion = 0;
        pthread_mutex_unlock(&m);

        pthread_cond_broadcast(&cargar);
        // Desbloqueo ambos mutex, ahora pueden entrar camiones nuevos
        pthread_mutex_unlock(&mTruck);
        // pthread_mutex_unlock(&mEnter);
        delivery(id);
    }
}
void *worker(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        // Coge paquete
        get_parcel(id);

        pthread_mutex_lock(&m);
        // Espera a que haya camiones y no haya nadie cargando
        while (trucksInAlley == 0 || cargaCamion == NP)
            pthread_cond_wait(&cargar, &m);

        // Añade un trabajador, carga el paquete
        load_parcel(id);
        // Si se ha llenado al máximo avisa al camión para que salga, si no avisa al resto de trabajadores
        // para que sigan cargando paquetes
        if (cargaCamion < NP)
            pthread_cond_broadcast(&cargar);
        else
            pthread_cond_signal(&salir);

        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[])
{
    pthread_t workers[NWORKERS];
    pthread_t trucks[NTRUCKS];
    trucksInAlley = 0;
    int idW[NWORKERS];
    int idT[NTRUCKS];
    for (int i = 0; i < NWORKERS; i++)
    {
        idW[i] = i;
        pthread_create(&workers[i], NULL, worker, &idW[i]);
    }
    for (int i = 0; i < NTRUCKS; i++)
    {
        idT[i] = i;
        pthread_create(&trucks[i], NULL, truck, &idT[i]);
    }
    for (int i = 0; i < NWORKERS; i++)
    {
        pthread_join(workers[i], NULL);
    }
    for (int i = 0; i < NTRUCKS; i++)
    {
        pthread_join(trucks[i], NULL);
    }

    return 0;
}