NAME = miniRT
MLX_DIR = mlx42-codam

CC = gcc
CFLAGS = -Wall -Werror -Wextra -I$(MLX_DIR)/include -Ilib/libft
LDFLAGS = -L$(MLX_DIR)/build -lmlx42 -lglfw -lXext -lX11 -lm -Llib/libft -lft

SRCS = main.c \
		parsing/init_parse_rt.c parsing/init_elements.c \
		parsing/init_objects.c parsing/parse_utils.c free.c \
		gnl/get_next_line.c gnl/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_BUILD = $(MLX_DIR)/build/Makefile
MLX_LIB = $(MLX_DIR)/build/libmlx42.a

all: $(LIBFT) $(MLX_LIB) $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX_LIB)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
    $(MAKE) -C $(LIBFT_DIR)

$(MLX_LIB): $(MLX_BUILD)
	cd $(MLX_DIR)/build && make

$(MLX_BUILD): $(MLX_DIR)
	cd $(MLX_DIR) && cmake -B build

$(MLX_DIR):
	git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
    if [ -d "$(MLX_DIR)/build" ]; then cd $(MLX_DIR)/build && make clean; fi

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
    if [ -d "$(MLX_DIR)/build" ]; then cd $(MLX_DIR)/build && make clean; fi

re: fclean all