#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define NUMITER 15

sem_t *raciones;
sem_t *vacias;

int *buffer;
void eat(void)
{
	unsigned long id = (unsigned long)getpid();
	printf("Savage %lu eating\n", id, *buffer);
	sleep(rand() % 5);
}
int getServingsFromPot(void)
{
	unsigned long id = (unsigned long)getpid();
	if (*buffer == 0)
	{
		sem_post(vacias);
	}
	sem_wait(raciones);
	*buffer -= 1;
	printf("Savage %lu gets a serving, servings left:%d\n", id,*buffer);
}

void savages(void)
{
	for (int i = 0; i < NUMITER; i++)
	{
		getServingsFromPot();
		eat();
	}
}

int main(int argc, char *argv[])
{
	//Productor/consumidor
	int shared;
	if ((shared = shm_open("/BUFFER", O_RDWR,  S_IRUSR | S_IWUSR)) == -1)
	{
		printf("Error: Debes ejecutar primero el programa del cocinero\n");
		return -1;
	}
	buffer = (int *)mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, shared, 0);
	raciones = sem_open("/RACIONES", 0);
	vacias = sem_open("/VACIAS", 0);
	savages();

	sem_close(raciones);
	sem_close(vacias);
	return 0;
}