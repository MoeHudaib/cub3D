/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:00 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:00 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Return the character at (row, col), or ' ' when out of range.
** This lets boundary checks treat the area outside the map as void.
*/
static char	map_char_at(t_config_parser *parser, int row, int col)
{
	if (row < 0 || row >= parser->row_count)
		return (' ');
	if (col < 0 || col >= (int)ft_strlen(parser->map_rows[row]))
		return (' ');
	return (parser->map_rows[row][col]);
}

/*
** A floor tile ('0') or spawn tile must be completely surrounded by wall
** or other floor tiles — never by void (' ') or out-of-bounds.
** Returns 1 if the tile is invalid (exposed to void).
*/
static int	floor_tile_is_exposed(t_config_parser *parser, int row, int col)
{
	static const int	dr[4] = {-1, 1, 0, 0};
	static const int	dc[4] = {0, 0, -1, 1};
	char				neighbour;
	int				dir;

	dir = 0;
	while (dir < 4)
	{
		neighbour = map_char_at(parser, row + dr[dir], col + dc[dir]);
		if (neighbour == ' ')
			return (1);
		dir++;
	}
	return (0);
}

/*
** Check every cell: valid characters only, exactly one spawn, and no
** floor tile exposed to void.
*/
void	validate_map(t_config_parser *parser)
{
	static const char	valid_chars[] = "01 NSWE";
	int					spawn_count;
	int					row;
	int					col;
	char				cell;

	spawn_count = 0;
	row = 0;
	while (row < parser->row_count)
	{
		col = 0;
		while (parser->map_rows[row][col])
		{
			cell = parser->map_rows[row][col];
			if (!ft_strchr(valid_chars, cell))
				parser_fatal(parser, "Invalid character in map");
			if (cell == 'N' || cell == 'S' || cell == 'W' || cell == 'E')
				spawn_count++;
			if ((cell == '0' || cell == 'N' || cell == 'S'
					|| cell == 'W' || cell == 'E')
				&& floor_tile_is_exposed(parser, row, col))
				parser_fatal(parser, "Map is not enclosed by walls");
			col++;
		}
		row++;
	}
	if (spawn_count != 1)
		parser_fatal(parser, "Map must have exactly one player spawn");
}
