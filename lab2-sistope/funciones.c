#include "funciones.h"

/*
    Demis Huaccha - Rut: 21.288.793-5
    Jose Antonio Muñoz Alvarez - 21.154.079-6
*/ 


/*
 * Entradas: char *str - String a limpiar
 * Salidas: void
 * Descripción: Elimina espacios en blanco al inicio y final del string
 */
void trim(char *str) {
    char *start = str;
    while(isspace((unsigned char)*start)) start++;
    char *end = str + strlen(str) - 1;
    while(end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    if (start != str) memmove(str, start, strlen(start) + 1);
}

/*
 * Entradas: char *str - String con comando y argumentos
 * Salidas: char ** - Arreglo de strings (argv)
 * Descripción: Divide un string en argumentos respetando comillas
 */
char **parse_command(char *str) {
    char **argv = malloc(sizeof(char*) * MAX_ARGS);
    int argc = 0, len = strlen(str), arg_pos = 0, in_quote = 0;
    char *arg = malloc(len + 1);
    
    trim(str);
    
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (in_quote) {
            /* Si estamos dentro de comillas, seguimos hasta encontrar el cierre */
            if ((in_quote == 1 && c == '\'') || (in_quote == 2 && c == '"')) in_quote = 0;
            else arg[arg_pos++] = c;
        } else {
            /* Si no estamos en comillas, verificamos separadores o inicio de comillas */
            if (isspace(c)) {
                /* Fin de un argumento */
                if (arg_pos > 0) {
                    arg[arg_pos] = '\0';
                    argv[argc++] = strdup(arg);
                    arg_pos = 0;
                }
            } else if (c == '\'') in_quote = 1;
            else if (c == '"') in_quote = 2;
            else arg[arg_pos++] = c;
        }
    }
    
    if (arg_pos > 0) {
        arg[arg_pos] = '\0';
        argv[argc++] = strdup(arg);
    }
    
    argv[argc] = NULL;
    free(arg);
    return argv;
}

/*
 * Entradas: char *line - Línea de comando con pipes
 *           int *num_commands - Puntero para retornar cantidad de comandos
 * Salidas: char *** - Arreglo tridimensional con comandos y argumentos
 * Descripción: Parsea una línea de comandos separada por pipes
 */
char ***parse_pipeline(char *line, int *num_commands) {
    char ***commands = malloc(sizeof(char**) * MAX_ARGS); 
    *num_commands = 0;
    int len = strlen(line), in_quote = 0, buf_pos = 0;
    char *buffer = malloc(len + 1);
    
    for (int i = 0; i < len; i++) {
        char c = line[i];
        if (in_quote) {
            /* Ignoramos pipes si estan dentro de comillas */
            if ((in_quote == 1 && c == '\'') || (in_quote == 2 && c == '"')) in_quote = 0;
            buffer[buf_pos++] = c;
        } else {
             /* Si encontramos un pipe fuera de comillas, terminamos el comando actual */
            if (c == '|') {
                buffer[buf_pos] = '\0';
                commands[*num_commands] = parse_command(buffer);
                (*num_commands)++;
                buf_pos = 0;
            } else {
                if (c == '\'') in_quote = 1; else if (c == '"') in_quote = 2;
                buffer[buf_pos++] = c;
            }
        }
    }
    
    if (buf_pos > 0 || *num_commands > 0) {
        buffer[buf_pos] = '\0';
        char *temp = strdup(buffer);
        trim(temp);
        if (strlen(temp) > 0) {
             commands[*num_commands] = parse_command(buffer);
             (*num_commands)++;
        }
        free(temp);
    }
    
    free(buffer);
    return commands;
}

/*
 * Entradas: char ***commands - Estructura de comandos
 *           int num_commands - Cantidad de comandos
 * Salidas: void
 * Descripción: Libera la memoria de la estructura de comandos
 */
void free_pipeline(char ***commands, int num_commands) {
    for (int i = 0; i < num_commands; i++) {
        for (int j = 0; commands[i][j] != NULL; j++) free(commands[i][j]);
        free(commands[i]);
    }
    free(commands);
}
