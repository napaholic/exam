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
	else if (x - id_x < 1.000000 || (id_x + w) - x < 1.000000 || y - id_y < 1.000000
	|| (id_y + h) - y < 1.000000)
		return (1);
	return (2);
}

int main(int ac, char **av)
{
	FILE *file;
	char *paper;
	int read, pos;
	int y, x;
	int b_width, b_height;
	char background;
	char id, color;
	float id_x, id_y, w, h;
	
	if (ac != 2)
		return (ft_perror(ERR1));
	if (!(file = fopen(av[1], "r")) || (fscanf(file, "%d %d %c\n", &b_width, &b_height, &background) != 3) || \
	(!(b_width > 0 && b_width <= 300 && b_height > 0 && b_height < 300)) || \
	(!(paper = (char *)malloc(sizeof(char) * (b_width * b_height)))))
	return (ft_perror(ERR1));
	memset(paper, background, b_width * b_height);
	while ((read = fscanf(file, "%c %f %f %f %f %c\n", &id, &id_x, &id_y, &w, &h, &color)) == 6)
	{
		if (!(w > 0 && h > 0) || !(id == 'R' || id == 'r'))
			break;
		y = -1;
		while (++y < b_height)
		{
			x = -1;
			while (++x < b_width)
			{
				pos = chk_pos((float)x, (float)y, id_x, id_y, w, h);
				if (pos == 1 || (pos == 2 && id == 'R'))
					paper[y * b_width + x] = color;
			}
		}
	}
}