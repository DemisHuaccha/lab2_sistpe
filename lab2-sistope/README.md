# Lab 2: Monitoreo y Procesamiento de Procesos con Bash y Pipes

Este proyecto implementa un orquestador de 'pipelines' en C que ejecuta una serie de scripts de Bash para monitorear y procesar información del sistema.

## Compilación

Para compilar el programa, utiliza el `Makefile` incluido:

```bash
make
```

Esto generará el ejecutable `lab2`.

## Ejecución

El programa se ejecuta pasando la pipeline como argumentos de línea de comandos.

### Argumentos de Línea de Comandos
Puedes pasar la pipeline completa como un único argumento (entre comillas) al programa.

```bash
./lab2 "./generator.sh -i 1 -t 10 | ./preprocess.sh | ./filter.sh -c 10 -m 5 -r \"^(python|chrome)$\" | ./transform.sh --anon-uid | ./aggregate.sh | ./report.sh -o reporte.tsv"
```

Si el programa se ejecuta sin argumentos, mostrará el siguiente mensaje y terminará:

```bash
./lab2
no se recibio linea de comando a ejecutar
```

## Estructura del Proyecto

- `lab2.c`: Programa principal. Lee la entrada, parsea los comandos y gestiona los procesos hijos con `fork`, `execvp` y `pipe`.
- `funciones.c`: Funciones auxiliares para el manejo de strings (parsing, trim, split).
- `funciones.h`: Cabeceras de las funciones auxiliares.
- `*.sh`: Scripts de procesamiento (generator, filter, etc.).
- `Makefile`: Script de compilación.

## Requisitos
- Sistema operativo Linux.
- GCC y Make instalados.
