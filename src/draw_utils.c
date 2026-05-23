/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:40:14 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:45:44 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	select_texture(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir_x > 0)
			return (TEX_WE);
		return (TEX_EA);
	}
	if (ray->dir_y > 0)
		return (TEX_NO);
	return (TEX_SO);
}

int	select_tex_x(t_ray *ray, t_img *tex)
{
	int	tex_x;

	tex_x = (int)(ray->wall_x * tex->width);
	if (tex_x < 0)
		tex_x = 0;
	if (ray->side == 0 && ray->dir_x > 0)
		tex_x = tex->width - tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		tex_x = tex->width - tex_x - 1;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	return (tex_x);
}
