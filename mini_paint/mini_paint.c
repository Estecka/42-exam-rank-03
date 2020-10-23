/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 20:52:00 by abaur             #+#    #+#             */
/*   Updated: 2020/10/23 22:20:52 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 1

FILE*	g_filestream;
char*	g_rendertexture;
int	g_width, g_height;
#define pixel(x, y)	g_rendertexture[(y * g_width) + x]

typedef struct s_op	t_op;
struct s_op
{
	char	type;
	char	color;
	float	x, y;
	float	radius;
};

static void renderflush(int fd)
{
	for (int y=0; y<g_height; y++)
	{
		write(fd, &pixel(0, y), g_width);
		write(fd, "\n", 1);
	}
}

static short get_header()
{
	char background;
	char lineterm;
	int  status;

	status = fscanf(g_filestream, "%i %i %c%c", &g_width, &g_height, &background, &lineterm);
	#ifdef DEBUG
	dprintf(STDERR_FILENO, "[%i]HEADER: [%i, %i] %c 0x%02x\n", status, g_width, g_height, background, lineterm);
	#endif
	if (   (status < 3)
		|| (status == 4 && lineterm != '\n')
		|| (background == ' ' || background == '\n' || background == EOF)
		|| (g_width  <= 0 || 300 < g_width )
		|| (g_height <= 0 || 300 < g_height)
		)
	{
		return (0);
	}
	else
	{
		g_rendertexture = malloc(g_width*g_height);
		if (!g_rendertexture)
			return (0);
		memset(g_rendertexture, background, g_width*g_height);
		return (1);
	}
}

static void draw_op(t_op* op)
{
	for (int x=0; x<g_width;  x++)
	for (int y=0; y<g_height; y++)
	{
		float distance = sqrtf(powf(x - op->x, 2) + powf(y - op->y, 2));
		if (distance <= (op->radius-1) && op->type == 'c')
			continue;
		if (distance >  (op->radius))
			continue;
		pixel(x, y) = op->color;
	}
}

static short get_next_op()
{
	t_op op;
	char lineterm;
	int  status;

	status = fscanf(g_filestream, "%c %f %f %f %c%c", &op.type, &op.x, &op.y, &op.radius, &op.color, &lineterm);
	#ifdef DEBUG
	dprintf(STDERR_FILENO, "[%i]OP: %c (%f, %f) (%f) %c 0x%02x\n", status, op.type, op.x, op.y, op.radius, op.color, lineterm);
	#endif
	if (   (status < 5)
		|| (status == 6 && lineterm != '\n')
		|| (op.type != 'c' && op.type != 'C')
		|| (op.color == ' ' || op.color == '\n' || op.color == EOF)
		|| (op.radius <= 0)
		)
	{
		return (-1);
	}
	else
	{
		draw_op(&op);
		return (status == 6);	
	}
}

static int clean_exit(int status)
{
	#ifdef DEBUG
	if (status && g_rendertexture)
		renderflush(STDERR_FILENO);
	#endif

	if (g_filestream)
		fclose(g_filestream);
	if (g_rendertexture)
		free(g_rendertexture);

	if (status)
		write(STDOUT_FILENO, "Error: Operation file corrupted\n", 32);
	return (status);
}

extern int main(int argc, char** argv)
{
	if (argc != 2)
	{
		write(STDOUT_FILENO, "Error: argument\n", 16);
		return (EXIT_FAILURE);
	}

	g_filestream = fopen(argv[1], "r");
	if (!g_filestream)
		return(clean_exit(EXIT_FAILURE));
	
	if (!get_header())
		return(clean_exit(EXIT_FAILURE));

	short status;
	while(0 < (status = get_next_op()))
		continue;
	if (status < 0)
		return (clean_exit(EXIT_FAILURE));

	renderflush(STDOUT_FILENO);
	return(clean_exit(EXIT_SUCCESS));
}