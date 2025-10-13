#ifndef MINIRT_H
# define MINIRT_H

# include <string.h>
# include "lib/libft/libft.h"
# include "gnl/get_next_line.h"
# include "mlx42-codam/include/MLX42/MLX42.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>

# define WIDTH 512
# define HEIGHT 512
# define EPSILON 0.001
# define REFLECTION_INTENSITY 30 //cuanto mas alto, mas baja la intensidad. en 100 el reflejo es un punto pequeño, y en 1 o menos casi todo es blanco

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

extern mlx_image_t* image;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_ambient_lighting
{
	double		ratio;
	t_color		color;
}	t_amb_light;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_hit
{
	int		is_hit;
	double	distance;
	t_color	color;
	t_vec3	surface_normal;
	t_vec3	camera_dir;
	t_vec3	light_dir;
	t_vec3	reflection_dir;
}	t_hit;

typedef struct s_camera
{
	t_vec3	coord;
	t_vec3	orient;
	int		fov; // degrees
}	t_camera;

typedef struct s_light
{
	t_vec3	coord;
	double	bright;
}	t_light;

typedef struct s_sphere
{
	t_vec3	coord;
	double	diam;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	coord;
	t_vec3	ori;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	coord;
	t_vec3	ori;
	double	diam;
	double	hgt;
	t_color	color;
}	t_cylinder;

typedef struct	s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_scene
{
	t_amb_light	ambient;
	t_camera	camera;
	t_light		light;
	int			has_light;
	t_sphere	*spheres;
	int			num_spheres;
	t_plane		*planes;
	int			num_planes;
	t_cylinder	*cylinders;
	int			num_cylinders;
}	t_scene;

/* parsing and init (unchanged) */
void	init_scene(t_scene *scene);
int		init_ambient(char *line, t_scene *scene);
int		init_camera(char *line, t_scene *scene);
int		init_light(char *line, t_scene *scene);
int		init_sphere(char *line, t_scene *scene);
int		init_plane(char *line, t_scene *scene);
int		init_cylinder(char *line, t_scene *scene);
int		check_args(int argc, char *argv);
int		parse_rt_file(char *filename, t_scene *scene);
int		parse_color(char *str, t_color *color);
int		parse_vector(char *str, t_vec3 *vec, int is_vec);
int		ft_error(char *message);
void	free_split(char **split);
void	free_scene(t_scene *scene);
double	ft_atof(char *str);

/* color helpers (Santi) */
t_color	color_add(t_color c1, t_color c2);
t_color	color_sub(t_color c1, t_color c2);
t_color	color_prod(t_color c1, t_color c2);
t_color	color_scale(t_color c, double s);

/* vector ops (vec_op.c) */
t_vec3	vec_normalize(t_vec3 v);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_reverse(t_vec3 v);
t_vec3	vec_scale(t_vec3 v, double s);
t_vec3	vec_prod(t_vec3 a, t_vec3 b);
double	vec_dot(t_vec3 a, t_vec3 b);
double	vec_length(t_vec3 v);
double	dot_distance(t_vec3 a, t_vec3 b);
double	point_to_plane_distance(t_vec3 point, t_plane plane);
int		is_empty_vec(t_vec3 v);
t_vec3	reflect_vector(t_vec3 target, t_vec3 normal);
t_vec3	rotate_rodrigues(t_vec3 target, t_vec3 axis, double angle);

t_hit	get_sphere_hit(t_ray line, const t_sphere sphere);
double	get_plane_hit(t_ray line, const t_plane plane);
double	get_cylinder_hit(const t_ray line, const t_cylinder cylinder);

t_ray	vec_rotate_by_plane(t_ray target, t_vec3 axis, t_vec3 reference);
t_ray	vec_cylinder_rotate(t_ray target, t_cylinder cylinder);
t_ray	vec_camera_rotate(t_ray target, t_camera camera);

/* Santi's shading helpers (updated to take pointers to scene) */
t_vec3	get_hit_point(t_ray ray, double t);
t_vec3	get_normal_at_sphere(t_vec3 point, t_sphere *sphere);
t_color	compute_ambient(const t_amb_light *amb, t_color col);
t_color	compute_diffuse(const t_scene *scene, t_color obj_color, t_vec3 normal, t_vec3 hit_point);
t_color	compute_specular(const t_scene *scene, t_color obj_color, t_vec3 normal, t_vec3 hit_point);

/* ray + render */
int		hit_sphere(t_ray ray, const t_sphere s, double *t_out);
t_ray	generate_ray(int x, int y, t_camera cam);
t_hit	get_ray_hit(const int x, const int y, const t_scene *scene);
void	render_scene(const t_scene *scene);

#endif
