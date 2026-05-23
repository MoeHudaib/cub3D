/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:41 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:42 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_blocked(t_cub *cub, double x, double y)
{
	int	xi;
	int	yi;

	xi = (int)x;
	yi = (int)y;
	if (xi < 0 || yi < 0 || xi >= cub->map.width || yi >= cub->map.height)
		return (1);
	if (cub->map.grid[yi][xi] == '1')
		return (1);
	if (cub->map.grid[yi][xi] == ' ')
		return (1);
	return (0);
}

static void	move_step(t_cub *cub, double dx, double dy)
{
	double	new_x;
	double	new_y;

	new_x = cub->player.x + dx;
	new_y = cub->player.y + dy;
	if (!is_blocked(cub, new_x, cub->player.y))
		cub->player.x = new_x;
	if (!is_blocked(cub, cub->player.x, new_y))
		cub->player.y = new_y;
}

void	move_player(t_cub *cub)
{
	if (cub->keys.w)
		move_step(cub, cub->player.dir_x * MOVE_SPEED, cub->player.dir_y
			* MOVE_SPEED);
	if (cub->keys.s)
		move_step(cub, -cub->player.dir_x * MOVE_SPEED, -cub->player.dir_y
			* MOVE_SPEED);
	if (cub->keys.a)
		move_step(cub, -cub->player.plane_x * MOVE_SPEED, -cub->player.plane_y
			* MOVE_SPEED);
	if (cub->keys.d)
		move_step(cub, cub->player.plane_x * MOVE_SPEED, cub->player.plane_y
			* MOVE_SPEED);
}
