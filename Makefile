NAME = miniRT
MLX_DIR = mlx42-codam

CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iminilibx-linux
LDFLAGS = -L$(MLX_DIR)/build -lmlx42 -lglfw -lXext -lX11 -lm

SRCS = main.c

MLX_BUILD = $(MLX_DIR)/build/Makefile
MLX_LIB = $(MLX_DIR)/build/libmlx42.a

all: $(MLX_LIB) $(NAME)

$(NAME): $(SRCS) $(MLX_LIB)
	$(CC) $(SRCS) $(LDFLAGS) -o $(NAME)

$(MLX_LIB): $(MLX_BUILD)
	cd $(MLX_DIR)/build && make

$(MLX_BUILD): $(MLX_DIR)
	cd $(MLX_DIR) && cmake -B build

$(MLX_DIR):
	git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(MLX_DIR) clean

re: fclean all