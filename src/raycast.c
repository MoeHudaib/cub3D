/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:54 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:54 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_ray(t_cub *cub, int x, t_ray *ray)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->dir_x = cub->player.dir_x + cub->player.plane_x * camera_x;
	ray->dir_y = cub->player.dir_y + cub->player.plane_y * camera_x;
	ray->map_x = (int)cub->player.x;
	ray->map_y = (int)cub->player.y;
}

static void	prepare_ray(t_cub *cub, t_ray *ray)
{
	if (ray->dir_x == 0)
		ray->delta_x = 1e30;
	else
		ray->delta_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y == 0)
		ray->delta_y = 1e30;
	else
		ray->delta_y = fabs(1.0 / ray->dir_y);
	ray->step_x = 1;
	ray->side_dist_x = (ray->map_x + 1.0 - cub->player.x) * ray->delta_x;
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (cub->player.x - ray->map_x) * ray->delta_x;
	}
	ray->step_y = 1;
	ray->side_dist_y = (ray->map_y + 1.0 - cub->player.y) * ray->delta_y;
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (cub->player.y - ray->map_y) * ray->delta_y;
	}
}

static void	perform_dda(t_cub *cub, t_ray *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= cub->map.width)
			fatal_cub(cub, "Ray escaped map bounds");
		if (ray->map_y < 0 || ray->map_y >= cub->map.height)
			fatal_cub(cub, "Ray escaped map bounds");
		if (cub->map.grid[ray->map_y][ray->map_x] != '0')
			hit = 1;
	}
}

static void	compute_distance(t_cub *cub, t_ray *ray)
{
	double	offset;

	if (ray->side == 0)
	{
		offset = (1 - ray->step_x) / 2.0;
		ray->perp_dist = (ray->map_x - cub->player.x + offset) / ray->dir_x;
		ray->wall_x = cub->player.y + ray->perp_dist * ray->dir_y;
	}
	else
	{
		offset = (1 - ray->step_y) / 2.0;
		ray->perp_dist = (ray->map_y - cub->player.y + offset) / ray->dir_y;
		ray->wall_x = cub->player.x + ray->perp_dist * ray->dir_x;
	}
	if (ray->perp_dist <= 0)
		ray->perp_dist = 0.1;
	ray->wall_x -= floor(ray->wall_x);
}

int	perform_raycast(t_cub *cub, int x, t_ray *ray)
{
	int	height;

	init_ray(cub, x, ray);
	prepare_ray(cub, ray);
	perform_dda(cub, ray);
	compute_distance(cub, ray);
	height = (int)(WIN_H / ray->perp_dist);
	return (height);
}
