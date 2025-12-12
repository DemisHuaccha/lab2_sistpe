# Lab 2: Monitoreo y Procesamiento de Procesos con Bash y Pipes

Demis Huaccha - Rut: 21.288.793-5
Jose Antonio Muñoz Alvarez - 21.154.079-6

Signo "$" indica comando en terminal

Flags y argumentos de scripts se mantienen iguales a los del primer laboratorio.

## Compilación

Para compilar el programa, utilizando el `Makefile` se debe realizar el siguiente comando:

- $ make

y tambien se necesita dar permisos de ejecución a los scripts .sh: 

- $ chmod +x *.sh

Esto generará el ejecutable `lab2`.

## Ejecución

El programa se ejecuta pasando la pipeline como argumentos de línea de comandos.

### Argumentos de Línea de Comandos

Todos los scripts .sh deben comenzar con ./

Ejemplo de comando de ejecucion:

- $ ./lab2 ./generator.sh -i 1 -t 10 | ./preprocess.sh | ./filter.sh -c 10 -m 5 -r "sh" | ./transform.sh --anon-uid | ./aggregate.sh | ./report.sh -o reporte.tsv


Si el programa se ejecuta sin argumentos, mostrará el siguiente mensaje y terminará:

- "no se recibio linea de comando a ejecutar"


## Estructura del Proyecto

- `lab2.c`: Programa principal. Lee la entrada, parsea los comandos y gestiona los procesos hijos con `fork`, `execvp` y `pipe`.
- `funciones.c`: Funciones auxiliares para el manejo de strings (parsing, trim, split).
- `funciones.h`: Cabeceras de las funciones auxiliares.
- `*.sh`: Scripts bash (generator, filter, etc.).
- `Makefile`: Script de compilación.

## Requisitos
- Sistema operativo Linux.
- GCC y Make instalados.
