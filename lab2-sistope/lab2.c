#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funciones.h"

#define CHECK_ERROR(cond, msg) do { if (cond) { perror(msg); exit(EXIT_FAILURE); } } while(0)

/*
 * Entradas: int argc - Cantidad de argumentos
 *           char *argv[] - Argumentos de línea de comandos
 * Salidas: int - 0 si éxito
 * Descripción: Función principal que orquesta la ejecución de la pipeline
 */
int main(int argc, char *argv[]) {
    char line[4096] = {0};
    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            strncat(line, argv[i], sizeof(line) - strlen(line) - 1);
            if (i < argc - 1) strncat(line, " ", sizeof(line) - strlen(line) - 1);
        }
    } else {
        if (isatty(STDIN_FILENO)) {
            printf("Ingrese comando: ");
            fflush(stdout);
        }
        if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    }
    
    line[strcspn(line, "\n")] = 0;
    if (strlen(line) == 0) return 0;

    int num_commands = 0;
    char ***commands = parse_pipeline(line, &num_commands);
    if (!num_commands) { free_pipeline(commands, num_commands); return 0; }
    
    int pipefd[2], prev_fd = -1;

    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) CHECK_ERROR(pipe(pipefd) == -1, "pipe");

        pid_t pid = fork();
        CHECK_ERROR(pid == -1, "fork");

        if (pid == 0) {
            if (prev_fd != -1) { dup2(prev_fd, STDIN_FILENO); close(prev_fd); }
            if (i < num_commands - 1) { close(pipefd[0]); dup2(pipefd[1], STDOUT_FILENO); close(pipefd[1]); }
            execvp(commands[i][0], commands[i]);
            perror("execvp"); exit(EXIT_FAILURE);
        } else {
            if (prev_fd != -1) close(prev_fd);
            if (i < num_commands - 1) { close(pipefd[1]); prev_fd = pipefd[0]; }
        }
    }
    
    for (int i = 0; i < num_commands; i++) wait(NULL);
    free_pipeline(commands, num_commands);
    return 0;
}
