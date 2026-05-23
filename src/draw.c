/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:45:40 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:45:41 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	dst = img->addr + y * img->line_len + x * (img->bpp / 8);
	*(unsigned int *)dst = color;
}

static void	draw_background(t_cub *cub, t_slice *slice)
{
	int	y;

	y = 0;
	while (y < slice->start)
	{
		put_pixel(&cub->mlx.img, slice->x, y, cub->ceiling_color);
		y++;
	}
	y = slice->end + 1;
	while (y < WIN_H)
	{
		put_pixel(&cub->mlx.img, slice->x, y, cub->floor_color);
		y++;
	}
}

static void	compute_bounds(t_slice *slice)
{
	slice->start = WIN_H / 2 - slice->height / 2;
	slice->end = WIN_H / 2 + slice->height / 2;
	if (slice->start < 0)
		slice->start = 0;
	if (slice->end >= WIN_H)
		slice->end = WIN_H - 1;
}

static void	draw_slice(t_cub *cub, t_slice *slice, t_img *tex)
{
	double	step;
	double	tex_pos;
	int		y;
	int		tex_y;
	char	*addr;

	step = (double)tex->height / slice->height;
	tex_pos = (slice->start - WIN_H / 2 + slice->height / 2) * step;
	y = slice->start;
	while (y <= slice->end)
	{
		if (tex_pos < 0)
			tex_pos = 0;
		if (tex_pos >= tex->height)
			tex_pos = tex->height - 1;
		tex_y = (int)tex_pos;
		addr = tex->addr + tex_y * tex->line_len + slice->tex_x * (tex->bpp
				/ 8);
		put_pixel(&cub->mlx.img, slice->x, y, *(unsigned int *)addr);
		tex_pos += step;
		y++;
	}
}

void	draw_column(t_cub *cub, int x, t_ray *ray, int height)
{
	t_slice	slice;
	t_img	*tex;

	if (height < 1)
		height = 1;
	slice.height = height;
	slice.x = x;
	compute_bounds(&slice);
	draw_background(cub, &slice);
	tex = &cub->textures[select_texture(ray)];
	slice.tex_x = select_tex_x(ray, tex);
	draw_slice(cub, &slice, tex);
}
