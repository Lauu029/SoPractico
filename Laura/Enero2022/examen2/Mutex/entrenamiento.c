#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NJUGADORES 4
#define MAXREBOTE 70
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
int dificultadRebote = 0; // var. Global para indicar la dificultad del rebote actual
struct jugador jugadores[NJUGADORES] = {
    {0, 30, MAXREBOTE, 0}, // jugador con dorsal 0, eficTiro 30, eficRebote MAXREBOTE
    {1, 80, 20, 0},
    {2, 40, MAXREBOTE, 0},
    {3, 50, 50, 0}};

pthread_mutex_t m;

void *jugadorInit(void *args)
{
    struct jugador *infoJugador = (struct jugador *)args;
    printf("[Jugador %d]\n\tEfectividad de tiro: %d \n\tEfictividad del rebote %d \n\tPuntos anotados: %d\n",
           infoJugador->dorsal, infoJugador->eficTiro, infoJugador->eficRebote, infoJugador->puntos);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t jugador[NJUGADORES];

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