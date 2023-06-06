#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 3
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

pthread_mutex_t m;
pthread_cond_t turnoCola;
int turnoNormal, turnoVip;
int ticketNormal, ticketVip;
int numVipsCola;
int numClientes;
typedef struct
{
	int id;
	int isVip;
} thread_info;

/*
	Definir condiciones, un mutex y las variables comunes
*/
void enter_normal_client(int id)
{
	pthread_mutex_lock(&m);
	// Coge su ticket y avanza el número del siguiente (Estilo la cola de la frutería)
	int myTicket = ticketNormal;
	ticketNormal++;
	printf("Normal client %d waiting in queue\n", id);
	// Espera a que sea su turno, que el local esté lleno y a que no haya vips en la cola
	while (myTicket != turnoNormal || numVipsCola != 0 || numClientes == CAPACITY)
		pthread_cond_wait(&turnoCola, &m);
	printf("Normal client %d has enter the disco, people in disco %d\n", id, numClientes);
	// avanza un turno
	turnoNormal++;
	// Añade un cliente
	numClientes++;
	pthread_mutex_unlock(&m);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&m);
	// Se coge un ticket y ese ticket avanza, luego se espera a que el ticket coincida con el turno
	int myTicket = ticketVip;
	ticketVip++;
	// Añado un cliente vip a la cola
	numVipsCola++;
	printf("Vip client %d waiting in queue, vips in queue:%d\n", id, numVipsCola);
	// Espera mientras esté lleno y no sea su turno
	while (myTicket != turnoVip || numClientes == CAPACITY)
	{
		pthread_cond_wait(&turnoCola, &m);
	}
	// Quito al cliente vip de la cola
	numVipsCola--;
	// Avanza un turno en la cola
	turnoVip++;
	numClientes++;
	pthread_mutex_unlock(&m);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
	// Bloquea, manda señal de siguiente en la cola, resta uno a los de dentro
	pthread_mutex_lock(&m);
	numClientes--;
	printf("Client %2d (%s) has exit the disco, clients in disco: %d\n", id, VIPSTR(isvip), numClientes);
	pthread_cond_broadcast(&turnoCola);
	pthread_mutex_unlock(&m);
}
/*
	Estructura general del mutex:
	lock;
	while()
		wait(cond)
	acción
	unlock()
*/
void *client(void *arg)
{
	thread_info *info = (thread_info *)arg;
	if (info->isVip)
		enter_vip_client(info->id);
	else
		enter_normal_client(info->id);
	dance(info->id, info->isVip);
	disco_exit(info->id, info->isVip);
	// Libero la memoria
	free(info);
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	FILE *entry;
	ticketNormal = 0;
	ticketVip = 0;
	turnoNormal = 0;
	turnoVip = 0;
	numClientes = 0;
	if ((entry = fopen(argv[1], "r")) == NULL)
	{
		printf("Error en la lectura del fichero\n");
		exit(EXIT_FAILURE);
	};

	int nClients;
	// leer datos de un archivo en un formato específico
	fscanf(entry, "%d", &nClients);

	// crear tantos hilos como clientes haya
	pthread_t clientes[nClients];
	for (int i = 0; i < nClients; i++)
	{
		thread_info *info;
		info = malloc(sizeof(thread_info));
		fscanf(entry, "%d", &info->isVip);
		info->id = i;
		pthread_create(&clientes[i], NULL, client, info);
	}

	for (int i = 0; i < nClients; i++)
	{
		pthread_join(clientes[i], NULL);
	}

	return 0;
}
