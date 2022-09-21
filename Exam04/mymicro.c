//
// Created by jaewkim on 2022/09/21.
//

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#define READ 0
#define WRITE 1

void ft_print_error(char *str)
{
	int i = 0;
	
	while (str[i])
		++i;
	write(2, str, i);
}

int ft_execve(char **argv, int i, char **envp)
{
	argv[i] = NULL;
	ft_print_error(argv[0]);
	execve(argv[0], argv, envp);
	ft_print_error("error : cannot execute ");
	ft_print_error(argv[0]);
	ft_print_error("\n");
	return (1);
}

int main(int ac, char **av, char **envp)
{
	int pid;
	int fd[2];
	int temp_fd = dup(STDIN_FILENO);
	int i = 0;
	
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			++i;
		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				ft_print_error("error: cd: bad arguments\n");
			else if (chdir(av[1])	!= 0)
			{
				ft_print_error("error: cd: cannot change directory to path_to_change ");
				ft_print_error(av[1]);
				ft_print_error("\n");
			}
		}
		else if (i && (av[i] == NULL || !strcmp(av[i], ";")))
		{
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[WRITE], STDIN_FILENO);
				if (ft_execve(av, i, envp))
					return (1);
			}
			else
				waitpid(pid, NULL, 0);
			dup2(temp_fd, STDIN_FILENO);
		}
		else if (i && !strcmp(av[i], "|"))
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[WRITE], STDOUT_FILENO);
				close(fd[WRITE]);
				if (ft_execve(av, i, envp))
					return (1);
			}
			else
			{
				close(fd[WRITE]);
				waitpid(pid, NULL, 0);
				dup2(fd[READ], STDIN_FILENO);
				close(fd[READ]);
			}
		}
	}
	close(temp_fd);
	return (0);
}