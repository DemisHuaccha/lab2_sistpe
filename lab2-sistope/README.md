# Laboratorio 2: Monitoreo y Procesamiento con Bash y Pipes (Refactorizado)

Este programa implementa un "orquestador" de procesos en C que recibe una cadena de comandos por `stdin`, crea los procesos hijos correspondientes mediante `fork()`, y los comunica mediante `pipe()` y `dup2()`.

## Integrantes
- Nombre: [Nombre Estudiante 1] RUT: [RUT Estudiante 1]
- Nombre: [Nombre Estudiante 2] RUT: [RUT Estudiante 2]

## Compilación

Para compilar el programa `lab2` y las funciones auxiliares, utilice el archivo `Makefile` proporcionado:

```bash
make
```

Esto generará el ejecutable `lab2`.

Para limpiar los archivos compilados:
```bash
make clean
```

## Ejecución

El programa `lab2` diseñado para esta entrega NO utiliza banderas de línea de comandos para la configuración de los scripts (como `-i` o `-t`), sino que **lee la línea de comandos completa desde la entrada estándar (stdin)**. Esto permite definir la pipeline de forma dinámica.

### Sintaxis

```bash
echo "cmd1 arg1 ... | cmd2 arg2 ... | ... " | ./lab2
```

Ó

```bash
cat archivo_comandos.txt | ./lab2
```

### Opciones de ./lab2

- `-v` : Modo verbose. Imprime información de depuración sobre la creación de hijos y argumentos pareseados.

## Ejemplos de Uso

### 1. Ejecución completa del pipeline
Este ejemplo ejecuta los 6 scripts en cadena. Note cómo se pasan los argumentos en la cadena enviada por `echo`.

```bash
echo "./generator.sh -i 1 -t 5 | ./preprocess.sh | ./filter.sh -c 0 -m 0 -r \".*\" | ./transform.sh --anon-uid | ./aggregate.sh | ./report.sh -o reporte.csv" | ./lab2 -v
```

- **generator.sh**: Genera datos cada 1 segundo por 5 segundos.
- **preprocess.sh**: Limpia los datos.
- **filter.sh**: Filtra (aquí con umbrales 0 para dejar pasar todo). Nota: El regex `".*"` debe ir entre comillas escapadas si se usa `echo` para protegerlo de la shell antes de que llegue a `lab2`. `lab2` se encarga de quitar las comillas al procesar.
- **transform.sh**: Anonimiza el UID.
- **aggregate.sh**: Agrupa por nombre de proceso.
- **report.sh**: Escribe el resultado en `reporte.csv`.

### 2. Ejecución parcial
Solo generar y preprocesar:

```bash
echo "./generator.sh -i 1 -t 2 | ./preprocess.sh" | ./lab2
```

## Solución de Problemas

- **Permisos**: Asegúrese de que los scripts `.sh` tengan permisos de ejecución:
  ```bash
  chmod +x *.sh
  ```
- **Line Endings**: Si los scripts fueron editados en Windows, pueden tener caracteres `\r`. El código entregado ya fue convertido, pero si edita, asegúrese de usar formato Unix (LF).
