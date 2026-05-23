/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:59 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:59 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Cast a ray for every screen column, draw the resulting wall slice,
** then blit the completed framebuffer to the window.
*/
void	render_frame(t_game *game)
{
	t_ray	ray;
	int		wall_height;
	int		screen_x;

	screen_x = 0;
	while (screen_x < WIN_WIDTH)
	{
		wall_height = cast_ray(game, screen_x, &ray);
		draw_wall_column(game, screen_x, &ray, wall_height);
		screen_x++;
	}
	mlx_put_image_to_window(game->mlx.handle, game->mlx.window,
		game->mlx.framebuffer.mlx_img, 0, 0);
}
