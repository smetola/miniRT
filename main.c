#include "miniRT.h"

// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

mlx_image_t* image;

uint32_t ft_pixel(t_color color)
{
    return (color.r << 24 | color.g << 16 | color.b << 8 | 255);
}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

t_shape	*get_ray_shape(const int x, const int y, const t_scene scene)
{
	t_shape	*hit;
	double	dist;
	double	min;
	int		i;

	hit = NULL;
	min = 999; //@TODO max value
	i = 0;
	while (i < scene.num_spheres)
	{
		dist = get_sphere_distance(generate_ray(x, y, scene.camera), scene.spheres[i]);
		if (dist < min && dist > 0)
		{
			min = dist;
			hit = &(scene.spheres + i)->shape;
		}
		i++;
	}
	while (i < scene.num_planes)
	{
		dist = get_plane_distance(generate_ray(x, y, scene.camera), scene.planes[i]);
		if (dist < min && dist > 0)
		{
			min = dist;
			hit = &(scene.planes + i)->shape;
		}
		i++;
	}
	while (i < scene.num_cylinders)
	{
		dist = get_cylinder_distance(generate_ray(x, y, scene.camera), scene.cylinders[i]);
		if (dist < min && dist > 0)
		{
			min = dist;
			hit = &(scene.cylinders + i)->shape;
		}
		i++;
	}

	return (hit);
}

void render_scene(const t_scene scene)
{
	int x;
	int	y;
	t_shape *hit;
	uint32_t	color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			hit = get_ray_shape(x, y, scene);
			if (!hit) //no collision, use ambient light
			{
				color = ft_pixel(scene.ambient.color);
			}
			else
			{
				color = ft_pixel(hit->color);
				//color = hit->color.b;
				//printf("Printing %d-%d-%d\n", hit->color.b, hit->color.g, hit->color.r);
			}
			mlx_put_pixel(image, x, y, color);
			//ray = generate_ray_simple(x, y, scene->camera);
			/*if (hit_sphere(ray, sphere))
				mlx_put_pixel(image, x, y, ft_pixel(255, 0, 0, 255));
			else
				mlx_put_pixel(image, x, y, ft_pixel(0, 0, 0, 255));*/
			x++;
		}
		y++;
	}
}



int32_t main(int argc, char **argv)
{
	mlx_t* mlx;
	t_scene	scene;

	if (!check_args(argc, argv[1]) || !parse_rt_file(argv[1], &scene))
		return (1);
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
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	render_scene(scene);

	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_scene(&scene);
	return (EXIT_SUCCESS);
}