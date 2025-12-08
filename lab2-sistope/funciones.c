/**
 * Laboratorio 2 - Sistemas Operativos
 * Integrantes:
 * - Nombre: [Nombre Estudiante 1] RUT: [RUT Estudiante 1]
 * - Nombre: [Nombre Estudiante 2] RUT: [RUT Estudiante 2]
 */

#include "funciones.h"

// Entradas: No recibe entradas.
// Salidas: No retorna valor (void).
// Descripción: Imprime por pantalla las instrucciones de uso del programa y termina la ejecución.
void print_usage() {
    fprintf(stderr, "Uso: el programa recibe el pipeline por stdin.\n");
    fprintf(stderr, "Ejemplo: echo \"./generator.sh ... | ./process ...\" | ./lab2 [-v]\n");
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "  -v : Modo verbose (imprime debug info)\n");
    exit(EXIT_FAILURE);
}

// Entradas:
//   - line: cadena de caracteres con el pipeline completo (modificada por strtok).
//   - cmds: arreglo de punteros donde se guardarán los comandos.
//   - max_cmds: número máximo de comandos permitidos.
// Salidas: Entero con el número de comandos parseados.
// Descripción: Separa la línea de comandos por el caracter '|'.
int split_line(char *line, char **cmds, int max_cmds) {
    int num_cmds = 0;
    char *token = strtok(line, "|");
    while (token != NULL && num_cmds < max_cmds) {
        cmds[num_cmds++] = token;
        token = strtok(NULL, "|");
    }
    return num_cmds;
}

// Entradas:
//   - cmd: cadena con el comando completo y sus argumentos.
//   - args: arreglo de punteros donde se guardarán los tokens.
//   - max_args: número máximo de argumentos.
// Salidas: Entero con el número de argumentos.
// Descripción: Tokeniza un comando por espacios y elimina comillas de los argumentos.
int parse_command_args(char *cmd, char **args, int max_args) {
    int num_args = 0;
    char *arg_token = strtok(cmd, " "); 
    while (arg_token != NULL && num_args < max_args - 1) {
        // Logica para eliminar comillas al inicio y final
        size_t len = strlen(arg_token);
        if (len >= 2) {
            if ((arg_token[0] == '"' && arg_token[len-1] == '"') || 
                (arg_token[0] == '\'' && arg_token[len-1] == '\'')) {
                arg_token[len-1] = '\0';
                arg_token++;
            }
        }
        args[num_args++] = arg_token;
        arg_token = strtok(NULL, " ");
    }
    args[num_args] = NULL;
    return num_args;
}
