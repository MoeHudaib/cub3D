/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:06 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:18:22 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Print "Error\n<msg>\n" to stderr, free any partially parsed data,
** and exit with status 1.
*/
void	parser_fatal(t_config_parser *parser, const char *msg)
{
	int	i;

	ft_putstr_fd("Error\n", 2);
	ft_putendl_fd(msg, 2);
	if (parser)
	{
		safe_free((void **)&parser->current_line);
		if (parser->fd >= 0)
		{
			gnl_cleanup(parser->fd);
			close(parser->fd);
		}
		i = 0;
		while (i < 4)
			safe_free((void **)&parser->texture_paths[i++]);
		i = 0;
		while (i < parser->row_count && parser->map_rows)
			safe_free((void **)&parser->map_rows[i++]);
		safe_free((void **)&parser->map_rows);
	}
	exit(1);
}

/*
** Print "Error\n<msg>\n" to stderr, destroy the game, and exit.
*/
void	game_fatal(t_game *game, const char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putendl_fd(msg, 2);
	if (game)
		destroy_game(game);
	exit(1);
}

/*
** Free a NULL-terminated array of strings and the array itself.
*/
void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		safe_free((void **)&arr[i++]);
	free(arr);
}
