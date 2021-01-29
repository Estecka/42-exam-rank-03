/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minicro_paint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 20:52:00 by abaur             #+#    #+#             */
/*   Updated: 2021/01/29 17:19:14 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 1

FILE* g_filestream;
char* g_rendertexture;
int   g_width, g_height;
#define pixel(x, y)	g_rendertexture[(y * g_width) + x]

/*
** Reprents an operation. A single circle to be drawn.
** @var char type	The type of operation to draw.
** 	C is filled, c is outline only.
** @var char color	The character to use to fill
** @var float x, y	The coordinates of the circle's center.
** @var float radius	The radius of the circle's outer border.
*/
typedef struct s_op	t_op;
#ifdef MINI
struct s_op
{
	char  type;
	char  color;
	float x, y;
	float radius;
};
#elif defined MICRO
#endif

/*
** Prints the rendertexture to a file descriptor.
** @param int fd	The file dscriptor.
*/
static void renderflush(int fd)
{
	for (int y=0; y<g_height; y++)
	{
		write(fd, &pixel(0, y), g_width); // Get the adress of the first pixel in a row, and prints the entire row.
		write(fd, "\n", 1);
	}
}

/*
** Parses a line of the as the header.
** The result is written into the appropriates global variables.
** @return bool
** 	true  OK
** 	false Error
*/
static short get_header()
{
	char background;
	int  status;

	status = fscanf(g_filestream, "%i %i %c\n",
		&g_width, &g_height, &background);
	#ifdef DEBUG
	dprintf(STDERR_FILENO, "[%i]HEADER: [%i, %i] %c\n",
		status, g_width, g_height, background);
	#endif
	// Die if any occurs:
	if (   (status < 3) // The header is incomplete.
		|| (background == ' ' || background == '\n' || background == (char)EOF) // The last property is not actually specified.
		|| (g_width  <= 0 || 300 < g_width ) // The texture's dimensions are invalid.
		|| (g_height <= 0 || 300 < g_height)
		)
	{
		return (0);
	}
	else // Allocate and initialize the rendertexture.
	{
		g_rendertexture = malloc(g_width*g_height);
		if (!g_rendertexture)
			return (0);
		memset(g_rendertexture, background, g_width*g_height);
		return (1);
	}
}

/*
** Renders an operation to the texture.
** @param t_op* op	The operation to render.
*/
#ifdef MINI
static void draw_op(t_op* op)
{
	for (int y=0; y<g_height; y++)
	for (int x=0; x<g_width;  x++)
	{
		float distance = sqrtf(powf(x - op->x, 2) + powf(y - op->y, 2));
		// Skip pixels inside an empty circle
		if (distance <= (op->radius-1) && op->type == 'c')
			continue;
		// Skip pixels outside any circle.
		if (distance >  (op->radius))
			continue;
		pixel(x, y) = op->color;
	}
}
#elif defined MICRO
#endif

/*
** Parses an operation from the file, and renders it immediately.
** @return int
** 	 1	An operation was drawn.
** 	 0	End of File was reached.
** 	-1	Error
*/
#ifdef MINI
static short get_next_op()
{
	t_op op;
	int  status;

	status = fscanf(g_filestream, "%c %f %f %f %c\n",
		&op.type, &op.x, &op.y, &op.radius, &op.color);
	#ifdef DEBUG
	dprintf(STDERR_FILENO, "[%i]OP: %c (%f, %f) (%f) %c\n",
		status, op.type, op.x, op.y, op.radius, op.color);
	#endif
	if (status < 0) // Check for End of File
		return (0);
	// Die if any occurs: 
	if (   (status < 5) // The op is incomplete
		|| (op.type != 'c' && op.type != 'C') // The type is unknown
		|| (op.color == ' ' || op.color == '\n' || op.color == (char)EOF) // The last property is not actually specified.
		|| (op.radius <= 0) // The circle's radius is invalid.
		)
	{
		return (-1);
	}
	else
	{
		draw_op(&op);
		return (1);
	}
}
#elif defined MICRO
#endif

/*
** Frees all data and returns the given status.
** This does not actually exit the program.
** @param int status	The status to be returned.
** @return int	Same value as `status`.
*/
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
	// Iterates over each operation line, parses and renders it immediately.
	while(0 < (status = get_next_op()))
		continue;
	if (status < 0) // Triggers if an invalid operation is encountered.
		return (clean_exit(EXIT_FAILURE));

	renderflush(STDOUT_FILENO);
	return(clean_exit(EXIT_SUCCESS));
}
