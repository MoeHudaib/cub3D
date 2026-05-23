/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:48:12 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:48:13 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Zero-out the MLX context and every texture slot before mlx_init() is called.
** (The t_game struct itself is already zero'd in main, but this makes the
** intent explicit and guards against future partial initialisations.)
*/
static void	reset_mlx_context(t_game *game)
{
	int	i;

	game->mlx.handle = NULL;
	game->mlx.window = NULL;
	game->mlx.framebuffer.mlx_img = NULL;
	game->mlx.framebuffer.pixel_data = NULL;
	game->mlx.framebuffer.width = 0;
	game->mlx.framebuffer.height = 0;
	i = 0;
	while (i < 4)
	{
		game->textures[i].mlx_img = NULL;
		game->textures[i].pixel_data = NULL;
		game->textures[i].width = 0;
		game->textures[i].height = 0;
		i++;
	}
}

/*
** Initialise the MLX connection, open the window, and allocate the
** framebuffer image that will be redrawn each frame.
*/
void	init_mlx(t_game *game)
{
	reset_mlx_context(game);
	game->mlx.handle = mlx_init();
	if (!game->mlx.handle)
		game_fatal(game, "Failed to initialise MLX");
	game->mlx.window = mlx_new_window(game->mlx.handle,
			WIN_WIDTH, WIN_HEIGHT, "cub3D");
	if (!game->mlx.window)
		game_fatal(game, "Failed to create window");
	game->mlx.framebuffer.mlx_img = mlx_new_image(game->mlx.handle,
			WIN_WIDTH, WIN_HEIGHT);
	if (!game->mlx.framebuffer.mlx_img)
		game_fatal(game, "Failed to create framebuffer image");
	game->mlx.framebuffer.pixel_data = mlx_get_data_addr(
			game->mlx.framebuffer.mlx_img,
			&game->mlx.framebuffer.bits_per_pixel,
			&game->mlx.framebuffer.line_length,
			&game->mlx.framebuffer.endian);
	if (!game->mlx.framebuffer.pixel_data)
		game_fatal(game, "Failed to access framebuffer data");
	game->mlx.framebuffer.width = WIN_WIDTH;
	game->mlx.framebuffer.height = WIN_HEIGHT;
}

/*
** Load one XPM texture from disk into the given texture slot.
*/
static void	load_single_texture(t_game *game, int index)
{
	t_image	*tex;

	tex = &game->textures[index];
	tex->mlx_img = mlx_xpm_file_to_image(game->mlx.handle,
			game->texture_paths[index],
			&tex->width, &tex->height);
	if (!tex->mlx_img)
		game_fatal(game, "Failed to load texture file");
	tex->pixel_data = mlx_get_data_addr(tex->mlx_img,
			&tex->bits_per_pixel,
			&tex->line_length,
			&tex->endian);
	if (!tex->pixel_data)
		game_fatal(game, "Failed to read texture pixel data");
}

/*
** Load all four directional textures (N, S, W, E).
*/
void	load_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		load_single_texture(game, i);
		i++;
	}
}
