#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	int numBlocks = *blocks / 2;
	printf("\t%dK %s\n", numBlocks, fname);
	return numBlocks;
}

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	// Struct con la informacion que lee de los archivos dentro del directorio
	/*
	struct dirent {
			   ino_t          d_ino;       Inode number
			   off_t          d_off;       Not an offset; see below
			   unsigned short d_reclen;    Length of this record
			   unsigned char  d_type;      Type of file; not supported by all filesystem types
			   char           d_name[256]; Null-terminated filename
		   };
	DT_BLK  block device.
	DT_CHR  character device.
	DT_DIR  directory.
	DT_FIFO named pipe (FIFO).
	DT_LNK  symbolic link.
	DT_REG  regular file.s
	DT_SOCK UNIX domain socket.
	DT_UNKNOWN  The file type could not be determined.
	*/
	struct dirent *dirInfo;
	struct stat statbuf;
	DIR *d;
	d = opendir(dname);
	int t = statbuf.st_blocks / 2;
	// Leo el directorio hasta el final del archivo
	while ((dirInfo = readdir(d)) != NULL)
	{
		// Comparo el string del nombre con . y .. para ver si hay que ignorar el archivo
		if ((strcmp(dirInfo->d_name, ".") != 0) & (strcmp(dirInfo->d_name, "..") != 0))
		{
			lstat(dirInfo->d_name, &statbuf);
			switch (statbuf.st_mode & __S_IFMT)
			{
			case __S_IFDIR:
				t += get_size_dir(dirInfo->d_name, &statbuf.st_blocks);
				break;
			case __S_IFREG:
				t += get_size(dirInfo->d_name, &statbuf.st_blocks);
				break;
			default:
				break;
			}
		}
	}
	printf("%dK %s\n", t, dname);
	closedir(d);
	return t;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	int size;
	struct stat statbuf;
	// Reviso toda la lista de argumentos
	for (int i = 1; i < argc; i++)
	{
		lstat(argv[i], &statbuf);
		switch (statbuf.st_mode & __S_IFMT)
		{
		case __S_IFDIR:
			size = get_size_dir(argv[i], &statbuf.st_blocks);
			break;
		case __S_IFREG:
			size = get_size(argv[i], &statbuf.st_blocks);
			break;
		default:
			break;
		}
	}

	return 0;
}
