/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:48:18 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:48:19 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	map_char(t_parse *parse, int y, int x)
{
	int	len;

	if (y < 0 || y >= parse->map_size)
		return (' ');
	len = ft_strlen(parse->map_lines[y]);
	if (x < 0 || x >= len)
		return (' ');
	return (parse->map_lines[y][x]);
}

static int	is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static void	check_neighbors(t_parse *parse, int y, int x)
{
	if (map_char(parse, y - 1, x) == ' ')
		fatal_parse(parse, "Map is not closed");
	if (map_char(parse, y + 1, x) == ' ')
		fatal_parse(parse, "Map is not closed");
	if (map_char(parse, y, x - 1) == ' ')
		fatal_parse(parse, "Map is not closed");
	if (map_char(parse, y, x + 1) == ' ')
		fatal_parse(parse, "Map is not closed");
}

static void	validate_cell(t_parse *parse, int y, int x, int *players)
{
	char	c;

	c = parse->map_lines[y][x];
	if (!ft_strchr("01NSEW ", c))
		fatal_parse(parse, "Invalid map character");
	if (ft_strchr("NSEW", c))
		(*players)++;
	if (is_walkable(c))
	{
		if (y == 0 || x == 0)
			fatal_parse(parse, "Map is not closed");
		if (y == parse->map_size - 1)
			fatal_parse(parse, "Map is not closed");
		if ((int)ft_strlen(parse->map_lines[y]) - 1 == x)
			fatal_parse(parse, "Map is not closed");
		check_neighbors(parse, y, x);
	}
}

void	validate_map(t_parse *parse)
{
	int	y;
	int	x;
	int	players;

	players = 0;
	y = 0;
	while (y < parse->map_size)
	{
		x = 0;
		while (parse->map_lines[y][x])
		{
			validate_cell(parse, y, x, &players);
			x++;
		}
		y++;
	}
	if (players != 1)
		fatal_parse(parse, "Invalid player count");
}
