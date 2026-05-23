/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:06 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	loop_hook(t_cub *cub)
{
	move_player(cub);
	if (cub->keys.left)
		rotate_player(cub, -ROT_SPEED);
	if (cub->keys.right)
		rotate_player(cub, ROT_SPEED);
	render(cub);
	return (0);
}

void	start_loop(t_cub *cub)
{
	mlx_hook(cub->mlx.win, 17, 0, handle_close, cub);
	mlx_hook(cub->mlx.win, 2, 1L << 0, handle_key_press, cub);
	mlx_hook(cub->mlx.win, 3, 1L << 1, handle_key_release, cub);
	mlx_loop_hook(cub->mlx.ptr, loop_hook, cub);
	render(cub);
	mlx_loop(cub->mlx.ptr);
}
