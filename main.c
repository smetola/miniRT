/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntuser <ubuntuser@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:00:00 by smetola           #+#    #+#             */
/*   Updated: 2025/11/17 21:39:04 by ubuntuser        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

mlx_image_t	*g_image; //todo delete global var?

static void	ft_hook(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
}

static mlx_t	*init_mlx_window(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return (NULL);
	}
	g_image = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!g_image)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return (NULL);
	}
	if (mlx_image_to_window(mlx, g_image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return (NULL);
	}
	return (mlx);
}

int32_t	main(int argc, char **argv)
{
	mlx_t	*mlx;
	t_scene	scene;

	if (!check_args(argc, argv[1]) || !parse_rt_file(argv[1], &scene))
		return (1);
	mlx = init_mlx_window();
	if (!mlx)
		return (EXIT_FAILURE);
	render_scene(scene);
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_scene(&scene);
	return (EXIT_SUCCESS);
}
