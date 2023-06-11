#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NJUGADORES 4
#define MAXREBOTE 70
#define TOPEPUNTOS 100
// typedef struct jugador;
typedef struct jugador
{
    int dorsal;     // dorsal (único) del jugador
    int eficTiro;   // entero entre 1 y 100 que determina su efectividad en el tiro
    int eficRebote; // entero entre 1 y 100 que determina su efectividad en el rebote
    int puntos;     // puntos conseguidos durante el entrenamiento
} jugador;

// POSESION -> un jugador tiene el balon. ENAIRE -> el balón se ha lanzado a canasta
enum estadoBalon_t
{
    POSESION,
    ENAIRE
};

enum estadoBalon_t balon; // Modela el estado actual del balón
struct jugador jugadores[NJUGADORES] = {
    {0, 30, MAXREBOTE, 0}, // jugador con dorsal 0, eficTiro 30, eficRebote MAXREBOTE
    {1, 80, 20, 0},
    {2, 40, MAXREBOTE, 0},
    {3, 50, 50, 0}};

// Para la sincronización

pthread_mutex_t m;
pthread_cond_t rebote, tira;

int difRebote;
int finished;
void tirar(struct jugador *j)
{
    // si no ha acabado el entrenamiento
    if (!finished)
    {

        pthread_mutex_lock(&m);
        while (!finished && (balon != POSESION))
            pthread_cond_wait(&tira, &m);
        if (!finished)
        {
            balon = ENAIRE;
            int difTiro;
            printf("Jugador %d tirando a canasta...", j->dorsal);
            difTiro = rand() % 100;
            // <si difTiro es menor que nuestra eficacia en tiro : canasta>
            if (difTiro < j->eficTiro)
            {
                j->puntos += 2;
                printf(" ¡DENTRO! %d puntos\n", j->puntos);
                //<si hemos sobrepasado TOPEPUNTOS, termina el entrenamiento>
                if (j->puntos >= TOPEPUNTOS)
                {
                    finished = 1;
                    printf("\n\n---EL JUGADOR %d HA LLEGADO AL MAXIMO DE PUNTOS!!!--\n\n", j->dorsal);
                }
            }
            else
                printf(" ha fallado :(\n");
            difRebote = rand() % MAXREBOTE;
        }
        //<avisar a todos los jugadores para que luchen por el rebote>
        pthread_cond_broadcast(&rebote);
        pthread_mutex_unlock(&m);
    }
}
void rebotear(struct jugador *j)
{
    if (!finished)
    {
        //<esperar a conseguir rebote>
        pthread_mutex_lock(&m);
        while (!finished && (balon != ENAIRE || difRebote > j->eficRebote))
            pthread_cond_wait(&rebote, &m);
        if (!finished)
        {
            balon = POSESION;
            printf("El jugador %d ha cogido el rebote!!\n", j->dorsal);
        }
        pthread_cond_broadcast(&tira);
        pthread_mutex_unlock(&m);
    }
}
void correr(int j)
{
    if (!finished)
    {
        printf("\nJugador %d corriendo...\n", j);
        usleep(100);
    }
}
void *jugadorInit(void *args)
{
    struct jugador *infoJugador = (struct jugador *)args;
    printf("[Jugador %d]\n\tEfectividad de tiro: %d \n\tEfictividad del rebote %d \n\tPuntos anotados: %d\n",
           infoJugador->dorsal, infoJugador->eficTiro, infoJugador->eficRebote, infoJugador->puntos);
    if (infoJugador->dorsal == 1)
        tirar(infoJugador);
    while (!finished)
    {

        rebotear(infoJugador);
        correr(infoJugador->dorsal);
        tirar(infoJugador);
        correr(infoJugador->dorsal);
    }
    printf("Yo me he ido ya a ducharme %d\n", infoJugador->dorsal);
    // Avisan todos por si alguno se ha quedado bloqueado
    pthread_cond_broadcast(&tira);
    pthread_cond_broadcast(&rebote);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t jugador[NJUGADORES];
    pthread_cond_init(&rebote, NULL);
    pthread_cond_init(&tira, NULL);
    pthread_mutex_init(&m, NULL);
    finished = 0;
    difRebote = 0;
    balon = POSESION;
    for (int i = 0; i < NJUGADORES; i++)
    {
        pthread_create(&jugador[i], NULL, jugadorInit, &jugadores[i]);
    }
    for (int i = 0; i < NJUGADORES; i++)
    {
        pthread_join(jugador[i], NULL);
    }
    return 0;
}