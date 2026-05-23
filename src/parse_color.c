/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:14 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:14 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_component(const char **str)
{
	int	value;

	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (!ft_isdigit(**str))
		return (-1);
	value = 0;
	while (ft_isdigit(**str))
	{
		value = value * 10 + (**str - '0');
		(*str)++;
	}
	if (value < 0 || value > 255)
		return (-1);
	while (**str == ' ' || **str == '\t')
		(*str)++;
	return (value);
}

static void	check_trailing(t_parse *parse, const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			fatal_parse(parse, "Invalid color format");
		str++;
	}
}

static void	validate_separator(t_parse *parse, const char **str, int index)
{
	if (index == 2)
		return ;
	if (**str != ',')
		fatal_parse(parse, "Invalid color format");
	(*str)++;
}

void	set_color_value(t_parse *parse, int index, const char *value)
{
	const char	*str;
	int			rgb[3];
	int			i;

	if (parse->colors[index] != -1)
		fatal_parse(parse, "Duplicate color identifier");
	str = value;
	i = 0;
	while (i < 3)
	{
		rgb[i] = parse_component(&str);
		if (rgb[i] < 0)
			fatal_parse(parse, "Invalid color value");
		validate_separator(parse, &str, i);
		i++;
	}
	check_trailing(parse, str);
	parse->colors[index] = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
}
