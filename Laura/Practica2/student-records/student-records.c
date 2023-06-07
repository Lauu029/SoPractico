#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>

/*
	coge un string caracter a caracter hasta que llega a '\0'
	resetea el puntero del archivo con la longitud que ha leído
	reserva el espacio en memoria
	lee los caracteres correspondientes y devuelve el array de chars creado
*/
char *loadstr(FILE *file)
{
	char aux;
	int cont = 0;

	do
	{
		fread(&aux, sizeof(char), 1, file);
		cont++;
	} while (aux != '\0');
	char *salida = malloc(cont);
	fseek(file, -cont, SEEK_CUR);
	fread(salida, sizeof(char), cont, file);
	return salida;
}
/*
	Esta función acepta como parámetro el listado de registros en formato ASCII
	pasados como argumentro al programa en la línea de comando(records)
	así como el número de registros (nr_records), y devuelve la representación
	binaria en memoria de los mismos. Esta representación será un array de estructuras
*/
student_t *parse_records(char *records[], int nr_records)
{
	student_t *binaryStudents = (student_t *)malloc(nr_records * sizeof(student_t));

	char *aux;
	for (int i = 0; i < nr_records; i++)
	{
		// mejor strsep porque devuelve la cadena y luego le escribe \0 directamente, le pasa & a records porque es el contenido del puntero

		aux = strsep(&records[i], ":");
		binaryStudents[i].student_id = atoi(aux);

		aux = strsep(&records[i], ":");
		strcpy(binaryStudents[i].NIF, aux);

		aux = strsep(&records[i], ":");
		binaryStudents[i].first_name = aux;

		aux = strsep(&records[i], ":");
		binaryStudents[i].last_name = aux;
	}
	return binaryStudents;
}

int dump_entries(student_t *entries, int nr_entries, FILE *students)
{
	for (int i = 0; i < nr_entries; i++)
	{
		fwrite(&entries[i].student_id, sizeof(int), 1, students);
		// suma 1 a los tamaños para tener en cuenta el '\0'
		fwrite(entries[i].NIF, strlen(entries[i].NIF) + 1, 1, students);
		fwrite(entries[i].first_name, strlen(entries[i].first_name) + 1, 1, students);
		fwrite(entries[i].last_name, strlen(entries[i].last_name) + 1, 1, students);
	}
	return 0;
}

student_t *read_student_file(FILE *students, int *nr_entries)
{
	fread(nr_entries, sizeof(int), 1, students);
	student_t *readStudents = (student_t *)malloc(*nr_entries * sizeof(student_t));
	for (int i = 0; i < *nr_entries; i++)
	{
		fread(&readStudents[i].student_id, sizeof(int), 1, students);
		strcpy(readStudents[i].NIF, loadstr(students));
		readStudents[i].first_name = loadstr(students);
		readStudents[i].last_name = loadstr(students);
	}
	return readStudents;
}

int main(int argc, char *argv[])
{
	int opt;
	FILE *alumnos = NULL;
	char *name;
	// Lee los flags que se le meten por consola
	while ((opt = getopt(argc, argv, "hlq:f:c:a:i:n:")) != -1)
	{

		if (opt == 'h')
		{
			fprintf(stderr, "Usage:%s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n", argv[0]);
			exit(0);
		}
		else if (opt == 'f')
		{
			name = optarg;
		}
		else if (opt == 'l')
		{
			// imprime el archivo de estudiantes
			if ((alumnos = fopen(name, "r")) == NULL)
			{
				fprintf(stderr, "Non existing file\n");
				exit(1);
			}
			int nr_students = 0;
			student_t *Students = read_student_file(alumnos, &nr_students);
			for (int i = 0; i < nr_students; i++)
			{
				printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n", i,
					   Students[i].student_id, Students[i].NIF, Students[i].first_name, Students[i].last_name);
			}
			free(Students);
			free(alumnos);
			exit(0);
		}
		else if (opt == 'c')
		{
			// crea el archivo de estudiantes
			if ((alumnos = fopen(name, "w")) == NULL)
			{
				exit(1);
			}
			// parsear los alumnos que se le pasan por argv
			int nalumnos = argc - (optind - 1);
			char **nuevosAlumnos = malloc(sizeof(char *) * nalumnos);
			for (int i = 0; i < nalumnos; i++)
			{
				nuevosAlumnos[i] = argv[i + optind - 1];
			}
			student_t *s = parse_records(nuevosAlumnos, nalumnos);
			// Escribe los alumnos en un archivo
			fwrite(&nalumnos, sizeof(int), 1, alumnos);
			dump_entries(s, nalumnos, alumnos);

			printf("%d records written succesfully\n", nalumnos);

			// free
			free(s);
			free(nuevosAlumnos);
			fclose(alumnos);
			exit(0);
		}

		else if (opt == 'a')
		{
			// añade un nuevo estudiante
			if ((alumnos = fopen(name, "r")) == NULL)
			{
				perror("error en la lectura");
				exit(-1);
			}
			// lee a los estudiantes que ya están
			int num_entries = 0;
			student_t *st = read_student_file(alumnos, &num_entries);

			// lee la entrada
			int nalumnos = argc - (optind - 1);
			char **nuevosAlumnos = malloc(sizeof(char *) * nalumnos);
			for (int i = 0; i < nalumnos; i++)
			{
				nuevosAlumnos[i] = argv[i + optind - 1];
			}
			student_t *s = parse_records(nuevosAlumnos, nalumnos);
			// Comprueba si existen y si no  los añade
			int nuevosAlumnosDentro = num_entries;
			for (int i = 0; i < nalumnos; i++)
			{
				int j = 0;
				while (j < num_entries && s[i].student_id != st[j].student_id)
					j++;

				if (j == num_entries)
				{
					nuevosAlumnosDentro++;
					dump_entries(&s[i], 1, alumnos);
				}
				else
					printf("Found duplicate student_id %d\n", s[i].student_id);
			}
			printf("%d records written succesfully\n", nuevosAlumnosDentro - num_entries);

			// write new number of entries
			fseek(alumnos, 0, SEEK_SET);
			fwrite(&nuevosAlumnosDentro, sizeof(int), 1, alumnos);

			// free
			free(st);
			fclose(alumnos);
			exit(0);
		}
		else if (opt == 'q')
		{
			// busca un estudiante por numero de registro
			if ((alumnos = fopen(name, "r")) == NULL)
			{
				perror("error en la lectura");
				exit(-1);
			}
		}
		else if (opt == 'i')
		{
			//
			if (alumnos == NULL)
			{
				fprintf(stderr, "Non existing file\n");
				exit(1);
			}
			int student_id;
			sscanf(optarg, "%d", &student_id);

			// read students already existing
			int num_entries = 0;
			student_t *students = read_student_file(alumnos, &num_entries);

			// check if anyone has id = student_id
			int i = 0;
			while (i < num_entries && student_id != students[i].student_id)
				i++;

			// no entry found
			if (i == num_entries)
				printf("No entry was found\n");
			else
				printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
					   i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);

			// free
			fclose(alumnos);
			free(students);

			exit(0);
		}
		else if (opt == 'n')
		{
			//
			if (alumnos == NULL)
			{
				fprintf(stderr, "Non existing file\n");
				exit(1);
			}

			char *NIF;
			NIF = optarg;

			// read students already existing
			int num_entries = 0;
			student_t *students = read_student_file(alumnos, &num_entries);

			// check if anyone has NIF = NIF
			int i = 0;
			while (i < num_entries && strcmp(NIF, students[i].NIF) != 0)
				i++;

			// no entry found
			if (i == num_entries)
				printf("No entry was found\n");
			else
				printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
					   i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);

			// free
			fclose(alumnos);
			free(students);

			exit(0);
		}
	}
	return 0;
}