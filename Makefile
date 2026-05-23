NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
IFLAGS = -Iinclude -Ilibft -Iminilibx-linux
LFLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm
LIBFT = libft/libft.a
SRC_DIR = src
OBJ_DIR = obj
SRCS = main.c parse.c parse_lines.c parse_color.c parse_map.c \
        parse_tex.c parse_utils.c validate_map.c init.c texture.c \
        render.c raycast.c draw.c draw_utils.c hooks.c loop.c move.c \
        rotate.c cleanup.c errors.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft $(LFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c include/cub3d.h | $(OBJ_DIR)
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
