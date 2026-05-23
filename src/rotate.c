/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:48:05 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:48:05 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_player(t_cub *cub, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	double	cos_val;
	double	sin_val;

	old_dir_x = cub->player.dir_x;
	old_plane_x = cub->player.plane_x;
	cos_val = cos(angle);
	sin_val = sin(angle);
	cub->player.dir_x = cub->player.dir_x * cos_val - cub->player.dir_y
		* sin_val;
	cub->player.dir_y = old_dir_x * sin_val + cub->player.dir_y * cos_val;
	cub->player.plane_x = cub->player.plane_x * cos_val - cub->player.plane_y
		* sin_val;
	cub->player.plane_y = old_plane_x * sin_val + cub->player.plane_y * cos_val;
}
