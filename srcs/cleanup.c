/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:06 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	free_map_grid(t_world_map *map)
{
	int	row;

	if (!map->grid)
		return ;
	row = 0;
	while (row < map->height)
	{
		safe_free((void **)&map->grid[row]);
		row++;
	}
	safe_free((void **)&map->grid);
}

static void	free_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->textures[i].mlx_img && game->mlx.handle)
			mlx_destroy_image(game->mlx.handle, game->textures[i].mlx_img);
		game->textures[i].mlx_img = NULL;
		safe_free((void **)&game->texture_paths[i]);
		i++;
	}
}

static void	free_mlx_resources(t_game *game)
{
	if (game->mlx.framebuffer.mlx_img && game->mlx.handle)
		mlx_destroy_image(game->mlx.handle, game->mlx.framebuffer.mlx_img);
	if (game->mlx.window && game->mlx.handle)
		mlx_destroy_window(game->mlx.handle, game->mlx.window);
	game->mlx.framebuffer.mlx_img = NULL;
	game->mlx.window = NULL;
}

/*
** Release every resource owned by t_game in a safe, ordered manner.
*/
void	destroy_game(t_game *game)
{
	if (!game)
		return ;
	free_textures(game);
	free_mlx_resources(game);
	free_map_grid(&game->map);
}
