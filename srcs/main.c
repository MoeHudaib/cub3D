/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:46:21 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:36:21 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static int	has_cub_extension(const char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 4)
		return (0);
	return (ft_strncmp(path + len - 4, ".cub", 4) == 0);
}

static void	validate_args(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		exit(1);
	}
	if (!has_cub_extension(argv[1]))
	{
		ft_putstr_fd("Error\n", 2);
		ft_putendl_fd("Invalid file extension", 2);
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;

	validate_args(argc, argv);
	ft_memset(&game, 0, sizeof(t_game));
	load_map_file(&game, argv[1]);
	init_player(&game);
	init_mlx(&game);
	load_textures(&game);
	start_game_loop(&game);
	destroy_game(&game);
	return (0);
}
