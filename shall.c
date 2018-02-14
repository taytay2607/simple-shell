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

//command parser
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

//go in interactive mode, use user input 
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

        //splite command into each token 
        char** tokens;
        tokens = str_split(user_input, ';');

        if (tokens)
        {
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                printf("command=[%s]\n", *(tokens + i)); //use to debug parse 
                free(*(tokens + i));
            }
            printf("\n");
            free(tokens);
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