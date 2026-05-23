/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:41 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:42 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Returns 1 if the tile at (world_x, world_y) blocks movement
** (out-of-bounds, wall '1', or void ' ').
*/
static int	tile_is_solid(t_game *game, double world_x, double world_y)
{
	int	col;
	int	row;

	col = (int)world_x;
	row = (int)world_y;
	if (col < 0 || row < 0 || col >= game->map.width || row >= game->map.height)
		return (1);
	if (game->map.grid[row][col] == '1')
		return (1);
	if (game->map.grid[row][col] == ' ')
		return (1);
	return (0);
}

/*
** Attempt to slide the player by (delta_x, delta_y).
** Each axis is tested independently so the player can slide along walls.
*/
static void	slide_player(t_game *game, double delta_x, double delta_y)
{
	double	next_x;
	double	next_y;

	next_x = game->player.pos_x + delta_x;
	next_y = game->player.pos_y + delta_y;
	if (!tile_is_solid(game, next_x, game->player.pos_y))
		game->player.pos_x = next_x;
	if (!tile_is_solid(game, game->player.pos_x, next_y))
		game->player.pos_y = next_y;
}

/*
** Apply movement for all pressed directional keys this frame.
** Forward/backward move along dir, strafing moves along the camera plane.
*/
void	update_player_movement(t_game *game)
{
	if (game->keys.move_forward)
		slide_player(game,
			game->player.dir_x * MOVE_SPEED,
			game->player.dir_y * MOVE_SPEED);
	if (game->keys.move_backward)
		slide_player(game,
			-game->player.dir_x * MOVE_SPEED,
			-game->player.dir_y * MOVE_SPEED);
	if (game->keys.strafe_left)
		slide_player(game,
			-game->player.plane_x * MOVE_SPEED,
			-game->player.plane_y * MOVE_SPEED);
	if (game->keys.strafe_right)
		slide_player(game,
			game->player.plane_x * MOVE_SPEED,
			game->player.plane_y * MOVE_SPEED);
}

/*
** Rotate both the view direction and camera plane vectors by `angle` radians.
** Positive angle rotates clockwise (right), negative counter-clockwise (left).
*/
void	rotate_player(t_game *game, double angle)
{
	double	prev_dir_x;
	double	prev_plane_x;
	double	cos_a;
	double	sin_a;

	prev_dir_x = game->player.dir_x;
	prev_plane_x = game->player.plane_x;
	cos_a = cos(angle);
	sin_a = sin(angle);
	game->player.dir_x = game->player.dir_x * cos_a
		- game->player.dir_y * sin_a;
	game->player.dir_y = prev_dir_x * sin_a
		+ game->player.dir_y * cos_a;
	game->player.plane_x = game->player.plane_x * cos_a
		- game->player.plane_y * sin_a;
	game->player.plane_y = prev_plane_x * sin_a
		+ game->player.plane_y * cos_a;
}
