/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:27 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:28 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	find_max_width(t_parse *parse)
{
	int	max;
	int	len;
	int	i;

	max = 0;
	i = 0;
	while (i < parse->map_size)
	{
		len = ft_strlen(parse->map_lines[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

static void	copy_line(char *dst, const char *src, int width)
{
	int	i;

	ft_memset(dst, ' ', width);
	dst[width] = '\0';
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
}

static void	transfer_textures(t_cub *cub, t_parse *parse)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		cub->tex_path[i] = parse->tex[i];
		parse->tex[i] = NULL;
		i++;
	}
	cub->floor_color = parse->colors[0];
	cub->ceiling_color = parse->colors[1];
}

void	build_map(t_cub *cub, t_parse *parse)
{
	int	i;

	cub->map.height = parse->map_size;
	cub->map.width = find_max_width(parse);
	cub->map.grid = (char **)ft_calloc(cub->map.height, sizeof(char *));
	if (!cub->map.grid)
		fatal_parse(parse, "Memory allocation failed");
	i = 0;
	while (i < cub->map.height)
	{
		cub->map.grid[i] = (char *)malloc(cub->map.width + 1);
		if (!cub->map.grid[i])
			fatal_parse(parse, "Memory allocation failed");
		copy_line(cub->map.grid[i], parse->map_lines[i], cub->map.width);
		i++;
	}
	transfer_textures(cub, parse);
	free_split(parse->map_lines);
	parse->map_lines = NULL;
}
