#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/**
 * Clona un puntero char, reserva espacio en memoria, cogiendo la longitud
 * del string original + 1 (caracter '\0' que no cuenta para la longitud)
 * strcpy copia a un string copy el original. Devuelve un puntero a un array de char.
 *
 * Es igual que la funcion strdup
 **/
static inline char *clone_string(char *original)
{
	char *copy;
	copy = malloc(strlen(original) + 1);
	strcpy(copy, original);
	return copy;
}

/**
 * Primero abre el archivo paswd, con fopen, si falla devuelve un error
 * El while lee todo el archivo para contar el número de filas que tiene
Devuelve un struct de tipo paswd_entry
 */
passwd_entry_t *parse_passwd(struct options *options, int *nr_entries)
{
	FILE *passwd;
	char line[MAX_PASSWD_LINE + 1];
	passwd_entry_t *entries;
	passwd_entry_t *cur_entry;
	int line_count = 0;
	char *token;
	char *lineptr;
	token_id_t token_id;
	int entry_idx;
	int entry_count = 0;
	int cur_line;

	if ((passwd = fopen(options->inputfile, "r")) == NULL)
	{
		fprintf(stderr, options->inputfile ," could not be opened: ");
		perror(NULL);
		return NULL;
	}

	/* Figure out number of lines */
	// Fgets lee uno menos del número de caracteres que le pidas (MAX_PASSWD_LINE) del archivo que se le pasa (passwd) y los
	//  almacena en un puntero de chars (line) fgets para cuando se acaba el archivo o en una nueva linea, que se almacena
	//  en el buffer, seguida de '\0'
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL)
	{
		line_count++;
		/* Discard lines that begin with # */
		if (line[0] != '#')
			entry_count++;
	}

	/* Rewind position indicator*/
	// pone el puntero al archivo paswd en el principio, SEEK_SET es un flag que dice que el offset anterior(0) es relativo
	// al inicio del archivo.
	fseek(passwd, 0, SEEK_SET);

	// Reserva espacio en memoria
	entries = malloc(sizeof(passwd_entry_t) * entry_count);

	/* Se pone el array de entradas a 0 */
	memset(entries, 0, sizeof(passwd_entry_t) * entry_count);

	/* Parseo del archivo */
	entry_idx = 0;
	cur_line = 1;
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL)
	{

		/* Descarta las líneas que comienzan con '#' */
		if (line[0] == '#')
		{
			cur_line++;
			continue;
		}

		/* Puntero al inicio de la línea*/
		lineptr = line;
		token_id = LOGIN_NAME_IDX;
		cur_entry = &entries[entry_idx];
		/*
			strsep divide una cadena en tokens utilizando un delimitador concreto. Toma un puntero a puntero  a char
			que apunta a la cadena que se va a dividir y un puntero o char que especifica que caracter separa los tokens
			reemplaza los tokens de la cadena ppor '\0' , devuelve el token y actrualiza el puntero a char (lineptr) para que
			apunte al siguiente token o a null cuando se ha acabado
		*/
		while ((token = strsep(&lineptr, ":")) != NULL)
		{
			switch (token_id)
			{
			case LOGIN_NAME_IDX:
				/*
					strcpy copia la cadena en una ubicacion existente de memoria y
					clone_string crea el espacio en memoria
				*/
				strcpy(cur_entry->login_name, token);
				break;
			case ENCRYPTED_PASS_IDX:
				cur_entry->optional_encrypted_passwd = clone_string(token);
				break;
			case UID_IDX:
				/*
					sscanf() analiza la cadena token y asigna el valor entero resultante a la variable cur_entry->uid.
					 Es útil cuando se desea convertir una representación de texto de un número en una variable numérica.
				*/
				if (sscanf(token, "%d", &cur_entry->uid) != 1)
				{
					fprintf(stderr, "Warning: Wrong format for UID field in line %d of /etc/passwd\n", cur_line);
					cur_entry->uid = 0;
				}

				break;
			case GID_IDX:
				if (sscanf(token, "%d", &cur_entry->gid) != 1)
				{
					fprintf(stderr, "Warning: Wrong format for GID field in line %d of /etc/passwd\n", cur_line);
					cur_entry->gid = 0;
				}
				break;
			case USER_NAME_IDX:
				// strdup hace exactamente lo mismo que la funcion clone_string
				cur_entry->user_name = strdup(token);
				// cur_entry->user_name = clone_string(token);
				break;
			case USER_HOME_IDX:
				cur_entry->user_home = clone_string(token);
				break;
			case USER_SHELL_IDX:
				/* Borra la nueva línea del token */
				token[strlen(token) - 1] = '\0';
				cur_entry->user_shell = clone_string(token);
				break;
			default:
				break;
			}
			token_id++;
		}
		if (token_id != NR_FIELDS_PASSWD)
		{
			fprintf(stderr, "Could not process all tokens from line %d of /etc/passwd\n", entry_idx + 1);
			return NULL;
		}
		cur_line++;
		entry_idx++;
	}
	(*nr_entries) = entry_count;
	return entries;
}

/**
 * Libera la memoria del array entries incluida la memoria dinámica
 * que está en los strings
 */
static void free_entries(passwd_entry_t *entries, int nr_entries)
{
	int i = 0;
	passwd_entry_t *entry;

	for (i = 0; i < nr_entries; i++)
	{
		entry = &entries[i]; /* Puntero a la entrada correspondiente */
		free(entry->optional_encrypted_passwd);
		free(entry->user_name);
		free(entry->user_home);
		free(entry->user_shell);
	}

	free(entries);
}

/**
 * Llama al parseo del archivo y escribe su contenido en
 * consola según el formato de options y por ultimo libera la memoria
 */
static int show_passwd(struct options *options)
{
	int nr_entries;
	int i;
	passwd_entry_t *entries = parse_passwd(options, &nr_entries);

	if (!entries)
		return EXIT_FAILURE;

	for (i = 0; i < nr_entries; i++)
	{
		passwd_entry_t *e = &entries[i]; /* Puntero a la entrada correspondiente */
		switch (options->output_mode)
		{
		case VERBOSE_MODE:
			fprintf(options->outfile, "[Entry #%d]\n", i);
			fprintf(options->outfile, "\tlogin=%s\n\tenc_pass=%s\n\t"
									  "uid=%d\n\tgid=%d\n\tuser_name=%s\n\t"
									  "home=%s\n\tshell=%s\n",
					e->login_name, e->optional_encrypted_passwd,
					e->uid, e->gid, e->user_name,
					e->user_home, e->user_shell);
			break;
		case PIPE_MODE:
			fprintf(options->outfile, "|%s|%s|%d|%d|%s|%s|%s|\n",
					e->login_name, e->optional_encrypted_passwd,
					e->uid, e->gid, e->user_name,
					e->user_home, e->user_shell);
			break;
		case CSV_MODE:
			fprintf(options->outfile, "%s,%s,%d,%d,%s,%s,%s\n",
					e->login_name, e->optional_encrypted_passwd,
					e->uid, e->gid, e->user_name,
					e->user_home, e->user_shell);
		}
	}
	free_entries(entries, nr_entries);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int retCode, opt;
	struct options options;

	/* Valores por defecto de las opciones */
	options.outfile = stdout;
	options.output_mode = VERBOSE_MODE;
	options.inputfile = "/etc/passwd\0";
	/* Parsea las opciones que se han puesto en la linea de comandos */
	// El parámetro optstring sirve para indicar de forma compacta a getopt() cuáles son las opciones que el programa acepta
	while ((opt = getopt(argc, argv, "hvpo:i:c")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -v | -p | -o <output_file> | -i <input_file> ]\n", argv[0]);
			exit(0);
		case 'v':
			options.output_mode = VERBOSE_MODE;
			break;
		case 'p':
			options.output_mode = PIPE_MODE;
			break;
		case 'o':
			if ((options.outfile = fopen(optarg, "wx")) == NULL)
			{
				fprintf(stderr, "The output file %s could not be opened: ",
						optarg);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			break;
		case 'i':
			options.inputfile = strdup(optarg);
			break;
		case 'c':
			options.output_mode = CSV_MODE;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	retCode = show_passwd(&options);
	free(options.inputfile);
	exit(retCode);
}
/****** PREGUNTAS DE LA PRACTICA ****
 * ¿Cuál es el propósito de la función clone_string() que se usa para inicializar algunos de los citados campos tipo cadena?
 * ¿Por qué no es posible en algunos casos simplemente copiar la cadena vía strcpy() o realizando una asignación campo=cadena_existente;?
 * 	+ clone_string copia una cadena a otra reservando antes un espacio en memoria. No sirve strcpy para todas porque no tienen ningun
 * 	tamaño en memoria asignado, y por tanto daría error en memoria. Para login_name si se puede usar strcpy porque es un array con un espacio
 *	en memoria ya asignado. No se puede utilizar campo = cadena_existente porque los strings son realmente punteros a posiciones de
 *	memoria y no se permite igualar directamente un puntero a otro.
 *
 * ¿Qué tipo de modificaciones sufre la cadena (variable line) tras invocaciones sucesivas de strsep()?
 *	+ Line va apuntando al siguiente nombre de usuario que precede a toda la información de etc/psswd
 * */