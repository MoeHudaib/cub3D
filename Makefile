NAME        = cub3D
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
IFLAGS      = -Iinclude -Ilibft -Iminilibx-linux
LFLAGS      = -Lminilibx-linux -lmlx -lXext -lX11 -lm
LIBFT       = libft/libft.a
SRC_DIR     = srcs
OBJ_DIR     = obj

SRCS        = main.c             \
              engine_loop.c      \
              input_hooks.c      \
              player_movement.c  \
              raycaster.c        \
              renderer.c         \
              column_draw.c      \
              mlx_setup.c        \
              map_init.c         \
              file_parser.c      \
              line_processor.c   \
              color_parser.c     \
              texture_parser.c   \
              map_validator.c    \
              parser_utils.c     \
              cleanup.c          \
              error_handlers.c

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft $(LFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/game.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
