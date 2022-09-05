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

int main(int ac, char **av)
{
    char id_c, r, background;
    int b_w, b_h;
    int id_w, id_h, id_x, id_y;
    int pos
    FILE *file;
    char *paper;
    int read, pos;
    int x, y;

    if (ac != 2)
        return (ft_perror("argument count is error"));
    if (!(file = fopen(av[1], "r")))
        return (ft_perror("file dir is wrong"));
    if ((fscanf(file, "%d %d %c\n", &b_w, &b_h), &background) != 3)
        return (ft_perror("background data is wrong"));
    if (!(b_w > 0 && b <= 300 && b_h > 0 && b_h <= 300))
        return (ft_perror("background data overflow"));
    if (!(paper = (char *)malloc(sizeof(char) * (b_w * b_h))))
        return (ft_perror("malloc error"));
    memset(paper, background, b_w * b_h);
    while ((read = fscanf(file, "%c %f %f %f %f %c\n", &id, &id_x, &id_y, &id_w, &id_h, &id_c)) == 6)
    {

    }
    if (read != -1) {
        free(paper);
        return (ft_perror("read fail : read pointer is not the end of file"));
    }
}