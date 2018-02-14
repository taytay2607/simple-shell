//include library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

//define constant
#define PATH_MAX 1024
#define COMMAND_SIZE 1024
#define MAX_INPUT_LENGTH 2048
#define STDIN 0
#define STDOUT 1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

//string parser
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

//go in interactive mode, use user'input for program'input
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

        /*  because fgets() get '\n' in to string.so that  
            we need to remove it */
          if(user_input[strlen(user_input) - 1] == '\n')
            user_input[strlen(user_input) - 1] = '\0';

        
        /********************************
        //parse command
        ********************************/

        //splite input string into command line
        char** commands;
        commands = str_split(user_input, ';');

        if (commands)
        {
            int i;
            for (i = 0; *(commands + i); i++)
            {
                
                if ((strcmp(*(commands + i), "quit") == 0)||(strcmp(*(commands + i), "exit") == 0)) exit(1);
                
                //printf("command=[%s]\n", *(commands + i)); //use to debug command 
                
                //fork process
                pid_t child_pid, wpid;
                int status = 0;

                //for child process only 
			    if ((child_pid = fork()) == 0) {

                    //splite command into each token
                    char** tokens;
                    tokens = str_split(*(commands + i), ' ');

                    if (tokens)
                    {
                        int i;
                        for (i = 0; *(tokens + i); i++)
                        {
                            //printf("token=[%s]\n", *(tokens + i)); //use to debug parse 
                        }
                        *(tokens + i)=NULL;
                        execvp(tokens[0],(tokens));
                        //if error command
                        perror("Error ");
                        free(tokens);
                    }
                    exit(0);
                }

                //parent process wait for all child process
                while ((wpid = wait(&status)) > 0);

                //free memory for command
                free(*(commands + i));
            }
            free(commands);
        }
    }
}


void batch_mode(char cmd[], char *argv[]) {

    //open file 
    FILE * fp;
    fp = fopen(argv[1], "r");

    //if file is null exit programs
    if (fp == NULL){
	    printf("file not exits");
        exit(EXIT_FAILURE);
 	}

    //read from file line by line 
    char buffer[MAX_INPUT_LENGTH];
    while(fgets(buffer, MAX_INPUT_LENGTH, fp)) {

        /*  because fgets() get '\n' in to string.so that  
            we need to remove it */
          if(buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        /********************************
        //parse command
        ********************************/

        //splite input string into command line
        char** commands;
        commands = str_split(buffer, ';');

        if (commands)
        {
            int i;
            for (i = 0; *(commands + i); i++)
            {
                
                if ((strcmp(*(commands + i), "quit") == 0)||(strcmp(*(commands + i), "exit") == 0)) exit(1);
                
                //printf("command=[%s]\n", *(commands + i)); //use to debug command 
                
                //fork process
                pid_t child_pid, wpid;
                int status = 0;

                //for child process only 
			    if ((child_pid = fork()) == 0) {

                    //splite command into each token
                    char** tokens;
                    tokens = str_split(*(commands + i), ' ');

                    if (tokens)
                    {
                        int i;
                        for (i = 0; *(tokens + i); i++)
                        {
                            //printf("token=[%s]\n", *(tokens + i)); //use to debug parse 
                        }
                        //redirect to file
                        int fd2 = open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                        dup2(fd2, 1);
                        close(fd2);
                        //exe command
                        *(tokens + i)=NULL;
                        execvp(tokens[0],(tokens));
                        //if error command
                        perror("Error ");
                        free(tokens);
                    }
                    exit(0);
                }

                //parent process wait for all child process
                while ((wpid = wait(&status)) > 0);

                //free memory for command
                free(*(commands + i));
            }
            free(commands);
        }

    }
    //close(fp);

}

int main(int argc, char *argv[]){

    char cmd[COMMAND_SIZE]; 
    if (argc == 1)
        interactive_mode(cmd);
    else if(argc == 3)
        batch_mode(cmd, argv);
    else
        printf("wrong input");
    
}