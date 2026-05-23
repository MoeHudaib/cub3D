/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:06 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

int	on_window_close(t_game *game)
{
	destroy_game(game);
	exit(0);
}

int	on_key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESCAPE)
		on_window_close(game);
	if (keycode == KEY_W)
		game->keys.move_forward = 1;
	if (keycode == KEY_S)
		game->keys.move_backward = 1;
	if (keycode == KEY_A)
		game->keys.strafe_left = 1;
	if (keycode == KEY_D)
		game->keys.strafe_right = 1;
	if (keycode == KEY_ARROW_LEFT)
		game->keys.rotate_left = 1;
	if (keycode == KEY_ARROW_RIGHT)
		game->keys.rotate_right = 1;
	return (0);
}

int	on_key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->keys.move_forward = 0;
	if (keycode == KEY_S)
		game->keys.move_backward = 0;
	if (keycode == KEY_A)
		game->keys.strafe_left = 0;
	if (keycode == KEY_D)
		game->keys.strafe_right = 0;
	if (keycode == KEY_ARROW_LEFT)
		game->keys.rotate_left = 0;
	if (keycode == KEY_ARROW_RIGHT)
		game->keys.rotate_right = 0;
	return (0);
}
