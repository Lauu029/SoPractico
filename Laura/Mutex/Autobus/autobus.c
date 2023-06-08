#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N_PARADAS 5   // número de paradas de la ruta
#define EN_RUTA 0      // autobús en ruta
#define EN_PARADA 1    // autobús en la parada
#define MAX_USUARIOS 4 // capacidad del autobús
#define USUARIOS 13    // numero de usuarios

// estado inicial
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus
int n_ocupantes = 0;   // ocupantes que tiene el autobús

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};

// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

// Otras definiciones globales (comunicación y sincronización)
pthread_mutex_t m;
pthread_cond_t esperaPasajeros, esperaAutobus;

/* Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
quieran bajar y/o subir la parada actual. Después se pone en marcha */
void Autobus_En_Parada()
{
    pthread_mutex_lock(&m);
    estado = EN_PARADA;
    printf("\n-------PARADA %d--------\n\tusuarios dentro: %d, usuarios esperando salir: %d, usuarios esperando entrar: %d\n",
           parada_actual, n_ocupantes, esperando_bajar[parada_actual], esperando_parada[parada_actual]);

    while (((n_ocupantes < MAX_USUARIOS) && (esperando_parada[parada_actual] > 0)) || (esperando_bajar[parada_actual] > 0))
    {
        // Cada vez que sube o baja un pasajero avisa a los demas para que comprueben si se pueden subir o bajar
        pthread_cond_broadcast(&esperaAutobus);
        pthread_cond_wait(&esperaPasajeros, &m);
    }
    printf("\nEN RUTA\n\n");
    estado = EN_RUTA;
    pthread_mutex_unlock(&m);
}
/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
void Conducir_Hasta_Siguiente_Parada()
{
    pthread_mutex_lock(&m);
    parada_actual = (parada_actual + 1) % N_PARADAS;
    printf("Autobús dirigiéndose a la parada %d...\n", parada_actual);
    sleep((rand() % 5) + 1);
    pthread_mutex_unlock(&m);
}
/* El usuario indicará que quiere subir en la parada ’origen’, esperará a que
el autobús se pare en dicha parada y subirá. El id_usuario puede utilizarse para
proporcionar información de depuración */
void Subir_Autobus(int id_usuario, int origen)
{
    pthread_mutex_lock(&m);
    esperando_parada[origen]++;
    printf("Usuario %d esperando en la parada %d\n", id_usuario, origen);
    while (estado != EN_PARADA || parada_actual != origen || n_ocupantes == MAX_USUARIOS)
        pthread_cond_wait(&esperaAutobus, &m);

    n_ocupantes++;
    esperando_parada[origen]--;
    printf("Usuario %d subiendo al autobus\n", id_usuario);

    // Avisa al autobús por si está esperando a que se bajen
    pthread_cond_signal(&esperaPasajeros);
    pthread_mutex_unlock(&m);
}
/* El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
proporcionar información de depuración */
void Bajar_Autobus(int id_usuario, int destino)
{
    pthread_mutex_lock(&m);
    esperando_bajar[destino]++;
    while (estado != EN_PARADA || parada_actual != destino)
        pthread_cond_wait(&esperaAutobus, &m);
    n_ocupantes--;
    esperando_bajar[destino]--;
    printf("Usuario %d bajando parada %d usuarios actuales: %d\n", id_usuario, destino, n_ocupantes);
    // Avisa al autobús por si está esperando a que se bajen
    pthread_cond_signal(&esperaPasajeros);
    pthread_mutex_unlock(&m);
}
void *thread_autobus(void *args)
{
    while (1)
    {
        // esperar a que los viajeros suban y bajen
        Autobus_En_Parada();
        // conducir hasta siguiente parada
        Conducir_Hasta_Siguiente_Parada();
    }
}
void Usuario(int id_usuario, int origen, int destino)
{
    // Esperar a que el autobus esté en parada origen para subir
    Subir_Autobus(id_usuario, origen);
    // Bajarme en estación destino
    Bajar_Autobus(id_usuario, destino);
}
void *thread_usuario(void *arg)
{
    int id_usuario;
    id_usuario = *(int *)arg;
    int a, b;
    // obtener el id del usario
    while (1)
    {
        a = rand() % N_PARADAS;
        do
        {
            b = rand() % N_PARADAS;
        } while (a == b);
        Usuario(id_usuario, a, b);
    }
}

int main(int argc, char *argv[])
{
    // Definición de variables locales a main
    // Opcional: obtener de los argumentos del programa la capacidad del
    // autobus, el numero de usuarios y el numero de paradas
    pthread_t hilosUsuarios[USUARIOS];
    int idUsuarios[USUARIOS];
    // Crear el thread Autobus
    pthread_t autobus;
    pthread_create(&autobus, NULL, thread_autobus, NULL);
    for (int i = 0; i < USUARIOS; i++)
    {
        // Crear thread para el usuario i
        // Esperar terminación de los hilos
        idUsuarios[i] = i;
        pthread_create(&hilosUsuarios[i], NULL, thread_usuario, &idUsuarios[i]);
    }
    pthread_join(autobus, NULL);
    for (int i = 0; i < USUARIOS; i++)
    {
        pthread_join(hilosUsuarios[i], NULL);
    }
    return 0;
}