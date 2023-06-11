#include <stdio.h>
#include <stdlib.h>
// Para fork
#include <sys/types.h>
#include <unistd.h>
// Para wait
#include <sys/wait.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */

int main(int argc, char **argv)
{
	/*
		fork crea un nuevo proceso duplicando el proceso actual,
		devuelve el pid del hijo en el proceso padre
		devuelve 0 en el proceso hijo
	*/
	int pid;
	// Chequeo de errores
	if ((pid = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// getppid() devuelve el id del proceso padre
		// getpid() devuelve el id del proceso
		printf("Soy el proceso hijo, mi ID es %d, el ID de mi padre es %d\n", getpid(), getppid());
	}
	else
	{
		printf("Soy el proceso padre, mi ID es %d, el ID de mi proceso padre es %d\n", getpid(), getppid());
		// Espera a que todos sus hijos terminen, wait devuelve el id del proceso que ha terminado, por lo que
		// al comprobar el padre si es igual que el pid de su hijo, puede cerrarse cuando su hijo termine
		while (pid != wait(NULL));
	}
	// Esto lo realizan ambos procesos
	// Chequeo de errores y creacion de un nuevo hijo
	if ((pid = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		printf("Soy el segundo proceso hijo, mi ID es %d, el ID de mi padre es %d\n", getpid(), getppid());
		// Los hijos cambian su ejecución a otro programa que se le pase por argumento, se le pueden pasar variables y argumentos para el
		// programa que va a ejecutar
		execvp(argv[1], argv + 1);
	}
	else
	{
		printf("Soy el segundo proceso padre, mi ID es %d, el ID de mi proceso padre es %d\n", getpid(), getppid());
		//Se puede usar el while y comparar el pid o waitpid directamente
		//while (pid != wait(NULL));
		waitpid(pid,NULL,0);
	}
	return 0;
}
