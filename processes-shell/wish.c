#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char *line = NULL; // Pointer to store the line
    size_t len = 0;    // Variable to store the length of the line
    size_t read;       // Variable to store the number of characters read
    char *exitStr = "exit";
    char *token;       // Variable to store the token
    char *tokens[10]; // Variable to store the token
    char *path[] = {"/bin", "/usr/bin"};
    
    // print a prompt "wish> "
    printf("wish> ");
    // read the line entered by user
    while ((read = getline(&line, &len, stdin)) != -1) {
        // remove the newline character
        line[strcspn(line, "\n")] = 0;
        printf("You entered: %s\n", line);
        // Exit if user entered exit\n
        if (strcmp(line, exitStr) == 0) {
            printf("Exiting");
            break;
        }
        // Parse the command
        int arg_count = 0;
        while ((token = strsep(&line, " ")) != NULL) {
            printf("%s\n", token);
            tokens[arg_count] = token;
            arg_count++;
        }
        // Fork a child process to execute the command
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            // Child process
            // Execute the command
            for (int i = 0; i < sizeof(path)/sizeof(path[0]); i++) {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path[i], tokens[0]);
                printf("Full path: %s\n", full_path);
                if (strcmp(tokens[0], "ls") == 0) {
                    char *args[] = {"ls", NULL};
                    execv(full_path, args);
                } else if (strcmp(tokens[0], "cd") == 0) {
                    if (tokens[1] != NULL) {
                        if (chdir(tokens[1]) != 0) {
                            perror("chdir failed");
                        }
                    } else {
                        fprintf(stderr, "cd: missing argument\n");
                    }
                    exit(0); // Exit after handling cd
                }
                
                printf("execv failed\n");
            }
        } else {
            // Parent process
            // Wait for the child process to finish
            int wc = wait(NULL);
            printf("wish> ");
        }

        
    }

    free(line);
    return 0;

}