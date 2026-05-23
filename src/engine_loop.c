/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:06 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Called by MLX every frame: update movement, handle rotation, then draw.
*/
int	on_frame_tick(t_game *game)
{
	update_player_movement(game);
	if (game->keys.rotate_left)
		rotate_player(game, -ROTATE_SPEED);
	if (game->keys.rotate_right)
		rotate_player(game, ROTATE_SPEED);
	render_frame(game);
	return (0);
}

/*
** Register all MLX hooks and enter the event loop.
** An initial frame is drawn before handing control to MLX.
*/
void	start_game_loop(t_game *game)
{
	mlx_hook(game->mlx.window, EVENT_WINDOW_CLOSE, 0,
		on_window_close, game);
	mlx_hook(game->mlx.window, EVENT_KEY_PRESS, EVENT_MASK_KEY_PRESS,
		on_key_press, game);
	mlx_hook(game->mlx.window, EVENT_KEY_RELEASE, EVENT_MASK_KEY_RELEASE,
		on_key_release, game);
	mlx_loop_hook(game->mlx.handle, on_frame_tick, game);
	render_frame(game);
	mlx_loop(game->mlx.handle);
}
