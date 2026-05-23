/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:14 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:47:14 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Skip leading whitespace, read a decimal integer, skip trailing whitespace.
** Returns the value, or -1 if no digit was found or value exceeds 255.
*/
static int	read_color_channel(const char **cursor)
{
	int	value;

	while (**cursor == ' ' || **cursor == '\t')
		(*cursor)++;
	if (!ft_isdigit(**cursor))
		return (-1);
	value = 0;
	while (ft_isdigit(**cursor))
		value = value * 10 + (*(*cursor)++ - '0');
	if (value > 255)
		return (-1);
	while (**cursor == ' ' || **cursor == '\t')
		(*cursor)++;
	return (value);
}

/*
** Ensure nothing but whitespace remains after the third channel.
*/
static void	ensure_no_trailing_garbage(t_config_parser *parser,
	const char *cursor)
{
	while (*cursor)
	{
		if (*cursor != ' ' && *cursor != '\t')
			parser_fatal(parser, "Invalid color format: trailing characters");
		cursor++;
	}
}

/*
** After reading a channel, verify a comma separator follows (skip for last).
*/
static void	expect_comma(t_config_parser *parser, const char **cursor,
	int channel_index)
{
	if (channel_index == 2)
		return ;
	if (**cursor != ',')
		parser_fatal(parser, "Invalid color format: expected ','");
	(*cursor)++;
}

/*
** Parse a "R,G,B" string and store the packed 24-bit colour in parser->colors.
** Duplicate identifiers are rejected.  Index 0 = floor, 1 = ceiling.
*/
void	set_color(t_config_parser *parser, int index, const char *value)
{
	const char	*cursor;
	int			channels[3];
	int			i;

	if (parser->colors[index] != -1)
		parser_fatal(parser, "Duplicate color identifier");
	cursor = value;
	i = 0;
	while (i < 3)
	{
		channels[i] = read_color_channel(&cursor);
		if (channels[i] < 0)
			parser_fatal(parser, "Invalid color channel value (0-255)");
		expect_comma(parser, &cursor, i);
		i++;
	}
	ensure_no_trailing_garbage(parser, cursor);
	parser->colors[index] = (channels[0] << 16) | (channels[1] << 8)
		| channels[2];
}
