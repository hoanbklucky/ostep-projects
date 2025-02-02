#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char *line = NULL; // Pointer to store the line
    size_t len = 0;    // Variable to store the length of the line
    ssize_t read;      // Variable to store the number of characters read
    char *exitStr = "exit";
    char *token;       // Variable to store the token
    char *tokens[10];  // Array to store the tokens
    char *path[] = {"/bin", "/usr/bin"};

    while (1) {
        // Print a prompt "wish> "
        printf("wish> ");
        // Read the line entered by user
        if ((read = getline(&line, &len, stdin)) == -1) {
            break;
        }
        // Remove the newline character
        line[strcspn(line, "\n")] = 0;

        // Tokenize the input line
        int i = 0;
        token = strtok(line, " ");
        while (token != NULL && i < 10) {
            tokens[i++] = token;
            token = strtok(NULL, " ");
        }
        tokens[i] = NULL;

        // Check for exit command
        if (tokens[0] != NULL && strcmp(tokens[0], exitStr) == 0) {
            break;
        }

        // Check for cd command
        if (tokens[0] != NULL && strcmp(tokens[0], "cd") == 0) {
            if (tokens[1] != NULL) {
                if (chdir(tokens[1]) != 0) {
                    perror("chdir failed");
                }
            } else {
                fprintf(stderr, "cd: missing argument\n");
            }
            continue;
        }

        // Fork a child process to execute other commands
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            // Child process
            for (int i = 0; i < sizeof(path)/sizeof(path[0]); i++) {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path[i], tokens[0]);
                execv(full_path, tokens);
            }
            fprintf(stderr, "execv failed\n");
            exit(1);
        } else {
            // Parent process
            // Wait for the child process to finish
            int wc = wait(NULL);
            if (wc == -1) {
                fprintf(stderr, "wait failed\n");
            }
        }
    }

    free(line);
    return 0;
}