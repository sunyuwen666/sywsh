//原作者：bilibili克拉克黎明之前
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SYW_TOK_DELIM " \t\r\n"
#define SYW_TOK_BUFFER_SIZE 64

int sywsh_cd(char **args);
int sywsh_help(char **args);
int sywsh_exit(char **args);

int sywsh_builtin_nums();

char * builtin_cmd[] =
{
    "cd",
    "help",
    "exit"
};

int (*builtin_func[])(char **) = 
{
    &sywsh_cd,
    &sywsh_help,
    &sywsh_exit
};

int sywsh_cd(char **args)
{
    if(args[1] == NULL) 
    {
        perror("sywsh error at cd, lack of args\n");
    }
    // cd 
    else
    {
        if(chdir(args[1]) != 0)
            perror("sywsh error at chdir\n");
    }
    return 1;
}

int sywsh_help(char **args)
{
    puts("This is Syw's shell");
    puts("Here are some built in cmd:");
    for (int i = 0; i < sywsh_builtin_nums(); i ++)
        printf("%s\n", builtin_cmd[i]);
    return 1;
}

int sywsh_exit(char **args)
{
    return 0;
}

int sywsh_builtin_nums()
{
    return sizeof(builtin_cmd) / sizeof(builtin_cmd[0]);
}

char * sywsh_read_line()
{
    char * line = NULL;
    ssize_t bufsize;
    getline(&line, &bufsize, stdin);
    return line;
}

//parsing 


char ** sywsh_split_line(char * line)
{
    int buffer_size = SYW_TOK_BUFFER_SIZE, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;
    // "cd /desk/main"
    token = strtok(line, SYW_TOK_DELIM);
    while(token != NULL)
    {
        tokens[position++] = token;
        token = strtok(NULL, SYW_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int sywsh_launch(char **args)
{
    // execvp
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if(execvp(args[0], args) == -1)
            perror("sywsh error at execvp\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int sywsh_execute(char **args)
{
    if(args[0] == NULL) return 1;
    
    for (int i = 0; i < sywsh_builtin_nums(); i ++)
        if(strcmp(args[0], builtin_cmd[i]) == 0)
            return (*builtin_func[i])(args);
    return sywsh_launch(args);
}

void sywsh_loop()
{
    char *line;
    // "cd /desk/main"
    // "cd" "/desk/main"
    char **args;
    int status;

    do
    {
        char path[100];
        getcwd(path, 100);
        char now[200] = "[sywsh ";
        strcat(now, path);
        strcat(now, " ]# ");
        printf("%s", now);

        line = sywsh_read_line();
        args = sywsh_split_line(line);
        status = sywsh_execute(args);

        free(line);
        free(args);
    } while (status);
    
}

int main(int argc, char *argv[])
{
    sywsh_loop();
    return 0;
}
