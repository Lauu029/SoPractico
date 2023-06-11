#include <stdio.h>

typedef struct
{
    char data[4096];
    int key;
} item;

item array[] = {
    {"bill", 3},
    {"neil", 4},
    {"john", 2},
    {"rick", 5},
    {"alex", 1},
};

/*Ordena el array, primero almacena un auxiliar p y lo iguala al array,
   y dentro del for hace lo siguiente, con j solamente cuenta de atrás a alante para
    comprobar que se han recorrido todos los elementos del array. Dentro del do-while 
    compara el primer elemento del array p con el siguiente, si es mayor los intercambia.
    Avanza uno en el array (que luego volverá a ponerlo en el primer elemento en la siguiente vuelta del for)
    si al volver al for s==0 quieere decir que ya no hace falta ordenaar más y se sale del for*/

//Igualar 'p' a 'a' hace que se vaya modificando 'a' sin que se pierda la referencia al primer valor, 
// así se puede ir seteando la dirección a la que apunta 'p' a la inicial en cada vuelta del bucle.
void sort(item *a, int n)
{
    int i = 0, j = 0;
    int s = 1;
    item *p;

    for (; (i < n) && (s != 0); i++)
    {
        s = 0;
        p = a;
        j = n - 1;
        do
        {
            if (p->key > (p + 1)->key)
            {
                item t = *p;
                *p = *(p + 1);
                *(p + 1) = t;
                s++;
            }
            p++;

        } while (--j > 0);
    }
}

int main()
{
    int i;
    sort(array, 5);
    for (i = 0; i < 5; i++)
        printf("array[%d] = {%s, %d}\n",
               i, array[i].data, array[i].key);
    return 0;
}
