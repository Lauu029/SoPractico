#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>	  /* For O_* constants */
#include <signal.h>
#define M 10

sem_t *raciones;
sem_t *vacias;
int finish = 0;
int *buffer;

void putServingsInPot(int servings)
{
	int i;
	int id = getpid();
	for (i = 0; i < servings; i++)
	{
		*buffer+=1;
		sem_post(raciones);
	}
	printf("All %d servings prepared by ID:%d\n", *buffer,id);
}

void cook(void)
{
	while (!finish)
	{
		putServingsInPot(M);
		sem_wait(vacias);
	}
}

void handler(int signo)
{
	finish = 1;
	printf(" Closing program\n");
	sem_post(vacias);
}

int main(int argc, char *argv[])
{
	//escucha las señales de consola
	//(ctrl+c)->sigInt para leerlo si está bloqueado en un semáforo
	//sigTerm-> es lo que se ejecuta al cerrar la consola(final de proceso)
	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	//id del fichero compartido y buffer de comida donde se almacena y se quita(int que guarda el valor)
	int shared;
	shared = shm_open("/BUFFER", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(shared, sizeof(int));
	buffer = (int *)mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, shared, 0);
	*buffer = 0;
	raciones = sem_open("/RACIONES", O_CREAT, 0700, 0);
	vacias = sem_open("/VACIAS", O_CREAT, 0700, 0);

	cook();

	munmap(buffer, sizeof(int));
	shm_unlink("/BUFFER");
	sem_close(raciones);
	sem_close(vacias);
	sem_unlink("/RACIONES");
	sem_unlink("/VACIAS");
	printf("Shared memory freed\n");
	return 0;
}