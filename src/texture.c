/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:48:12 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:48:13 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	reset_graphics(t_cub *cub)
{
	int	i;

	cub->mlx.ptr = NULL;
	cub->mlx.win = NULL;
	cub->mlx.img.img = NULL;
	cub->mlx.img.addr = NULL;
	cub->mlx.img.width = 0;
	cub->mlx.img.height = 0;
	i = 0;
	while (i < 4)
	{
		cub->textures[i].img = NULL;
		cub->textures[i].addr = NULL;
		cub->textures[i].width = 0;
		cub->textures[i].height = 0;
		i++;
	}
	cub->keys.w = 0;
	cub->keys.s = 0;
	cub->keys.a = 0;
	cub->keys.d = 0;
	cub->keys.left = 0;
	cub->keys.right = 0;
}

void	init_mlx(t_cub *cub)
{
	reset_graphics(cub);
	cub->mlx.ptr = mlx_init();
	if (!cub->mlx.ptr)
		fatal_cub(cub, "Failed to init MLX");
	cub->mlx.win = mlx_new_window(cub->mlx.ptr, WIN_W, WIN_H, "cub3D");
	if (!cub->mlx.win)
		fatal_cub(cub, "Failed to create window");
	cub->mlx.img.img = mlx_new_image(cub->mlx.ptr, WIN_W, WIN_H);
	if (!cub->mlx.img.img)
		fatal_cub(cub, "Failed to create image");
	cub->mlx.img.addr = mlx_get_data_addr(cub->mlx.img.img, &cub->mlx.img.bpp,
			&cub->mlx.img.line_len, &cub->mlx.img.endian);
	if (!cub->mlx.img.addr)
		fatal_cub(cub, "Failed to access image data");
	cub->mlx.img.width = WIN_W;
	cub->mlx.img.height = WIN_H;
}

static void	load_texture(t_cub *cub, int index)
{
	t_img	*tex;

	tex = &cub->textures[index];
	tex->img = mlx_xpm_file_to_image(cub->mlx.ptr, cub->tex_path[index],
			&tex->width, &tex->height);
	if (!tex->img)
		fatal_cub(cub, "Failed to load texture");
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	if (!tex->addr)
		fatal_cub(cub, "Failed to read texture data");
}

void	load_textures(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		load_texture(cub, i);
		i++;
	}
}
