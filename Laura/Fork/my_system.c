#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#define MAXARGUMENTOS 100
int system_(char *command)
{
    char *comando;
    comando = (char *)malloc(sizeof(char));
    comando = strsep(&command, " ");
    int hayArgumentos = 0;
    char *argumentos[MAXARGUMENTOS];
    int i = 0;
    if (command != NULL)
    {
        while ((argumentos[i] = strsep(&command, " ")) != NULL)
        {
            i++;
        }
        hayArgumentos = 1;
    }

    int pid;
    if ((pid = fork()) == -1)
    {
        perror("Error al crear el proceso hijo\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        if (hayArgumentos)
            execlp(comando, comando, argumentos, NULL);
        else
            execlp(comando, comando, NULL);
    }
    int status;
    waitpid(pid, &status, 0);
    return status;
}
int main(int argc, char **argv)
{
    char c;

    // lee el comando que se le pasa
    char *comando;
    while ((c = getopt(argc, argv, "c")) != -1)
    {
        if (c == 'c')
        {
            comando = malloc(strlen(argv[optind]));
            strcpy(comando, argv[optind]);
        }
    }
    int end = system_(comando);
    if (WIFEXITED(end))
        printf("the child terminated normally\n");
    else if (WIFSIGNALED(end))
        printf("the child process was terminated by a signal\n");
    else if (WCOREDUMP(end))
        printf("the child produced a core\n");
    else if (WIFSTOPPED(end))
        printf("the child process was stopped by delivery of a signal\n");
    else if (WIFCONTINUED(end))
        printf("the child process was resumed by delivery of SIGCONT\n");
    free(comando);
    return 0;
}