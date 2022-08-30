//
// Created by jaewkim on 2022/08/30.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define ERR1 "Error: arg wrong\n"
#define ERR2 "Error: operation file is wrong\n"

int ft_strlen(char *s)
{
	int count;
	
	count = 0;
	while (*s) {
		++s;
		++count;
	}
	return (count);
}

int ft_perror(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int chk_pos(float x, float y, float id_x, float id_y, float w, float h)
{
	if (x < id_x || x > id_x + w || y < id_y || y > id_y + h)
		return (0);
	
}