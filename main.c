#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SYW_TOK_DELIM "\t\r\n"
#define SYW_TOK_BUFFER_SIZE 64

int sywsh_cd(char **args);
int sywsh_help(char **args);
int sywsh_exit(char **args);

char * builtin_cmd[] =
{
	"cd",
	"help",
	"help"
};

int sywsh_buildin_nums()
{
	return sizeof (builtin_cmd) / sizeof(builtin_cmd[0]);
}

int (*builtin_func[])(char **)=
{
	&sywsh_cd,
	&sywsh_help,
	&sywsh_exit
};

int sywsh_cd(char **args)
{
	if(args[1]==NULL)
	{
		perror("Sywsh error at cd,lack of args\n");
	}
	
	else
	{
		if(chdir(args[1])!=0)
			perror("Sywsh error at chdir\n");
	}
	return 1;
}

char * sywsh_read_line()
{
	char * line=NULL;
	ssize_t bufsize;
	getline(&line,&bufsize,stdin);
	return line;
}

char ** sywsh_split_line(char * line)
{
	int buffer_size = SYW_TOK_BUFFER_SIZE;
	char **tokens = malloc(buffer_size * sizeof(char *));
	char *token;
	token = strtok(line,SYW_TOK_DELIM);
	while(token != NULL)
	{
		tokens[position++]=token;
		token = strtok(NULL,SYW_TOK_DELIM);
	}
	token[position]=NULL;
	return tokens;
}

int sywsh_launch(char **args)
	
int sywsh_execute(char **args)
{
	if(args[0]==NULL)	return 1;
	for(int i=0;i<sywsh_bulidin_nums();i++)
		if(strcmp(args[0],builtin_cmd[i])==0)
			return(buildin_func[i])(args);
	return sywsh_launch(args);
}

void sywsh_loop()
{
	char *line; //输入
	char **args; //参数
	int status=1; //状态
	
	do
	{
		char path[100]; //路径
		getcwd(path,100); //获取路径
		char now[200]="[sywsh:"; //自己sh的特点
		strcat(now,path); //拼接储存到now
		strcat(now,"]#");
		printf("%s",now);
		
		
		line = sywsh_read_line();
		args = sywsh_split_line(line);
		status = sywsh_execute(args);
		free(line);
		free(args);
	} while (status);
}

int main(int argc, char* argv[])
{
	sywsh_loop();
	return 0;
}
