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
		image->instances[0].y -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

// -----------------------------------------------------------------------------

/*
void render_scene(t_scene *scene)
{
    int     x;
    int     y;
    int     i;
    t_ray   ray;
    double  t;
    double  closest_t;
    int     hit_index;
    t_vec3  hit_point;
    t_vec3  normal;
    t_color obj_col;
    t_color amb_col;
    t_color diff_col;
    t_color final_col;

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            // 1) Generar rayo para este píxel
            ray = generate_ray(x, y, scene->camera);

            // 2) Buscar la esfera más cercana que intersecta este rayo
            hit_index = -1;
            closest_t = INFINITY;
            i = 0;
            while (i < scene->num_spheres)
            {
                if (hit_sphere(ray, &scene->spheres[i], &t))
                {
                    if (t > 0.0001 && t < closest_t) // evita hits "cerca" del origen
                    {
                        closest_t = t;
                        hit_index = i;
                    }
                }
                i++;
            }

            // 3) Si hubo intersección, calcular iluminación y pintar
            if (hit_index >= 0)
            {
                hit_point = get_hit_point(ray, closest_t);
                normal = get_normal_at_sphere(hit_point, &scene->spheres[hit_index]);
                obj_col = scene->spheres[hit_index].color;

                amb_col  = compute_ambient(&scene->ambient, obj_col);
                diff_col = compute_diffuse(&scene->light, obj_col, normal, hit_point);

                // suma canal a canal y clamp a 255
                final_col.r = amb_col.r + diff_col.r;
                final_col.g = amb_col.g + diff_col.g;
                final_col.b = amb_col.b + diff_col.b;
                if (final_col.r > 255) final_col.r = 255;
                if (final_col.g > 255) final_col.g = 255;
                if (final_col.b > 255) final_col.b = 255;

                mlx_put_pixel(image, x, y, ft_pixel(final_col.r, final_col.g, final_col.b, 255));
            }
            else
            {
                // fondo
                mlx_put_pixel(image, x, y, ft_pixel(0, 0, 0, 255));
            }

            x++;
        }
        y++;
    }
}
*/



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
	render_scene(&scene);

	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_scene(&scene);
	return (EXIT_SUCCESS);
}
