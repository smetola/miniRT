#include "miniRT.h"

// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

mlx_image_t* image;

uint32_t ft_pixel(t_color color, int32_t a)
{
    return (color.r << 24 | color.g << 16 | color.b << 8 | a);
}

int32_t	ft_map(double x, double in_max, int32_t out_max) //out_min and in_min is 0
{
	if (x > in_max)
		return (out_max);
	return (x * out_max / in_max);
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

t_hit	get_ray_hit(const int x, const int y, const t_scene scene)
{
	t_hit	result;
	t_hit	temp;
	double	min_dist;
	int		i;

	result.is_hit = 0;
	min_dist = __DBL_MAX__;
	i = 0;
	while (i < scene.num_spheres)
	{
		temp = get_sphere_hit(generate_ray(x, y, scene.camera), scene.spheres[i]);
		if (temp.distance < min_dist && temp.distance > 0)
		{
			min_dist = temp.distance;
			result = temp;
		}
		i++;
	}
	/*i = 0;
	while (i < scene.num_planes)
	{
		dist = get_plane_hit(generate_ray(x, y, scene.camera), scene.planes[i]);
		if (dist < min && dist > 0) //&& dist = 0?
		{
			min = dist;
			hit = &(scene.planes + i)->shape;
		}
		i++;
	}
	i = 0;
	while (i < scene.num_cylinders)
	{
		dist = get_cylinder_hit(generate_ray(x, y, scene.camera), scene.cylinders[i]);
		if (dist < min && dist > 0)
		{
			min = dist;
			hit = &(scene.cylinders + i)->shape;
		}
		i++;
	}*/

	return (result);
}

void render_scene(const t_scene scene)
{
	int 		x;
	int			y;
	t_hit 		hit;
	uint32_t	color;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			hit = get_ray_hit(x, y, scene);
			if (!hit.is_hit) //no collision, use ambient light
			{
				//double	d = get_ray_to_point_distance(generate_ray(x, y, scene.camera), scene.light.coord); //max of about 35700 (40k?) maybe max(min) brightness should be 10k, and anything above that, map to int 255
				//color = ft_pixel(scene.ambient.color, ft_map(d, 5000, 255));
				color = ft_pixel(scene.ambient.color, 25); //map scene.ambient.ratio to 0~255 in second argument
			}
			else
			{
				//hit.color = color_scale(hit.color, sin(...)); //scale by sine of angle of light hitting the surface
				//hit.color = color_prod(hit.color, color_scale(scene.ambient.color, 0.01)); merge with ambient color (this should be add and not prod i think)
				color = ft_pixel(hit.color, 255);
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