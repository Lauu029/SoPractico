#include <stdio.h>
#include <stdlib.h>
// Para las señales
#include <signal.h>
// Para fork
#include <sys/types.h>
#include <unistd.h>
// Para wait
#include <sys/wait.h>
#include <errno.h>

// pid publico para que lo pueda usar killchild
pid_t pid;
// Método que mata al proceso hijo
void killChild()
{
	// Envía una señal al proceso que tiene de id pid
	// SIGKILL indica matar el proceso pid
	if (kill(pid, SIGKILL) == -1)
	{
		perror("Error al enviar la señal\n");
		exit(EXIT_FAILURE);
	}
}

/*programa que temporiza la ejecución de un proceso hijo */
int main(int argc, char **argv)
{
	if ((pid = fork()) == -1)
	{
		perror("Error al crear el proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// El hijo ejecuta el programa que se le haya pasado
		execvp(argv[1], argv + 1);
	}
	// Ejecución del padre
	/*
	The sigaction structure is defined as something like:

		   struct sigaction {
			   void     (*sa_handler)(int);
			   void     (*sa_sigaction)(int, siginfo_t *, void *);
			   sigset_t   sa_mask;
			   int        sa_flags;
			   void     (*sa_restorer)(void);
		   };
	*/

	// Struct de creación de señales
	struct sigaction act;
	// Método que va a manejar la señar(declarado arriba)
	act.sa_handler = killChild;
	// SIGALRM se envía cuando acaba la alarma, entonces hace la función que indique act
	sigaction(SIGALRM, &act, NULL);

	// Configurar SIGINT para ser ignorada por el padre
	struct sigaction ignore;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore, NULL);

	// Alarma, crea una señal SIGALRM que se le manda al proceso en n segundos
	alarm(20);

	// Valor para wait que devuelve en que estado ha finalizado el proceso hijo
	int n, wStatus;
	/*
	 El bucle do-while con wait se utiliza para manejar la posibilidad de que se produzca una interrupción por señal durante
	 la espera del hijo. Si se recibe una señal mientras wait está en progreso, la llamada a wait puede fallar con
	  un error de EINTR, que indica que la llamada ha sido interrumpida por una señal.

	  Asegura que la función wait se vuelva a ejecutar en caso de que sea interrumpida
	  por una señal y garantiza que el programa siga esperando y recogiendo el estado de todos los hijos.
	*/
	do
	{
		n = wait(&wStatus);
	} while (n == -1 && errno == EINTR);

	// Wifsignaled devuelve true si el proceso hijo terminó por una señal
	if (WIFSIGNALED(wStatus))
	{
		printf("El proceso hijo terminó al recibir una señal\n");
	}
	// Wifexited devuelve true si el proceso hijo terminó de forma natural
	else if (WIFEXITED(wStatus))
	{
		printf("El proceso hijo terminó de forma natural\n");
	}
	return 0;
}
