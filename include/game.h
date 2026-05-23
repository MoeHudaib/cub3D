/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhdeeb <mhdeeb@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:13:21 by mhdeeb            #+#    #+#             */
/*   Updated: 2026/05/23 19:35:59 by mhdeeb           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include "../minilibx-linux/mlx.h"
# include <stdlib.h>
# include <unistd.h>

/*
** Window & rendering constants
*/
# define WIN_WIDTH      800
# define WIN_HEIGHT     600
# define TEX_WIDTH      64
# define TEX_HEIGHT     64

/*
** Player movement constants
*/
# define MOVE_SPEED     0.02
# define ROTATE_SPEED   0.01

/*
** Keyboard keycodes
*/
# define KEY_ESCAPE      65307
# define KEY_W           119
# define KEY_A           97
# define KEY_S           115
# define KEY_D           100
# define KEY_ARROW_LEFT  65361
# define KEY_ARROW_RIGHT 65363

/*
** Texture direction indices
*/
# define TEX_NORTH  0
# define TEX_SOUTH  1
# define TEX_WEST   2
# define TEX_EAST   3

/*
** MLX event codes
*/
# define EVENT_WINDOW_CLOSE  17
# define EVENT_KEY_PRESS     2
# define EVENT_KEY_RELEASE   3
# define EVENT_MASK_KEY_PRESS   (1L << 0)
# define EVENT_MASK_KEY_RELEASE (1L << 1)

/*
** A single texture or framebuffer image stored in MLX memory.
*/
typedef struct s_image
{
	void		*mlx_img;
	char		*pixel_data;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_image;

/*
** The world map: a 2-D grid of characters plus its dimensions.
*/
typedef struct s_world_map
{
	char		**grid;
	int			width;
	int			height;
}				t_world_map;

/*
** Player position, view direction, and camera plane vectors.
** dir and plane together define the field of view.
*/
typedef struct s_player
{
	double		pos_x;
	double		pos_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

/*
** Boolean flags for every key the engine polls each frame.
*/
typedef struct s_key_state
{
	int			move_forward;
	int			move_backward;
	int			strafe_left;
	int			strafe_right;
	int			rotate_left;
	int			rotate_right;
}				t_key_state;

/*
** MLX context: connection handle, window, and the single framebuffer.
*/
typedef struct s_mlx_context
{
	void		*handle;
	void		*window;
	t_image		framebuffer;
}				t_mlx_context;

/*
** Intermediate DDA ray used during raycasting for one screen column.
*/
typedef struct s_ray
{
	double		dir_x;
	double		dir_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	int			step_x;
	int			step_y;
	int			map_x;
	int			map_y;
	int			side;
	double		perp_wall_dist;
	double		wall_hit_x;
}				t_ray;

/*
** Describes the visible slice of a wall column that will be drawn.
*/
typedef struct s_wall_slice
{
	int			draw_start;
	int			draw_end;
	int			height;
	int			screen_x;
	int			tex_col;
}				t_wall_slice;

/*
** Top-level game state passed through every subsystem.
*/
typedef struct s_game
{
	t_mlx_context	mlx;
	t_world_map		map;
	t_player		player;
	t_key_state		keys;
	t_image			textures[4];
	char			*texture_paths[4];
	int				floor_color;
	int				ceiling_color;
}				t_game;

/*
** Parser phases: reading header identifiers, then map rows, then trailing.
*/
typedef enum e_parser_phase
{
	PHASE_HEADERS,
	PHASE_MAP,
	PHASE_TRAILING
}				t_parser_phase;

/*
** All state needed while parsing the .cub file before building t_game.
*/
typedef struct s_config_parser
{
	char			*texture_paths[4];
	int				colors[2];
	char			**map_rows;
	int				row_count;
	t_parser_phase	phase;
	char			*current_line;
	int				fd;
}				t_config_parser;

/* ── Parsing ────────────────────────────────────────────────────────────── */
int		load_map_file(t_game *game, const char *path);
void	process_map_line(t_config_parser *parser, char *line);
void	validate_map(t_config_parser *parser);
void	build_game_map(t_game *game, t_config_parser *parser);

/* ── Initialisation ─────────────────────────────────────────────────────── */
void	init_player(t_game *game);
void	init_mlx(t_game *game);
void	load_textures(t_game *game);

/* ── Game loop ──────────────────────────────────────────────────────────── */
void	start_game_loop(t_game *game);

/* ── Rendering ──────────────────────────────────────────────────────────── */
void	render_frame(t_game *game);
void	draw_wall_column(t_game *game, int screen_x, t_ray *ray, int height);
int		select_wall_texture(t_ray *ray);
int		compute_tex_column(t_ray *ray, t_image *tex);
int		cast_ray(t_game *game, int screen_x, t_ray *ray);

/* ── Input handlers ─────────────────────────────────────────────────────── */
int		on_window_close(t_game *game);
int		on_key_press(int keycode, t_game *game);
int		on_key_release(int keycode, t_game *game);
int		on_frame_tick(t_game *game);

/* ── Player movement ────────────────────────────────────────────────────── */
void	update_player_movement(t_game *game);
void	rotate_player(t_game *game, double angle);

/* ── Utilities ──────────────────────────────────────────────────────────── */
int		is_blank_line(const char *line);
void	safe_free(void **ptr);
void	free_string_array(char **arr);

/* ── Cleanup & errors ───────────────────────────────────────────────────── */
void	destroy_game(t_game *game);
void	parser_fatal(t_config_parser *parser, const char *msg);
void	game_fatal(t_game *game, const char *msg);
void	set_texture_path(t_config_parser *parser, int index, const char *value);
void	set_color(t_config_parser *parser, int index, const char *value);
void	gnl_cleanup(int fd);

#endif
