/**
 * Laboratorio 2 - Sistemas Operativos
 * Integrantes:
 * - Nombre: [Nombre Estudiante 1] RUT: [RUT Estudiante 1]
 * - Nombre: [Nombre Estudiante 2] RUT: [RUT Estudiante 2]
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Entradas: No recibe entradas.
// Salidas: No retorna valor (void).
// Descripción: Imprime por pantalla las instrucciones de uso del programa y termina la ejecución.
void print_usage();

// Entradas:
//   - line: cadena de caracteres con el pipeline completo (modificada por strtok).
//   - cmds: arreglo de punteros donde se guardarán los comandos.
//   - max_cmds: número máximo de comandos permitidos.
// Salidas: Entero con el número de comandos parseados.
// Descripción: Separa la línea de comandos por el caracter '|'.
int split_line(char *line, char **cmds, int max_cmds);

// Entradas:
//   - cmd: cadena con el comando completo y sus argumentos.
//   - args: arreglo de punteros donde se guardarán los tokens.
//   - max_args: número máximo de argumentos.
// Salidas: Entero con el número de argumentos.
// Descripción: Tokeniza un comando por espacios y elimina comillas de los argumentos.
int parse_command_args(char *cmd, char **args, int max_args);

#endif
