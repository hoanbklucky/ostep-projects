// reverse a file
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fptr;
    if (argc == 2) {
        //print the first argument
        printf("Input file %s\n", argv[1]);
        //open the file specified in the first argument
        fptr = fopen(argv[1], "r");
        printf("Before fseek: fptr = %p\n", fptr);
        //get size of the file
        if (fptr == NULL) {
            perror("fopen");
            return 1;
        }
        fseek(fptr, 0, SEEK_END);
        printf("After fseek: fptr = %p\n", fptr);
        long size = ftell(fptr);
        //create buffer
        char* buffer = (char*) malloc(size);
        printf("File size: %ld\n", size);

        //read into buffer
        fread(buffer, 1, size, fptr);
        //create output file
        FILE* out_fptr = fopen("reversed_foo.txt", "w");
        //write to output file
        for (long i = size -1; i >= 0; i--) {
            printf("%c\n", buffer[i]);
            fputc(buffer[i], out_fptr);
        }

        
        fclose(fptr);
        fclose(out_fptr);
        
    }
}