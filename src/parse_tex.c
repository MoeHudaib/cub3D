/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:34 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:34 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*trim_path(const char *value)
{
	int	start;
	int	end;

	start = 0;
	while (value[start] == ' ' || value[start] == '\t')
		start++;
	end = ft_strlen(value + start);
	if (end == 0)
		return (NULL);
	while (end > 0 && (value[start + end - 1] == ' ' || value[start + end
				- 1] == '\t'))
		end--;
	return (ft_substr(value + start, 0, end));
}

void	set_texture_path(t_parse *parse, int index, const char *value)
{
	char	*path;

	if (parse->tex[index])
		fatal_parse(parse, "Duplicate texture identifier");
	path = trim_path(value);
	if (!path || path[0] == '\0')
	{
		free(path);
		fatal_parse(parse, "Invalid texture path");
	}
	parse->tex[index] = path;
}
