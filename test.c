#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int ac, char **av, char **ev)
{
	av = &av[1];
	av[ac] = NULL;
	execve(av[0], av, ev);
}
