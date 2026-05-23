/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:13:21 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 15:28:06 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include <stdlib.h>
# include <unistd.h>

# define WIN_W 800
# define WIN_H 600
# define TEX_W 64
# define TEX_H 64
# define MOVE_SPEED 0.02
# define ROT_SPEED 0.01

# define K_ESC 65307
# define K_W 119
# define K_A 97
# define K_S 115
# define K_D 100
# define K_LEFT 65361
# define K_RIGHT 65363

# define TEX_NO 0
# define TEX_SO 1
# define TEX_WE 2
# define TEX_EA 3

typedef struct s_slice
{
	int		start;
	int		end;
	int		height;
	int		x;
	int		tex_x;
}			t_slice;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
}				t_img;

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
}				t_map;

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_keys
{
	int			w;
	int			s;
	int			a;
	int			d;
	int			left;
	int			right;
}				t_keys;

typedef struct s_mlx
{
	void		*ptr;
	void		*win;
	t_img		img;
}				t_mlx;

typedef struct s_ray
{
	double		dir_x;
	double		dir_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_x;
	double		delta_y;
	int			step_x;
	int			step_y;
	int			map_x;
	int			map_y;
	int			side;
	double		perp_dist;
	double		wall_x;
}				t_ray;

typedef struct s_cub
{
	t_mlx		mlx;
	t_map		map;
	t_player	player;
	t_keys		keys;
	t_img		textures[4];
	char		*tex_path[4];
	int			floor_color;
	int			ceiling_color;
}				t_cub;

typedef enum e_phase
{
	PH_HEADERS,
	PH_MAP,
	PH_TRAIL
}				t_phase;

typedef struct s_parse
{
	char		*tex[4];
	int			colors[2];
	char		**map_lines;
	int			map_size;
	t_phase		phase;
	char		*current_line;
	int			fd;
}				t_parse;

int				parse_file(t_cub *cub, const char *path);
void			parse_line_data(t_parse *parse, char *line);
void			validate_map(t_parse *parse);
void			build_map(t_cub *cub, t_parse *parse);
void			init_player(t_cub *cub);
void			init_mlx(t_cub *cub);
void			load_textures(t_cub *cub);
void			start_loop(t_cub *cub);
void			render(t_cub *cub);
void			draw_column(t_cub *cub, int x, t_ray *ray, int height);
int				select_texture(t_ray *ray);
int				select_tex_x(t_ray *ray, t_img *tex);
int				perform_raycast(t_cub *cub, int x, t_ray *ray);
int				handle_close(t_cub *cub);
int				handle_key_press(int keycode, t_cub *cub);
int				handle_key_release(int keycode, t_cub *cub);
int				loop_hook(t_cub *cub);
void			move_player(t_cub *cub);
void			rotate_player(t_cub *cub, double angle);
void			free_split(char **arr);
void			free0(void **ptr);
void			destroy_cub(t_cub *cub);
void			fatal_parse(t_parse *parse, const char *msg);
void			fatal_cub(t_cub *cub, const char *msg);
void			set_texture_path(t_parse *parse, int index, const char *value);
void			set_color_value(t_parse *parse, int index, const char *value);
int				is_line_empty(const char *line);
void			gnl_cleanup(int fd);

#endif
