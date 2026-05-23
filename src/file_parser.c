/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:48 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:49 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	init_parser(t_config_parser *parser)
{
	int	i;

	i = 0;
	while (i < 4)
		parser->texture_paths[i++] = NULL;
	parser->colors[0] = -1;
	parser->colors[1] = -1;
	parser->map_rows = NULL;
	parser->row_count = 0;
	parser->phase = PHASE_HEADERS;
	parser->current_line = NULL;
	parser->fd = -1;
}

/*
** Verify that all required identifiers were present in the file.
*/
static void	check_required_fields(t_config_parser *parser)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!parser->texture_paths[i])
			parser_fatal(parser, "Missing texture identifier");
		i++;
	}
	if (parser->colors[0] == -1 || parser->colors[1] == -1)
		parser_fatal(parser, "Missing color identifier");
	if (parser->row_count == 0)
		parser_fatal(parser, "Missing map data");
}

/*
** Read the file line by line, feeding each line to the state machine.
** gnl_cleanup drains any remaining get_next_line internal buffer.
*/
static void	read_file_lines(t_config_parser *parser, int fd)
{
	char	*line;

	parser->fd = fd;
	line = get_next_line(fd);
	while (line)
	{
		parser->current_line = line;
		process_map_line(parser, line);
		free(line);
		parser->current_line = NULL;
		line = get_next_line(fd);
	}
	gnl_cleanup(fd);
	parser->fd = -1;
}

/*
** Open the .cub file, run the parser, validate completeness, then build
** the live game structures from the collected data.
*/
int	load_map_file(t_game *game, const char *path)
{
	t_config_parser	parser;
	int				fd;

	init_parser(&parser);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		parser_fatal(&parser, "Cannot open map file");
	read_file_lines(&parser, fd);
	close(fd);
	check_required_fields(&parser);
	validate_map(&parser);
	build_game_map(game, &parser);
	return (1);
}
