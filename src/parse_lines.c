/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:20 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:20 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	remove_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
	{
		line[len - 1] = '\0';
		len--;
	}
}

static int	handle_texture(t_parse *parse, char *line)
{
	static const char	*ids[4] = {"NO", "SO", "WE", "EA"};
	int					i;

	i = 0;
	while (i < 4)
	{
		if (line[0] && line[1] && !ft_strncmp(line, ids[i], 2)
			&& (line[2] == ' ' || line[2] == '\t'))
		{
			set_texture_path(parse, i, line + 2);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	handle_header_line(t_parse *parse, char *line)
{
	char	*ptr;

	ptr = line;
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	if (handle_texture(parse, ptr))
		return (1);
	if ((ptr[0] == 'F' || ptr[0] == 'C') && (ptr[1] == ' ' || ptr[1] == '\t'))
	{
		if (ptr[0] == 'F')
			set_color_value(parse, 0, ptr + 1);
		else
			set_color_value(parse, 1, ptr + 1);
		return (1);
	}
	return (0);
}

static void	append_map_line(t_parse *parse, const char *line)
{
	char	**new_map;
	int		i;

	new_map = (char **)ft_calloc(parse->map_size + 2, sizeof(char *));
	if (!new_map)
		fatal_parse(parse, "Memory allocation failed");
	i = 0;
	while (i < parse->map_size)
	{
		new_map[i] = parse->map_lines[i];
		i++;
	}
	new_map[i] = ft_strdup(line);
	if (!new_map[i])
	{
		free(new_map);
		fatal_parse(parse, "Memory allocation failed");
	}
	free(parse->map_lines);
	parse->map_lines = new_map;
	parse->map_size++;
}

void	parse_line_data(t_parse *parse, char *line)
{
	remove_newline(line);
	if (is_line_empty(line))
	{
		if (parse->phase == PH_MAP)
			parse->phase = PH_TRAIL;
		return ;
	}
	if (parse->phase == PH_TRAIL)
		fatal_parse(parse, "Empty line inside map");
	if (parse->phase == PH_HEADERS && handle_header_line(parse, line))
		return ;
	if (parse->phase == PH_HEADERS)
		parse->phase = PH_MAP;
	append_map_line(parse, line);
}
