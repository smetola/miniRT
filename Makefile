NAME = miniRT
MLX_DIR = mlx42-codam

CC = gcc
CFLAGS = -Wall -Werror -Wextra -I. -I$(MLX_DIR)/include -Ilib/libft -lm
LDFLAGS = -L$(MLX_DIR)/build -lmlx42 -lglfw -lXext -lX11 -lm -Llib/libft -lft

HDRS = miniRT.h
SRCS = main.c free.c \
		parsing/init_parse_rt.c parsing/init_elements.c \
		parsing/init_objects.c parsing/parse_utils.c \
		vec_op/vec_op.c vec_op/vec_math.c vec_op/vec_utils.c \
		rendering/render.c rendering/ray.c rendering/shading.c \
		rendering/light.c rendering/color.c \
		geometry/cylinder_math.c geometry/intersections.c \
		gnl/get_next_line.c gnl/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_BUILD = $(MLX_DIR)/build/Makefile
MLX_LIB = $(MLX_DIR)/build/libmlx42.a

all: $(LIBFT) $(MLX_LIB) $(NAME)

$(NAME): $(HDRS) $(OBJS) $(LIBFT) $(MLX_LIB)
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

norm:
	norminette $(SRCS) $(LIBFT_DIR)

leaks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind_suppressions.supp ./$(NAME) scenes/error/template.rt

.PHONY: all clean fclean re norm leaks