#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_ARGS 64

/*
 * Entradas: char *line - Línea de comando con pipes
 *           int *num_commands - Puntero para retornar cantidad de comandos
 * Salidas: char *** - Arreglo tridimensional con comandos y argumentos
 * Descripción: Parsea una línea de comandos separada por pipes
 */
char ***parse_pipeline(char *line, int *num_commands);

/*
 * Entradas: char ***commands - Estructura de comandos
 *           int num_commands - Cantidad de comandos
 * Salidas: void
 * Descripción: Libera la memoria de la estructura de comandos
 */
void free_pipeline(char ***commands, int num_commands);

#endif
