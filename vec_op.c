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

	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});

}



t_vec3	vec_sub(t_vec3 a, t_vec3 b)

{

	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});

}



t_vec3	vec_reverse(t_vec3 v)

{

	return ((t_vec3){-v.x, -v.y, -v.z});

}



t_vec3	vec_scale(t_vec3 v, double s)

{

	return ((t_vec3){v.x * s, v.y * s, v.z * s});

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

	return (hypot(hypot(a.x - b.x, a.y - b.y), a.z - b.z));

}



double	point_to_plane_distance(t_vec3 point, t_plane plane)

{

	return (vec_dot(plane.ori, vec_sub(point, plane.coord)));

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

	double	x_angle;

	double	z_angle;

	t_vec3	temp_axis;

	t_vec3	temp_ref;



	temp_axis = axis;

	temp_ref = reference;

	temp_axis.x = 0;

	temp_ref.x = 0;

	if (vec_length(temp_axis) < EPSILON || vec_length(temp_ref) < EPSILON)

		x_angle = 0;

	else

		x_angle = acos(vec_dot(temp_axis, temp_ref) / (vec_length(temp_axis) * vec_length(temp_ref)));

	temp_axis = axis;

	temp_ref = reference;

	temp_axis.z = 0;

	temp_ref.z = 0;

	if (vec_length(temp_axis) < EPSILON || vec_length(temp_ref) < EPSILON)

		z_angle = 0;

	else

		z_angle = acos(vec_dot(temp_axis, temp_ref) / (vec_length(temp_axis) * vec_length(temp_ref)));

	if (x_angle != 0)

	{

		target.origin.y = cos(x_angle) * target.origin.y - sin(x_angle) * target.origin.z;

		target.direction.y = cos(x_angle) * target.direction.y - sin(x_angle) * target.direction.z;

	}

	if (z_angle != 0)

	{

		target.origin.y = sin(z_angle) * target.origin.x + cos(z_angle) * target.origin.y;

		target.direction.y = sin(z_angle) * target.direction.x + cos(z_angle) * target.direction.y;

	}

	return (target);

}



t_vec3	rotate_rodrigues(t_vec3 target, t_vec3 axis, double angle)

{

	t_vec3	term1;

	t_vec3	term2;

	t_vec3	term3;



	term1 = vec_scale(target, cos(angle));

	term2 = vec_scale(vec_prod(axis, target), sin(angle));

	term3 = vec_scale(axis, vec_dot(axis, target) * (1 - cos(angle)));

	return (vec_add(term1, vec_add(term2, term3)));

}



t_ray	vec_cylinder_rotate(t_ray target, t_cylinder cylinder)

{

	t_vec3	up;

	t_vec3	axis;

	double	angle;



	up = (t_vec3){0,1,0};

	axis = vec_normalize(vec_prod(cylinder.ori, up));

	angle = acos(vec_dot(cylinder.ori, up));

	target.origin = rotate_rodrigues(vec_sub(target.origin, cylinder.coord), axis, angle);

	target.direction = rotate_rodrigues(target.direction, axis, angle);

	return (target);

}



t_ray	vec_camera_rotate(t_ray target, t_camera camera)

{

	t_vec3	forward;

	t_vec3	axis;

	double	angle;



	forward = (t_vec3){0,0,1};

	axis = vec_normalize(vec_prod(camera.orient, forward));

	angle = acos(vec_dot(camera.orient, forward));

	target.origin = rotate_rodrigues(vec_sub(target.origin, camera.coord), axis, angle);

	target.direction = rotate_rodrigues(target.direction, axis, angle);

	return (target);

}



t_vec3	reflect_vector(t_vec3 target, t_vec3 normal)

{

	return (vec_sub(target, vec_scale(normal, 2 * vec_dot(target, normal))));

}



/* compute normal for finite cylinder (world-space):

   normal = normalize( hit_point - cylinder_center - proj_axis )

*/

t_vec3	compute_cylinder_normal(t_vec3 hit_point, const t_cylinder *cyl)

{

	t_vec3	v;

	t_vec3	axis;

	t_vec3	proj;

	t_vec3	perp;



	v = vec_sub(hit_point, cyl->coord);

	axis = vec_normalize(cyl->ori);

	proj = vec_scale(axis, vec_dot(v, axis));

	perp = vec_sub(v, proj);

	return (vec_normalize(perp));

}

