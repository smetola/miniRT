#include "miniRT.h"

t_vec3	vec_normalize(t_vec3 v)
{
	double	length;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0)
		return (v);
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return (v);
}

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

t_vec3	vec_reverse(t_vec3 v)
{
	return (t_vec3){-v.x, -v.y, -v.z};
}

t_vec3	vec_scale(t_vec3 v, double s)
{
	return (t_vec3){v.x * s, v.y * s, v.z * s};
}

t_vec3	vec_prod(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

double	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

double	dot_distance(t_vec3 a, t_vec3 b)
{
	//taken from https://stackoverflow.com/questions/14568249/finding-the-distance-between-2-3d-points
	// (math.h is allowed)
	return (hypot(hypot(a.x - b.x, a.y - b.y), a.z - b.z));
	//the following method also works and doesnt use macros, in case they're not allowed
	/*double	x;
	double	y;
	double	z;

	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
	x = pow(x, 2);
	y = pow(y, 2);
	z = pow(z, 2);
	return (sqrt(x + y + z));*/
}

double	point_to_plane_distance(t_vec3 point, t_plane plane)
{
	return	vec_dot(plane.ori, vec_sub(point, plane.coord));
}

double	vec_length(t_vec3 v)
{
	return (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
}

int	is_empty_vec(t_vec3 v)
{
	return (v.x == 0 && v.y == 0 && v.z == 0);
}

t_vec3	ray_distance(t_vec3 start, t_vec3 direction, double distance)
{
	return vec_add(start, vec_scale(direction, distance));
}

t_vec3	rotate_rodrigues(t_vec3 target, t_vec3 axis, double angle)
{
	t_vec3	term1 = vec_scale(target, cos(angle));
	t_vec3	term2 = vec_scale(vec_prod(axis, target), sin(angle));
	t_vec3	term3 = vec_scale(axis, vec_dot(axis, target) * (1 - cos(angle)));
	return (vec_add(term1, vec_add(term2, term3)));
}

/*t_ray	vec_rotate_by_plane(t_ray target, t_plane plane)
{
	//rodrigues formula
	target.origin = rotate_rodrigues(target.origin, plane.rotation_axis, plane.rotation_angle);
	target.direction = rotate_rodrigues(target.direction, plane.rotation_axis, plane.rotation_angle);
	return (target);
}*/

/*t_ray	vec_cylinder_rotate(t_ray target, t_cylinder cylinder, int reverse)
{
	//rodrigues formula
	double	angle = cylinder.rotation_angle;
	if (reverse)
		angle = -angle;
	target.origin = rotate_rodrigues(vec_sub(target.origin, cylinder.coord), cylinder.rotation_axis, angle);
	//target.origin = rotate_rodrigues(target.origin, axis, angle);
	target.direction = rotate_rodrigues(target.direction, cylinder.rotation_axis, angle);
	return (target);
}*/

t_ray	vec_camera_rotate(t_ray target, t_camera camera)
{
	//rodrigues formula
	target.origin = rotate_rodrigues(vec_sub(target.origin, camera.coord), camera.rotation_axis, camera.rotation_angle);
	target.direction = rotate_rodrigues(target.direction, camera.rotation_axis, camera.rotation_angle);
	return (target);
}

//used for calculating reflections, how vectors bounce off surfaces
t_vec3	reflect_vector(t_vec3 target, t_vec3 normal)
{
	return (vec_sub(target, vec_scale(normal, 2 * vec_dot(target, normal))));
}
