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

int main(int argc, char *argv[]){

    char current_path[PATH_MAX] = "";
    getcwd(current_path, PATH_MAX);
    printf("%s>", current_path);

    //char cmd[COMMAND_SIZE]; 
	/*
    if (argc != 2)
        shellCommand(cmd);
    else
        bashCommand(cmd, argv); 
    */
}