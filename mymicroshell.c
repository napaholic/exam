//
// Created by jaewkim on 2022/09/21.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void ft_print_error(char *str)
{
	int i = 0;
	
	while (str[i])
		++i;
	write(2, str, i);
}

int ft_execve(char **av, int i, char **envp)
{
	av[i] = NULL;
	execve(av[0], av, envp);
	ft_print_error("error: cannot execute ");
	ft_print_error(av[0]);
	ft_print_error("\n");
	return (1);
}

int main(int ac, char **av, char **envp)
{
	int pid;
	int fd[2];
	int temp_fd = dup(STDIN_FILENO);
	int i = 0;
	
	av = &av[i + 1];
	i = 0;
	while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
		++i;
	if (!strcmp(av[0], "cd"))
	{
		if (i != 2)
			ft_print_error("error: cd: bad arguments\n");
		else if (chdir(av[1]) != 0)
		{
			ft_print_error("error: cd: cannot change directory to ");
			ft_print_error(av[1]);
			ft_print_error("\n");
		}
	}
	else if (i && (av[i] == NULL || !strcmp(av[i], ";")))
	{
		pid = fork();
		if (pid == 0)
		{
		
		}
	}
}