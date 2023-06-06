#include <stdlib.h>
#include <stdio.h>
// Para los hilos
#include <pthread.h>

#define N 15 // Número de hilos

// Hay que compilar y enlazar con la opcion -pthread (Ya está en make)
typedef struct
{
	int nHilo;
	char prioritario; // Pares prioritarios (P) impares no prioritarios (N)
} info_hilos;		  // Información necesaria para un hilo

// Lo que ejecuta el hilo al ser creado, los argumentos en este caso son info_hilos
void *thread_usuario(void *arg)
{
	info_hilos *info = (info_hilos *)arg;
	printf("Soy el hilo numero: %d, mi prioridad es: %c, y mi ID es %ld\n", info->nHilo, info->prioritario, pthread_self());
	// Libera la memoria que se había creado para el hilo
	free(info);
	// Cierra el hilo
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	pthread_t hilos[N];
	for (int i = 0; i < N; i++)
	{
		info_hilos *informacionHilos;
		// reserva y liberación de memoria para los hilos
		informacionHilos = malloc(sizeof(info_hilos));

		if (i % 2) // Impares
			informacionHilos->prioritario = 'N';

		else // pares
			informacionHilos->prioritario = 'P';
		informacionHilos->nHilo = i;
		// Pasa la dirección del hijo, los que debe hacer el hilo y el struct de argumentos
		pthread_create(&hilos[i], NULL, thread_usuario, informacionHilos);
	}
	// Espera a que todos los hilos se cierren para cerrar el programa
	for (int i = 0; i < N; i++)
	{
		pthread_join(hilos[i], NULL);
	}

	return 0;
}
