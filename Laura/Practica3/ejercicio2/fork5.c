#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//Grafo de dependencias:
/*
		       [P3]
			   /   \
		      /     \
	      [P2]       \
	     /   \        \
	    /    [P4]-[P6]-[P8]
	   /		  /      /
	[P1] ------[P5]		/
	   \               /
		\-----[P7]----/
*/
/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */
#define N 8

int main(int argc, char **argv)
{
	// Creo un array de pid para ir guardando los pid de los hijos
	int pid[N];

	// Primer hijo(P1 en el grafo)
	if ((pid[0] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	// proceso hijo p1
	if (pid[0] == 0)
	{
		// hay que pasarle dos veces el programa que debe ejecutar
		execlp("echo", "echo", "P1", NULL);
	}

	// Como el hijo se sale de la ejecucion lo demás lo hace el proceso padre
	// los siguienes dependen del primero asi que espero a que se cierre
	// Uso waitpid en lugar del while porque es más claro, pero funcionan igual
	waitpid(pid[0], NULL, 0);

	// Hijos p2,p5 y p7
	if ((pid[1] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[1] == 0)
	{
		// Ruta absoluta (no está p)
		execl("/bin/echo", "/bin/echo", "P2", NULL);
	}

	if ((pid[4] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[4] == 0)
	{
		execlp("echo", "echo", "P5", NULL);
	}

	if ((pid[6] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[6] == 0)
	{
		execlp("echo", "echo", "P7", NULL);
	}

	// Espera a que acabe p2 para crear p3 y p4
	waitpid(pid[1], NULL, 0);

	// Creacion y ejecución de p3 y p4
	if ((pid[2] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[2] == 0)
	{
		execlp("echo", "echo", "P3", NULL);
	}

	if ((pid[3] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[3] == 0)
	{
		execl("/bin/echo", "/bin/echo", "P4", NULL);
	}

	// Espera a p4 y p5 para crear p6
	waitpid(pid[3], NULL, 0);
	waitpid(pid[4], NULL, 0);

	// creación y ejecución de p6
	if ((pid[5] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[5] == 0)
	{
		execl("/bin/echo", "/bin/echo", "P6", NULL);
	}

	// Espero a que acaben 3, 6 y 7 para crear 8
	waitpid(pid[2], NULL, 0);
	waitpid(pid[5], NULL, 0);
	waitpid(pid[6], NULL, 0);

	//Creo p8
	if ((pid[7] = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid[7] == 0)
	{
		execl("/bin/echo", "/bin/echo", "P8", NULL);
	}
	//Espero a que acabe p8 para cerrar el programa
	waitpid(pid[7],NULL,0);
	return 0;
}
