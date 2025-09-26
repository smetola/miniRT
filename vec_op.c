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

double	point_distance(t_vec3 a, t_vec3 b)
{
	//taken from https://stackoverflow.com/questions/14568249/finding-the-distance-between-2-3d-points
	// (math.h is allowed)
	return (hypot(hypot(a.x - b.x, a.y - b.y), a.z - b.z));
}

double	point_to_plane_distance(t_vec3 point, t_plane plane)
{
	return	vec_dot(plane.normal, vec_sub(point, plane.shape.ori));
}

double	vec_length(t_vec3 v)
{
	return (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
}

int	is_empty_vec(t_vec3 v)
{
	return (v.x == 0 && v.y == 0 && v.z == 0);
}

t_ray	vec_rotate_by_plane(t_ray target, t_vec3 axis, t_vec3 reference)
{
	/*i first need to find out the angle between axis and reference in all 3 axis, which is obtained by the formula: acos(vec_dot(axis, reference)/(vec_length(axis)*vec_length(reference)))
	we find out the angle for each axis by projecting both vectors to the same plane, e.g.:

	axis.x = 0;
	reference.x = 0;
	x_angle = acos(...);

	once we have all 3 angles, then we can rotate target around r degrees for each axis
	
	X axis
	y' = cos(r)*y - sin(r)*z;
	z' = sin(r)*y + cos(r)*z;

	Y axis
	x' = cos(r)*x + sin(r)*z;
	z' = -sin(r)*x + cos(r)*z;

	Z axis
	x' = cos(r)*x - sin(r)*y;
	y' = sin(r)*x + cos(r)*y;

	i think i need to rotate both target.origin and target.direction?*/

	double	x_angle;
	double	z_angle;

	t_vec3	temp_axis = axis;
	t_vec3	temp_ref = reference;
	temp_axis.x = 0;
	temp_ref.x = 0;
	if (vec_length(temp_axis) < EPSILON || vec_length(temp_ref) < EPSILON)
		x_angle = 0;
	else
		x_angle = acos(vec_dot(temp_axis, temp_ref)/(vec_length(temp_axis)*vec_length(temp_ref)));

	temp_axis = axis;
	temp_ref = reference;
	temp_axis.z = 0;
	temp_ref.z = 0;

	if (vec_length(temp_axis) < EPSILON || vec_length(temp_ref) < EPSILON)
		z_angle = 0;
	else
		z_angle = acos(vec_dot(temp_axis, temp_ref)/(vec_length(temp_axis)*vec_length(temp_ref)));

	//x axis
	if (x_angle != 0)
	{
		target.origin.y = cos(x_angle)*target.origin.y - sin(x_angle)*target.origin.z; //@TODO: rotate origin too? idk
		target.direction.y = cos(x_angle)*target.direction.y - sin(x_angle)*target.direction.z;
	}
	//z axis
	if (z_angle != 0)
	{
		target.origin.y = sin(z_angle)*target.origin.x + cos(z_angle)*target.origin.y; //@TODO: rotate origin too? idk
		target.direction.y = sin(z_angle)*target.direction.x + cos(z_angle)*target.direction.y;
	}
	return	(target);
}

t_vec3	rotate_rodrigues(t_vec3 target, t_vec3 axis, double angle)
{
	t_vec3	term1 = vec_scale(target, cos(angle));
	t_vec3	term2 = vec_scale(vec_prod(axis, target), sin(angle));
	t_vec3	term3 = vec_scale(axis, vec_dot(axis, target) * (1 - cos(angle)));
	return (vec_add(term1, vec_add(term2, term3)));
}

t_ray	vec_cylinder_rotate(t_ray target, t_cylinder cylinder)
{
	//rodrigues formula
	t_vec3	up = {0, 1, 0};
	t_vec3	axis = vec_normalize(vec_prod(cylinder.axis, up)); //axis of rotation is perpendicular to both cylinder axis and up axis
	double	angle = acos(vec_dot(cylinder.axis, up));
	target.origin = rotate_rodrigues(vec_sub(target.origin, cylinder.shape.ori), axis, angle);
	//target.origin = rotate_rodrigues(target.origin, axis, angle);
	target.direction = rotate_rodrigues(target.direction, axis, angle);
	return (target);
}

t_ray	vec_camera_rotate(t_ray target, t_camera camera)
{
	//rodrigues formula
	t_vec3	forward = {0, 0, 1};
	t_vec3	axis = vec_normalize(vec_prod(camera.orient, forward)); //axis of rotation is perpendicular to both cylinder axis and up axis
	double	angle = acos(vec_dot(camera.orient, forward));
	//target.origin = rotate_rodrigues(vec_sub(target.origin, cylinder.shape.ori), axis, angle);
	target.origin = rotate_rodrigues(vec_sub(target.origin, camera.coord), axis, angle);
	target.direction = rotate_rodrigues(target.direction, axis, angle);
	return (target);
}

//used for calculating reflections, how vectors bounce off surfaces
t_vec3	reflect_vector(t_vec3 target, t_vec3 normal)
{
	return (vec_sub(target, vec_scale(normal, 2 * vec_dot(target, normal))));
}
