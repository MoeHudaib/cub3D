/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:54 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:35:12 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Compute ray direction for screen column `screen_x` using the camera plane.
** camera_x maps [0, WIN_WIDTH] to [-1, +1] (left edge to right edge).
*/
static void	init_ray_direction(t_game *game, int screen_x, t_ray *ray)
{
	double	camera_x;

	camera_x = 2.0 * screen_x / (double)WIN_WIDTH - 1.0;
	ray->dir_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray->dir_y = game->player.dir_y + game->player.plane_y * camera_x;
	ray->map_x = (int)game->player.pos_x;
	ray->map_y = (int)game->player.pos_y;
}

/*
** Pre-compute the per-axis delta distances and the initial side distances,
** then set step directions (+1 or -1) for the DDA traversal.
** delta_dist is the ray length to cross one full grid cell on each axis.
*/
static void	prepare_dda(t_game *game, t_ray *ray)
{
	ray->delta_dist_x = (ray->dir_x == 0) ? 1e30 : fabs(1.0 / ray->dir_x);
	ray->delta_dist_y = (ray->dir_y == 0) ? 1e30 : fabs(1.0 / ray->dir_y);
	ray->step_x = 1;
	ray->side_dist_x = (ray->map_x + 1.0 - game->player.pos_x)
		* ray->delta_dist_x;
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.pos_x - ray->map_x)
			* ray->delta_dist_x;
	}
	ray->step_y = 1;
	ray->side_dist_y = (ray->map_y + 1.0 - game->player.pos_y)
		* ray->delta_dist_y;
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.pos_y - ray->map_y)
			* ray->delta_dist_y;
	}
}

/*
** Advance through the grid one cell at a time (DDA) until we hit a wall.
** ray->side is 0 for an X-axis wall hit, 1 for a Y-axis wall hit.
** Maximum iterations prevent infinite loops when delta_dist is very large.
*/
static void	run_dda(t_game *game, t_ray *ray)
{
	int	hit_wall;
	int	iterations;

	hit_wall = 0;
	iterations = 0;
	while (!hit_wall && iterations < 10000)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->map.width
			|| ray->map_y < 0 || ray->map_y >= game->map.height)
		{
			game->map.grid[ray->map_y][ray->map_x] = '1';
			hit_wall = 1;
		}
		if (game->map.grid[ray->map_y][ray->map_x] != '0')
			hit_wall = 1;
		iterations++;
	}
}

/*
** Compute the perpendicular wall distance and the exact X position on the
** wall face (wall_hit_x in [0, 1]) for texture mapping.
** A minimum distance of 0.1 prevents division-by-zero in the caller.
*/
static void	compute_wall_hit(t_game *game, t_ray *ray)
{
	double	wall_offset;

	if (ray->side == 0)
	{
		wall_offset = (1 - ray->step_x) / 2.0;
		ray->perp_wall_dist = (ray->map_x - game->player.pos_x + wall_offset)
			/ ray->dir_x;
		ray->wall_hit_x = game->player.pos_y
			+ ray->perp_wall_dist * ray->dir_y;
	}
	else
	{
		wall_offset = (1 - ray->step_y) / 2.0;
		ray->perp_wall_dist = (ray->map_y - game->player.pos_y + wall_offset)
			/ ray->dir_y;
		ray->wall_hit_x = game->player.pos_x
			+ ray->perp_wall_dist * ray->dir_x;
	}
	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = 0.1;
	ray->wall_hit_x -= floor(ray->wall_hit_x);
}

/*
** Cast a ray for screen column `screen_x`, populate `ray`, and return the
** projected wall height in pixels.
*/
int	cast_ray(t_game *game, int screen_x, t_ray *ray)
{
	init_ray_direction(game, screen_x, ray);
	prepare_dda(game, ray);
	run_dda(game, ray);
	compute_wall_hit(game, ray);
	return ((int)(WIN_HEIGHT / ray->perp_wall_dist));
}
