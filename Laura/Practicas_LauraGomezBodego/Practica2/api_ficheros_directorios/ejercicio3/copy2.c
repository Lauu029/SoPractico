#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysmacros.h>

int copy_regular(int fdo, int fdd)
{
	unsigned char buff[512];
	int readBytes;
	while (readBytes = read(fdo, buff, sizeof(buff)))
	{

		if (write(fdd, buff, readBytes) == -1)
		{
			close(fdd);
			close(fdo);
			err(3, "putc() failed!!");
			return -1;
		}
	}
	if (readBytes == -1)
	{
		close(fdd);
		close(fdo);
		err(3, "putc() failed!!");
		return -1;
	}
	return 0;
}

void copy_link(char *orig, char *dest, struct stat *statbuf)
{
	// Reservar memoria segun el tamaño del link simbolico
	char *buff;
	buff = malloc(statbuf->st_size + 1);
	
	// Copiar en el buffer la ruta del fichero al que se apunta
	// readlink pone el contenido del link simbolico que se le pasa en el buffer del que se le pasa tambien el tamaño
	// hay que añadir \0 al final
	if ((readlink(orig, buff, statbuf->st_size)) == -1)
	{
		perror("readLink");
		exit(EXIT_FAILURE);
	}
	buff[statbuf->st_size + 1] = '\0';

	//  crea un link simbolico en el archivo que se le pide
	if (symlink(buff, dest) < 0)
	{
		fprintf(stderr, "symlink failed!\n");
		printf("%d <- error type\n", errno);
		printf("Destiny file name: %s\n", dest);
	}
	free(buff);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	// Contiene la informacion del tipo de archivo que se ha leído
	/*
		todos los campos del struct:
		struct stat {
			   dev_t     st_dev;         ID of device containing file
			   ino_t     st_ino;         Inode number
			   mode_t    st_mode;        File type and mode
			   nlink_t   st_nlink;       Number of hard links
			   uid_t     st_uid;         User ID of owner
			   gid_t     st_gid;         Group ID of owner
			   dev_t     st_rdev;        Device ID (if special file)
			   off_t     st_size;        Total size, in bytes
			   blksize_t st_blksize;     Block size for filesystem I/O
			   blkcnt_t  st_blocks;      Number of 512B blocks allocated
		valores del tipo de archivo:
			S_IFBLK: block device
			S_IFCHR:  character device
			S_IFDIR:  directory
			S_IFIFO:  FIFO/pipe
			S_IFLNK:  symlink
			S_IFREG:  regular file
			S_IFSOCK: socket
	*/
	struct stat statbuf;
	// lstat devuelve información sobre un archivo concreto, pide una direccion de struct por lo que le paso & para devolver la direccion
	if (lstat(argv[1], &statbuf) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	// Hay que añadir __ antes de la flag a veces porque la vida sin frustracion es aburrida
	switch (statbuf.st_mode & __S_IFMT)
	{
	case __S_IFREG:
	{

		int readFile, writeFile;

		if ((readFile = open(argv[1], O_RDONLY)) == -1)
		{
			// Devuelve error si no ha podido abrir el archivo
			err(2, "The input file %s could not be opened", argv[1]);
		}
		// con los ficheros regulares hago lo que hacía antes
		if ((writeFile = open(argv[2], O_WRONLY | O_CREAT, 00700)) == -1)
		{
			err(2, "The input file %s could not be opened", argv[2]);
		}
		if (copy_regular(readFile, writeFile) == 0)
		{
			close(readFile);
			close(writeFile);
		}
		else
			return -1;
	}
	break;
	case __S_IFLNK:
		copy_link(argv[1], argv[2], &statbuf);
		break;
	default:
		printf("unknown file type");
		err(3, "The input file %s is not a regular file or a symbolic link", argv[1]);
		break;
	}
	return 0;
}
