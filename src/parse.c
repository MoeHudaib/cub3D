/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:48 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:49 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	parse_init(t_parse *parse)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		parse->tex[i] = NULL;
		i++;
	}
	parse->colors[0] = -1;
	parse->colors[1] = -1;
	parse->map_lines = NULL;
	parse->map_size = 0;
	parse->phase = PH_HEADERS;
	parse->current_line = NULL;
	parse->fd = -1;
}

static void	ensure_config(t_parse *parse)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!parse->tex[i])
			fatal_parse(parse, "Missing texture identifier");
		i++;
	}
	if (parse->colors[0] == -1 || parse->colors[1] == -1)
		fatal_parse(parse, "Missing color identifier");
	if (parse->map_size == 0)
		fatal_parse(parse, "Missing map data");
}

static void	parse_stream(t_parse *parse, int fd)
{
	char	*line;

	parse->fd = fd;
	line = get_next_line(fd);
	while (line)
	{
		parse->current_line = line;
		parse_line_data(parse, line);
		free(line);
		parse->current_line = NULL;
		line = get_next_line(fd);
	}
	gnl_cleanup(fd);
	parse->fd = -1;
}

int	parse_file(t_cub *cub, const char *path)
{
	t_parse	parse;
	int		fd;

	parse_init(&parse);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		fatal_parse(&parse, "Cannot open map file");
	parse_stream(&parse, fd);
	close(fd);
	ensure_config(&parse);
	validate_map(&parse);
	build_map(cub, &parse);
	return (1);
}
