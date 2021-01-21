/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_example.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:09:44 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/23 16:29:24 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <chrono>

# define FULL_ASCII 0
# define ASCII_RANGE (FULL_ASCII) ? 1 : 32, (FULL_ASCII) ? 255 : 126
# define MODE "cC"

int
	rand_int_range(int min, int max, std::mt19937 &rng)
{
	return (std::uniform_int_distribution<int>(min, max)(rng));
}

float
	rand_float_range(float min, float max, std::mt19937 &rng)
{
    return (std::uniform_real_distribution<float>(min, max)(rng));
}

/*
** Returns a string containing between 0 and 4 spaces.
** (Selects a random space in a longer string, and returns a pointer to it.)
*/
const char* rand_space(std::mt19937 &rng)
{
	if (rand_int_range(0, 100, rng))
		return "";
	else
		return &"    "[rand_int_range(0, 4, rng)];
}
#define rsp	rand_space(rng)

int
	main(void)
{
	FILE*	file;
	int		width, height, background;
	int		i, nbr_shapes;
	int		type, color;
	float	x, y, sradius;
	int		size;
	short	status = 1;

	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	if (!(file = fopen("example_", "w")))
		return (printf("file: fopen error.\n"));
	width = rand_int_range(-1, 301, rng);
	height = rand_int_range(-1, 301, rng);
	background = rand_int_range(ASCII_RANGE, rng);
	if (fprintf(file, "%d %d %c\n", width, height, background) < 0)
		return (fclose(file) && printf("file: fprintf error.\n"));

	nbr_shapes = rand_int_range(-2, 42, rng);
	i = 0;
	while (i < nbr_shapes)
	{
		type = rand_int_range(0, 10000, rng);
		if (type == 10000 || type == 0)
			type = 'a';
		else if (type >= 5000)
			type = 'c';
		else
			type = 'C';
		color = rand_int_range(ASCII_RANGE, rng);
		if ((size = rand_int_range(0, 100, rng)) >= 25)
		{
			x = rand_float_range(.001, (float)size, rng);
			y = rand_float_range(.001, (float)size, rng);
			sradius = rand_float_range(.001, (float)size, rng);
		}
		else
		{
			x = rand_float_range(-100., 400., rng);
			y = rand_float_range(-100., 400., rng);
			sradius = rand_float_range(-.90, 400., rng);
		}
		if (!rand_int_range(0, 100, rng))
			status &= 0 < fprintf(file, "\n%s", rsp);
		if (rand_int_range(0, 100, rng) >= 35)
			status &= 0 < fprintf(file, (++i == nbr_shapes) ?
					"%s%c %s%f %s%f %s%f %s%c%s" : "%s%c %s%f %s%f %s%f %s%c%s\n",
					rsp, type, rsp, x, rsp, y, rsp, sradius, rsp, color, rsp);
		else
			status &= 0 < fprintf(file, (++i == nbr_shapes) ?
					"%s%c %s%d %s%d %s%d %s%c%s" : "%s%c %s%d %s%d %s%d %s%c%s\n",
					rsp, type, rsp, (int)x, rsp, (int)y, rsp, (int)sradius, rsp, color, rsp);
		if (!rand_int_range(0, 100, rng))
			status &= 0 < fprintf(file, "\n%s", rsp);
		if (!status)
			return (fclose(file) && printf("file: fprintf error.\n"));
	}
	fclose(file);
	return (0);
}
