#ifndef MINIRT_H
# define MINIRT_H
# define _USE_MATH_DEFINES

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

extern mlx_image_t* image;

typedef struct s_color //@TODO: borrar, en su lugar parsear todo a uint32_t
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

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_camera
{
	t_vec3 coord;
	t_vec3	orient;
	int		fov; //º!!
}	t_camera;

typedef struct s_light
{
	t_vec3	coord;
	double	bright;
	//t_color	color;
}	t_light;

typedef struct s_shape
{
	t_vec3	ori;
	t_color	color;
}	t_shape;

typedef struct s_sphere
{
	t_shape	shape;
	double	diam;
}	t_sphere;

typedef struct s_plane
{
	t_shape	shape;
	t_vec3	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_shape	shape;
	t_vec3	axis;
	double	diam;
	double	hgt;
}	t_cylinder;

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

typedef struct	s_point
{
	t_vec3	pos;
	void	*shape;
}	t_point;

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

t_vec3	vec_normalize(t_vec3 v);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_scale(t_vec3 v, double s);
t_vec3	vec_prod(t_vec3 a, t_vec3 b);
double	vec_dot(t_vec3 a, t_vec3 b);
double	dot_distance(t_vec3 a, t_vec3 b);
int		is_empty_vec(t_vec3 v);

t_ray	generate_ray(const int x, const int y, const t_camera cam);

double	get_sphere_distance(const t_ray line, const t_sphere sphere);
double	get_plane_distance(const t_ray line, const t_plane plane);
double	get_cylinder_distance(const t_ray line, const t_cylinder cylinder);

int		ft_error(char *message);
void	free_split(char **split);
void	free_scene(t_scene *scene);
double	ft_atof(char *str);

#endif