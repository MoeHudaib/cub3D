/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:48:41 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:48:41 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			gnl_has_newline(char *s);
char		*gnl_join(char *s1, char *s2);
char		*gnl_extract(char *stash);
char		*gnl_trim(char *stash);

char	**stash_table(void)
{
	static char	*stash[FD_MAX];

	return (stash);
}

static int	append_to_stash(char **slot, char *buf, int fd)
{
	char	*tmp;

	tmp = gnl_join(*slot, buf);
	if (!tmp)
	{
		*slot = NULL;
		gnl_cleanup(fd);
		return (-1);
	}
	*slot = tmp;
	return (0);
}

static int	read_to_stash(int fd)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes;
	char	**stash;

	stash = stash_table();
	if (gnl_has_newline(stash[fd]))
		return (1);
	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		if (append_to_stash(&stash[fd], buffer, fd) < 0)
			return (-1);
		if (gnl_has_newline(stash[fd]))
			return (1);
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes < 0)
	{
		gnl_cleanup(fd);
		return (-1);
	}
	return (0);
}

static char	*finish_line(int fd)
{
	char	**stash;
	char	*line;

	stash = stash_table();
	line = gnl_extract(stash[fd]);
	if (!line)
	{
		gnl_cleanup(fd);
		return (NULL);
	}
	stash[fd] = gnl_trim(stash[fd]);
	return (line);
}

char	*get_next_line(int fd)
{
	int		status;
	char	**stash;
	char	*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	stash = stash_table();
	status = read_to_stash(fd);
	if (status == -1)
		return (NULL);
	if (status == 0 && (!stash[fd] || !stash[fd][0]))
	{
		gnl_cleanup(fd);
		return (NULL);
	}
	line = finish_line(fd);
	if (!line)
		return (NULL);
	return (line);
}
