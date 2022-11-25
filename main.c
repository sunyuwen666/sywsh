#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SYW_TOK_DELIM "\t\r\n"

char * sywsh_read_line()
{
	char * line=NULL;
	ssize_t bufsize;
	getline(&line,&bufsize,stdin);
	return line;
}

char ** sywsh_split_line(char * line)

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
