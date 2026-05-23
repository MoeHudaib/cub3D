/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:20 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:20 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	strip_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
}

/*
** Try to match the line against the four texture identifiers (NO/SO/WE/EA).
** Returns 1 if a match was found and handled, 0 otherwise.
*/
static int	try_parse_texture(t_config_parser *parser, char *line)
{
	static const char	*tex_ids[4] = {"NO", "SO", "WE", "EA"};
	int					i;

	i = 0;
	while (i < 4)
	{
		if (line[0] && line[1]
			&& !ft_strncmp(line, tex_ids[i], 2)
			&& (line[2] == ' ' || line[2] == '\t'))
		{
			set_texture_path(parser, i, line + 2);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
** Parse one header line (texture path or color definition).
** Returns 1 if the line was a recognised header, 0 if it belongs to the map.
*/
static int	try_parse_header(t_config_parser *parser, char *line)
{
	char	*trimmed;

	trimmed = line;
	while (*trimmed == ' ' || *trimmed == '\t')
		trimmed++;
	if (try_parse_texture(parser, trimmed))
		return (1);
	if ((trimmed[0] == 'F' || trimmed[0] == 'C')
		&& (trimmed[1] == ' ' || trimmed[1] == '\t'))
	{
		if (trimmed[0] == 'F')
			set_color(parser, 0, trimmed + 1);
		else
			set_color(parser, 1, trimmed + 1);
		return (1);
	}
	return (0);
}

/*
** Append one map row string to the parser's dynamic row array.
*/
static void	append_map_row(t_config_parser *parser, const char *line)
{
	char	**new_rows;
	int		i;

	new_rows = (char **)ft_calloc(parser->row_count + 2, sizeof(char *));
	if (!new_rows)
		parser_fatal(parser, "Memory allocation failed");
	i = 0;
	while (i < parser->row_count)
	{
		new_rows[i] = parser->map_rows[i];
		i++;
	}
	new_rows[i] = ft_strdup(line);
	if (!new_rows[i])
	{
		free(new_rows);
		parser_fatal(parser, "Memory allocation failed");
	}
	free(parser->map_rows);
	parser->map_rows = new_rows;
	parser->row_count++;
}

/*
** State-machine entry point for each raw line from the file.
** Blank lines switch from MAP phase to TRAILING; non-blank lines after
** the map started are an error.
*/
void	process_map_line(t_config_parser *parser, char *line)
{
	strip_newline(line);
	if (is_blank_line(line))
	{
		if (parser->phase == PHASE_MAP)
			parser->phase = PHASE_TRAILING;
		return ;
	}
	if (parser->phase == PHASE_TRAILING)
		parser_fatal(parser, "Content after map section");
	if (parser->phase == PHASE_HEADERS && try_parse_header(parser, line))
		return ;
	if (parser->phase == PHASE_HEADERS)
		parser->phase = PHASE_MAP;
	append_map_row(parser, line);
}
