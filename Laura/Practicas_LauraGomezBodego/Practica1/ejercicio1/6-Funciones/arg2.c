#include <stdio.h>

/************* TAREAS
 * Compila el código y ejecútalo. Responde:
 * 1. ¿Por qué cambia el valor de 'y' tras la llamada a suma()?
 * 		+ El valor cambia porque al haberle pasado un puntero a esa dirección, no se ha realizado una
 * 		copia de esta y por tanto la modificación que se ha realizado en el método, ha cambiado el valor de
 * 		la variable
 *
 * 2. ¿Por qué en ocasiones se usa el operador '.' y en otras '->' para acceder a los campos de una estructura?
 * 		+ Cuando se quiere acceder al campo de una variable local se utililza '.' y cuando se quiere acceder a el
 * 		de un puntero se utiliza '->'
 *
 * 3. ¿Por qué el valor de zc pasa a ser incorrecto sin volver a usarlo en el código?
 * 		+ Porque se le asigna la dirección de una variable local de la función que se asigna dentro de esta
 * 		y solo existe en ese contexto, por tanto al intentar accederla genera un error ya que esa dirección no existe en el 
 * 		contexto general del programa.
 * 4. Corrije el código para evitar el error producido en zc
 * 		+ He convertido r de sumC en una variable global 
 *******************/

/* Tipo estructurado */
struct _complex_
{
	float re;
	float im;
};
struct _complex_ r;

/* Declaración adelantada */
int sum(int *pa, int *pb);
struct _complex_ *sumC(struct _complex_ *a, struct _complex_ *b);

int main(void)
{
	int x = 4, y = 5;
	int *ptr = &y;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	struct _complex_ *zc;

	printf("Suma de complejos. (%f,%f i) + (%f,%f i) = ",
		   xc.re, xc.im, yc.re, yc.im);

	zc = sumC(&xc, &yc);
	printf("(%f,%f i)\n", zc->re, zc->im);

	int total = sum(&x, ptr);
	printf("Suma de enteros:  x +y = %d + %d = %d \n", x, y, total);
	printf("xc = (%f,%f i)  yc = (%f,%f i) zc = (%f,%f i)\n",
		   xc.re, xc.im, yc.re, yc.im, zc->re, zc->im);
	return 0;
}

int sum(int *pa, int *pb)
{
	/* args passed by reference */
	int c = *pa + *pb;

	*pa = 7;
	*pb = 8;
	return c; /* return by value */
}

struct _complex_ *sumC(struct _complex_ *a, struct _complex_ *b)
{
	r.re = a->re + b->re;
	r.im = a->im + b->im;

	// Modificamos el primer argumento
	a->re = 12.5;
	a->im = 13.4;
	return &r;
}
