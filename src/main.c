/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:21 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:46:21 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	has_cub_extension(const char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 4)
		return (0);
	if (ft_strncmp(path + len - 4, ".cub", 4) == 0)
		return (1);
	return (0);
}

static void	init_cub(t_cub *cub)
{
	ft_memset(cub, 0, sizeof(t_cub));
}

int	main(int argc, char **argv)
{
	t_cub	cub;

	if (argc != 2)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (1);
	}
	if (!has_cub_extension(argv[1]))
	{
		ft_putstr_fd("Error\n", 2);
		ft_putendl_fd("Invalid file extension", 2);
		return (1);
	}
	init_cub(&cub);
	parse_file(&cub, argv[1]);
	init_player(&cub);
	init_mlx(&cub);
	load_textures(&cub);
	start_loop(&cub);
	destroy_cub(&cub);
	return (0);
}
