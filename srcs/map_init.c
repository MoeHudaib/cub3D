/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:00 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:27:26 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Direction vectors and camera planes for each spawn orientation.
** The plane length (0.66) gives roughly a 66° horizontal FOV.
*/
static void	set_player_orientation(t_player *player, char facing)
{
	if (facing == 'N')
	{
		player->dir_x = 0;   player->dir_y = -1;
		player->plane_x = 0.66; player->plane_y = 0;
	}
	else if (facing == 'S')
	{
		player->dir_x = 0;   player->dir_y = 1;
		player->plane_x = -0.66; player->plane_y = 0;
	}
	else if (facing == 'W')
	{
		player->dir_x = -1;  player->dir_y = 0;
		player->plane_x = 0; player->plane_y = -0.66;
	}
	else if (facing == 'E')
	{
		player->dir_x = 1;   player->dir_y = 0;
		player->plane_x = 0; player->plane_y = 0.66;
	}
}

static char	*pad_map_row(const char *row, int width)
{
	char	*buffer;
	int		len;

	len = ft_strlen(row);
	buffer = (char *)ft_calloc(width + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	ft_memcpy(buffer, row, len);
	while (len < width)
		buffer[len++] = ' ';
	return (buffer);
}

/*
** Scan the map grid for the player spawn tile (N/S/W/E), set the position
** to the centre of that cell, configure the orientation, then replace the
** spawn tile with a floor tile ('0').
*/
void	init_player(t_game *game)
{
	static const char	spawn_chars[] = "NSWE";
	int					row;
	int					col;
	int					i;
	int					row_width;

	row = 0;
	while (row < game->map.height)
	{
		col = 0;
		row_width = ft_strlen(game->map.grid[row]);
		while (col < row_width)
		{
			i = 0;
			while (spawn_chars[i])
			{
				if (game->map.grid[row][col] == spawn_chars[i])
				{
					game->player.pos_x = col + 0.5;
					game->player.pos_y = row + 0.5;
					set_player_orientation(&game->player, spawn_chars[i]);
					game->map.grid[row][col] = '0';
					return ;
				}
				i++;
			}
			col++;
		}
		row++;
	}
}

/*
** Transfer the parsed map rows and colours from the temporary parser into
** the live t_game struct, then free the parser's path strings.
*/
void	build_game_map(t_game *game, t_config_parser *parser)
{
	int		max_width;
	int		row;
	char	*padding;

	game->map.height = parser->row_count;
	max_width = 0;
	row = 0;
	while (row < parser->row_count)
	{
		int w = (int)ft_strlen(parser->map_rows[row]);
		if (w > max_width)
			max_width = w;
		row++;
	}
	game->map.width = max_width;
	game->map.grid = (char **)ft_calloc(parser->row_count, sizeof(char *));
	if (!game->map.grid)
		game_fatal(game, "Memory allocation failed");
	row = 0;
	while (row < parser->row_count)
	{
		padding = pad_map_row(parser->map_rows[row], max_width);
		if (!padding)
			game_fatal(game, "Memory allocation failed");
		game->map.grid[row] = padding;
		safe_free((void **)&parser->map_rows[row]);
		row++;
	}
	safe_free((void **)&parser->map_rows);
	game->floor_color = parser->colors[0];
	game->ceiling_color = parser->colors[1];
	row = 0;
	while (row < 4)
	{
		game->texture_paths[row] = parser->texture_paths[row];
		parser->texture_paths[row] = NULL;
		row++;
	}
}
