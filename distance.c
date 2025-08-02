#include "miniRT.h"

int	is_point_behind(const t_ray line, const t_vec3 hit) //comprueba si hit esta delante (1) o detras (0) del origen de line
{
	//x
	if (line.direction.x > 0 && line.origin.x > hit.x)
		return (0);
	if (line.direction.x < 0 && line.origin.x < hit.x)
		return (0);
	//y
	if (line.direction.y > 0 && line.origin.y > hit.y)
		return (0);
	if (line.direction.y < 0 && line.origin.y < hit.y)
		return (0);
	//z
	if (line.direction.z > 0 && line.origin.z > hit.z)
		return (0);
	if (line.direction.z < 0 && line.origin.z < hit.z)
		return (0);
	return (1);
}

double	get_sphere_distance(const t_ray line, const t_sphere sphere)
{
	(void)line;
	(void)sphere;
	return -1;
}
double	get_plane_distance(const t_ray line, const t_plane plane)
{
	//taken from https://stackoverflow.com/questions/7168484/3d-line-segment-and-plane-intersection
	//@TODO: caso en el que el vector es parte del plano? supongo que se puede dejar como invisible al ser un plano perpendicular a la cámara y no hay curvatura de la luz?
	double	d;

	if (vec_dot(plane.normal, line.direction) == 0) //paralelo al plano, nunca intersecan
		return (-1);
	d = vec_dot(plane.normal, plane.shape.coord); //todos los puntos X del plano cumplen la ecuacion vec_dot(plane.normal, X) = d
	float x = (d - vec_dot(plane.normal, line.origin)) / vec_dot(plane.normal, line.direction);
	t_vec3	hit = vec_add(line.origin, vec_scale(line.direction, x));
	if (!is_point_behind(line, hit)) //behind the screen, not valid
		return (-1);
	return	(dot_distance(hit, line.origin));
}

double	get_cylinder_distance(const t_ray line, const t_cylinder cylinder)
{
	(void)line;
	(void)cylinder;
	return -1;
}