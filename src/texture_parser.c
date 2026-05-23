/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:00 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:00 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Store the texture file path for direction `index` (TEX_NORTH..TEX_EAST).
** Leading whitespace in `value` is skipped.  Duplicate identifiers abort.
*/
void	set_texture_path(t_config_parser *parser, int index, const char *value)
{
	const char	*path_start;

	if (parser->texture_paths[index])
		parser_fatal(parser, "Duplicate texture identifier");
	path_start = value;
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (!*path_start)
		parser_fatal(parser, "Empty texture path");
	parser->texture_paths[index] = ft_strdup(path_start);
	if (!parser->texture_paths[index])
		parser_fatal(parser, "Memory allocation failed");
}
