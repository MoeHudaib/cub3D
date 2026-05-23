/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:45:54 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:45:55 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_key_press(int keycode, t_cub *cub)
{
	if (keycode == K_W)
		cub->keys.w = 1;
	if (keycode == K_S)
		cub->keys.s = 1;
	if (keycode == K_A)
		cub->keys.a = 1;
	if (keycode == K_D)
		cub->keys.d = 1;
	if (keycode == K_LEFT)
		cub->keys.left = 1;
	if (keycode == K_RIGHT)
		cub->keys.right = 1;
	if (keycode == K_ESC)
		handle_close(cub);
	return (0);
}

int	handle_key_release(int keycode, t_cub *cub)
{
	if (keycode == K_W)
		cub->keys.w = 0;
	if (keycode == K_S)
		cub->keys.s = 0;
	if (keycode == K_A)
		cub->keys.a = 0;
	if (keycode == K_D)
		cub->keys.d = 0;
	if (keycode == K_LEFT)
		cub->keys.left = 0;
	if (keycode == K_RIGHT)
		cub->keys.right = 0;
	return (0);
}

int	handle_close(t_cub *cub)
{
	destroy_cub(cub);
	exit(0);
	return (0);
}
