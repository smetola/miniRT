#include "miniRT.h"

// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

mlx_image_t* image;

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
	{
		//image->instances[0].y -= 5;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	{
		//image->instances[0].y += 5;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		//image->instances[0].x -= 5;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		//image->instances[0].x += 5;
	}
}

t_vec3	get_pixel_angle(t_camera camera, uint32_t x, uint32_t y)
{
	//@TODO: get angle from pixel of screen in relation to height, width and camera position
}

t_point get_colliding_point(t_scene scene, t_vec3 angle)
{
	//@TODO: follow vector from camera until first finding a shape, and returning its color
	//if no collision, return closest point of the vector to the light object instead
}

uint32_t	ft_pixel_color(t_scene scene, uint32_t x, uint32_t y)
{
	uint32_t	base_color;
	t_vec3	angle = get_pixel_angle(scene.camera, x, y);
	t_point	point = get_colliding_point(scene, angle);
	if (!point.shape) //no shape found, use light and ambient light
		base_color = 0;
	else
		base_color = point.shape.color;
	if (scene.has_light)
	{
		//@TODO: calculate light intensity on point
		base_color += scene.light.bright;
	}
	//@TODO: add ambient light
	return (base_color + scene.ambient.color);
}

void	ft_render(mlx_t* mlx, mlx_image_t *image, t_scene scene)
{
	for (uint32_t x = 0; x < image->width; x++)
	{
		for (uint32_t y = 0; y < image->height; y++)
		{
			uint32_t color = x / 2; //placeholder
			mlx_put_pixel(image, x, y, color);
		}
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		//return(EXIT_FAILURE);
	}
	
}

// -----------------------------------------------------------------------------

int32_t main(int argc, char **argv)
{
	mlx_t* mlx;
	t_scene	scene;

	if (!check_args(argc, argv[1]) || !parse_rt_file(argv[1], &scene))
		return (1);
	free_scene(&scene);
	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	
	mlx_loop_hook(mlx, ft_hook, mlx);

	//render
	ft_render(mlx, image, scene);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
