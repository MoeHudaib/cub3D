/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   column_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:47:59 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:34:03 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/*
** Write a single pixel into the framebuffer at (col, row).
** Returns 1 if coordinates are valid, 0 if out of bounds.
*/
static int	put_pixel(t_image *fb, int col, int row, int color)
{
	char	*pixel;

	if (col < 0 || col >= WIN_WIDTH || row < 0 || row >= WIN_HEIGHT)
		return (0);
	pixel = fb->pixel_data + (row * fb->line_length + col * (fb->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
	return (1);
}

/*
** Sample one pixel from a texture image at (tex_col, tex_row).
*/
static int	sample_texture(t_image *tex, int tex_col, int tex_row)
{
	char	*pixel;

	pixel = tex->pixel_data
		+ (tex_row * tex->line_length + tex_col * (tex->bits_per_pixel / 8));
	return (*(unsigned int *)pixel);
}

/*
** Choose the texture index (N/S/W/E) based on which face the ray hit.
*/
int	select_wall_texture(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (TEX_EAST);
		return (TEX_WEST);
	}
	if (ray->step_y > 0)
		return (TEX_SOUTH);
	return (TEX_NORTH);
}

/*
** Map wall_hit_x (0..1) to a texture column index (0..tex->width-1).
*/
int	compute_tex_column(t_ray *ray, t_image *tex)
{
	int	tex_col;

	tex_col = (int)(ray->wall_hit_x * tex->width);
	if (tex_col < 0)
		tex_col = 0;
	if (tex_col >= tex->width)
		tex_col = tex->width - 1;
	return (tex_col);
}

/*
** Fill ceiling pixels (above the wall) and floor pixels (below the wall)
** for one screen column with their respective flat colours.
*/
static void	draw_ceiling_and_floor(t_game *game, int screen_x,
	int draw_start, int draw_end)
{
	int	row;

	row = 0;
	while (row < draw_start)
	{
		put_pixel(&game->mlx.framebuffer, screen_x, row, game->ceiling_color);
		row++;
	}
	row = draw_end + 1;
	while (row < WIN_HEIGHT)
	{
		put_pixel(&game->mlx.framebuffer, screen_x, row, game->floor_color);
		row++;
	}
}

/*
** Draw the textured wall strip for one screen column.
**
** tex_pos tracks which row of the texture corresponds to the current screen
** row.  It must be initialised from the *unclipped* draw_start (i.e. where
** the wall would begin if the screen were infinitely tall) so that texture
** alignment stays anchored to the wall centre even when the wall is taller
** than the screen and draw_start has been clamped to 0.
**
** tex_row is clamped with % rather than a bitmask so that non-power-of-two
** texture heights (rare but possible with XPM files) are handled correctly.
*/
static void	draw_textured_wall(t_game *game, t_wall_slice *slice, t_ray *ray)
{
	t_image	*tex;
	int		tex_index;
	int		tex_row;
	double	step;
	double	tex_pos;
	int		unclipped_draw_start;
	int		row;

	if (slice->draw_start > slice->draw_end || slice->height <= 0)
		return ;
	tex_index = select_wall_texture(ray);
	tex = &game->textures[tex_index];
	slice->tex_col = compute_tex_column(ray, tex);
	step = (double)tex->height / (double)slice->height;
	if (step <= 0)
		step = 1.0;
	unclipped_draw_start = (int)(WIN_HEIGHT / 2 - slice->height / 2.0);
	tex_pos = (slice->draw_start - unclipped_draw_start) * step;
	row = slice->draw_start;
	while (row <= slice->draw_end)
	{
		tex_row = (int)tex_pos % tex->height;
		if (tex_row < 0)
			tex_row += tex->height;
		put_pixel(&game->mlx.framebuffer, slice->screen_x, row,
			sample_texture(tex, slice->tex_col, tex_row));
		tex_pos += step;
		row++;
	}
}

/*
** Entry point called by the renderer for each column.
** Calculates the visible wall slice bounds, then delegates to helpers.
*/
void	draw_wall_column(t_game *game, int screen_x, t_ray *ray, int height)
{
	t_wall_slice	slice;

	if (screen_x < 0 || screen_x >= WIN_WIDTH || height <= 0)
		return ;
	slice.height = height;
	slice.screen_x = screen_x;
	slice.draw_start = WIN_HEIGHT / 2 - height / 2;
	if (slice.draw_start < 0)
		slice.draw_start = 0;
	slice.draw_end = WIN_HEIGHT / 2 + height / 2;
	if (slice.draw_end >= WIN_HEIGHT)
		slice.draw_end = WIN_HEIGHT - 1;
	draw_ceiling_and_floor(game, screen_x, slice.draw_start, slice.draw_end);
	draw_textured_wall(game, &slice, ray);
}
