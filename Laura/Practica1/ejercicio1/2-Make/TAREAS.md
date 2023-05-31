1. Examina el makefile, identifica las variables definidas, los objetivos
   (targets) y las regalas.
   + Aclaraciones puestas en el archivo makefile.

2. Ejecuta make en la linea de comandos y comprueba las ordenes que ejecuta para
   construir el proyecto.
   + Órdenes que ejecuta:
      gcc -Wall -g -c -o aux.o aux.c
      gcc -Wall -g -c -o init.o init.c
      gcc -Wall -g -o prueba aux.o init.o -lm

3. Marca el fichero aux.c como modificado ejecutando touch aux.c. Después
   ejecuta de nuevo make. ¿Qué diferencia hay con la primera vez que lo
   ejecutaste? ¿Por qué?
   + Solo ejecuta las siguientes órdenes:
      gcc -Wall -g -c -o aux.o aux.c
      gcc -Wall -g -o prueba aux.o init.o -lm
   Compila sólo los archivos que utilizan aux.c que ha sido tocado, el resto de archivos no necesitan recompilarse
   porque no se han cambiado y ya tienen sus archivos .o actualizados.

4. Ejecuta la orden make clean. ¿Qué ha sucedido?
   + Se han borrado todos los archivos intermedios y el ejecutable.

- Observa que el objetivo clean está marcado como phony en la orden .PHONY: clean. ¿por qué? 
   + El objetivo clean se marca como .PHONY para indicar que no es un archivo real que se debe construir, sino una acción o tarea a realizar. Al marcarlo como .PHONY, se le indica a la herramienta make que no busque un archivo llamado "clean" ni compruebe si existe, sino que simplemente ejecute la regla asociada al objetivo sin importar el estado de los archivos en el sistema.

- Para comprobarlo puedes comentar dicha línea del makefile, compilar de nuevo haciendo make, y
   después crear un fichero en el mismo directorio que se llame clean, usando el
   comando touch clean. Ejecuta ahora make clean, ¿qué pasa?
   + El makefile intenta compilar el archivo clean e ignora la orden de borrar los archivos que tiene el comando clean.

5. Comenta la linea LIBS = -lm poniendo delante una almoadilla (#). Vuelve a
   contruir el proyecto ejecutando make (haz un clean antes si es necesario).
   ¿Qué sucede? ¿Qué etapa es la que da problemas?
   + Da problemas al intentar ejecutar el comendo:  gcc -Wall -g -o prueba aux.o init.o 
   La compilación genera un error al intentar acceder a los archivos de la librería ya que no se ha enlazado y, por
   tanto, el compilador no la encuentra.
