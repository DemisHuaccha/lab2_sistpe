/**
 * Laboratorio 2 - Sistemas Operativos
 * Integrantes:
 * - Nombre: [Nombre Estudiante 1] RUT: [RUT Estudiante 1]
 * - Nombre: [Nombre Estudiante 2] RUT: [RUT Estudiante 2]
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <getopt.h>
#include "funciones.h"

#define MAX_LINE 2048
#define MAX_CMDS 20
#define MAX_ARGS 50

int verbose = 0;

int main(int argc, char *argv[]) {
    int opt;
    
    char line[MAX_LINE] = {0};
    int use_cli = 0;

    // Variables for CLI flags
    char *i_val = NULL;
    char *t_val = NULL;
    char *c_val = NULL;
    char *m_val = NULL;
    char *r_val = NULL;
    char *o_val = NULL;

    // Reset optind to 1 to rescan
    optind = 1;

    // Capture flags for pipeline construction
    while ((opt = getopt(argc, argv, "vi:t:c:m:r:o:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'i': i_val = optarg; use_cli = 1; break;
            case 't': t_val = optarg; use_cli = 1; break;
            case 'c': c_val = optarg; use_cli = 1; break;
            case 'm': m_val = optarg; use_cli = 1; break;
            case 'r': r_val = optarg; use_cli = 1; break;
            case 'o': o_val = optarg; use_cli = 1; break;
            default:
                // Ignore unknown options or handle error?
                // For now, let's assume valid usage
                break;
        }
    }

    if (use_cli) {
        // Construct pipeline from flags
        // Format: ./generator.sh -i [I] -t [T] | ./preprocess.sh | ./filter.sh -c [C] -m [M] -r "[R]" | ./transform.sh --anon-uid | ./aggregate.sh | ./report.sh -o [O]
        
        // Default values handling could be here or done by scripts.
        // We will pass the flags if they exist.
        
        // Generator + Preprocess (Always present in static flow?)
        // Let's assume the full 6 stage pipeline as per legacy request
        
        char gen_cmd[256] = "./generator.sh";
        if (i_val) { strcat(gen_cmd, " -i "); strcat(gen_cmd, i_val); }
        if (t_val) { strcat(gen_cmd, " -t "); strcat(gen_cmd, t_val); }
        
        char filter_cmd[256] = "./filter.sh";
        if (c_val) { strcat(filter_cmd, " -c "); strcat(filter_cmd, c_val); }
        if (m_val) { strcat(filter_cmd, " -m "); strcat(filter_cmd, m_val); }
        if (r_val) { strcat(filter_cmd, " -r \""); strcat(filter_cmd, r_val); strcat(filter_cmd, "\""); }
        
        char report_cmd[256] = "./report.sh";
        if (o_val) { strcat(report_cmd, " -o "); strcat(report_cmd, o_val); }

        snprintf(line, sizeof(line), 
            "%s | ./preprocess.sh | %s | ./transform.sh --anon-uid | ./aggregate.sh | %s",
            gen_cmd, filter_cmd, report_cmd);

        if (verbose) fprintf(stderr, "Modo CLI detectado. Pipeline construido: %s\n", line);

    } else {
        // 2. Leer pipeline desde stdin (Legacy User Request)
        if (verbose) fprintf(stderr, "Esperando input desde stdin...\n");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            return 0; 
        }
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) return 0;
        if (verbose) fprintf(stderr, "Pipeline recibido: %s\n", line);
    }

    // 3. Parsear comandos (usando función desacoplada)
    char *cmds[MAX_CMDS];
    int num_cmds = split_line(line, cmds, MAX_CMDS);

    if (num_cmds == 0) return 0;

    // 4. Crear procesos y pipes (Lógica principal en main para cumplir objetivos de SO)
    int prev_pipe_fd = -1;
    int curr_pipe[2];
    pid_t pid;

    for (int i = 0; i < num_cmds; i++) {
        // Crear pipe si no es el último
        if (i < num_cmds - 1) {
            if (pipe(curr_pipe) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Proceso Hijo
            // Configurar stdin
            if (prev_pipe_fd != -1) {
                if (dup2(prev_pipe_fd, STDIN_FILENO) == -1) {
                    perror("dup2 stdin");
                    exit(EXIT_FAILURE);
                }
                close(prev_pipe_fd);
            }

            // Configurar stdout
            if (i < num_cmds - 1) {
                close(curr_pipe[0]);
                if (dup2(curr_pipe[1], STDOUT_FILENO) == -1) {
                    perror("dup2 stdout");
                    exit(EXIT_FAILURE);
                }
                close(curr_pipe[1]);
            }

            // Parsear argumentos (función helper)
            char *args[MAX_ARGS];
            int num_args = parse_command_args(cmds[i], args, MAX_ARGS);

            if (num_args == 0) exit(EXIT_SUCCESS);

            if (verbose) fprintf(stderr, "[Hijo %d] Ejecutando: %s\n", getpid(), args[0]);

            execvp(args[0], args);
            perror("execvp"); 
            exit(EXIT_FAILURE);

        } else { // Proceso Padre
            if (prev_pipe_fd != -1) {
                close(prev_pipe_fd);
            }
            if (i < num_cmds - 1) {
                close(curr_pipe[1]);
                prev_pipe_fd = curr_pipe[0];
            }
        }
    }

    // Esperar a todos los hijos
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return 0;
}
