//include library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//define constant
#define PATH_MAX 1024
#define COMMAND_SIZE 1024
#define MAX_INPUT_LENGTH 2048

void interactive_mode(char cmd[]) {
    while(1){

        //allocate memory for user'path
        char current_path[PATH_MAX] = "";

        //show current path
        getcwd(current_path, PATH_MAX);
        printf("shall:~%s>", current_path);

        // allocation memory for user input (allocate buffer)
        fflush(stdout);
        char* user_input = (char*) malloc(MAX_INPUT_LENGTH);
        user_input = fgets(user_input, MAX_INPUT_LENGTH, stdin);
        if(!user_input){
            printf("shall:there was an error while reading user input.\n");
            free(user_input);
            exit(EXIT_FAILURE);
        }

    }
}

void batch_mode(char cmd[], char *argv[]) {
    printf("batch mode");
}

int main(int argc, char *argv[]){

    char cmd[COMMAND_SIZE]; 
    if (argc != 2)
        interactive_mode(cmd);
    else
        batch_mode(cmd, argv);
    
}