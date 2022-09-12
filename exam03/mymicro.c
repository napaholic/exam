//
// Created by jaewkim on 2022/08/31.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define ERR_argc "ERROR: argc count error"

int ft_strlen(char *str)
{
	int i;
	
	i = 0;
	while (str[i])
	{
		++i;
	}
	return i;
}

int print_err(char *str)
{
	write(1, str, ft_strlen(str));
	return 0;
}

int main(int ac, char **av)
{
	FILE *file;
	char *paper;
	int read, pos;
	int b_w, b_h;
	char color, id, background;
	float id_x, id_y, id_w, id_h;
	int
	
	if (ac != 2)
		return print_err(ERR_argc);
	if (!(file = fopen(av[1], "r")))
		return print_err("ERROR : file dir error");
	if (fscanf(file, "%d %d %c\n", &b_w, &b_h, &background) != 3)
		return print_err("ERR: background data is wrong");
	if (!(b_w > 0 && b_w <= 300) || !(b_h > 0 && b_h <= 300) || !(paper = (char *)malloc(sizeof(char) * (b_w * b_h))))
		return (print_err("ERR : background data is wrong"));
	memset(paper, background, b_w * b_h);
	while((read = fscanf(file, "%c %f %f %f %f %c\n", &id, &id_x, &id_y, &id_w, &id_h, &color)) == 6)
	{
		if (!(id_w > 0 && id_h > 0) || !(id == 'r' || id == 'R'))
			break;
		if ()
	}
	
}