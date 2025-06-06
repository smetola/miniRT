#include "mlx42-codam/include/MLX42/MLX42.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 512
#define HEIGHT 512

static mlx_image_t* image;

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
}	t_A;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_camera
{
	t_vec3 coord;
	t_vec3	ori;
	int		fov; //º!!
}	t_C;

typedef struct s_light
{
	t_vec3	coord;
	double	bright;
	//t_color	color;
}	t_L;

typedef struct s_sphere
{
	t_vec3	coord;
	double	diam;
	t_color	color;
}	t_sp;

typedef struct s_plane
{
	t_vec3	coord;
	t_vec3	ori;
	t_color	color;
}	t_pl;

typedef struct s_cylinder
{
	t_vec3	coord;
	t_vec3	ori;
	double	diam;
	double	hgt;
	t_color	color;
}	t_cy;


