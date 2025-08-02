#include "miniRT.h"

/* 
O: origen rayo
D: dirección rayo
t: incognita, distancia recorrida dentro del rayo (hasta q choca)
P(t) = punto a distancia t del O

P(t) = O + t·D
|P(t) - C| = r -> |O + t·D - C|² = r²
dot(P(t) - C, P(t) - C) = r² ( |V|² = dot(V, V) = V.x² + V.y² + V.z², logitud del vector)
dot((O - C + t·D), (O - C + t·D)) = r²
oc = O - C
dot((oc + t·D), (oc + t·D)) = r²
dot(oc, oc) + 2t·dot(oc, D) + t²·dot(D, D) = r²
t²·dot(D, D) +  2t·dot(oc, D)+ dot(oc, oc) - r² = 0

A = dot(D, D)
B = 2·dot(oc, D)
C = dot(oc, oc) - r²
Solución: A·t² + B·t + C = 0
Δ = B² - 4·A·C
< 0 → No hay soluciones reales → no hay colisión
= 0 → Hay 1 punto → el rayo roza la esfera
> 0 → Hay 2 puntos → el rayo entra y sale de la esfera
 */

/*int	hit_sphere(t_ray ray, t_sphere *sphere)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	// oc = O - C
	oc = vec_sub(ray.origin, sphere->coord);
	// coeficientes de la cuadrática
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - (sphere->diam / 2) * (sphere->diam / 2);
	discriminant = b * b - 4 * a * c;
	// si el discriminante es negativo, no choca
	if (discriminant < 0)
		return (0);
	return (1); // choca
}*/


t_ray	generate_ray(const int x, const int y, const t_camera cam)
{
	t_ray	ray;
	t_vec3	pixel;
	double	u;
	double	v;
	double	aspect_ratio;
	double	fov_rad;
	double	viewport_height;
	double	viewport_width;

	aspect_ratio = (double)WIDTH / (double)HEIGHT;
	fov_rad = cam.fov * M_PI / 180.0; //convertimos de grados a radianes
	viewport_height = 2.0 * tan(fov_rad / 2.0); // vertical
	viewport_width = aspect_ratio * viewport_height;
	// convertimos los pixeles x,y a rango [-1, 1] para q el 0,0 esté en el centro y no arriba a la izda
	u = ((x + 0.5) / WIDTH - 0.5) * viewport_width;
	v = (0.5 - (y + 0.5) / HEIGHT) * viewport_height;
	// Suponemos plano z = 1, el punto a mirar está ahí
	pixel.x = u;
	pixel.y = v;
	pixel.z = 1.0;
	ray.origin = cam.coord;
	ray.direction = vec_normalize(vec_sub(pixel, cam.coord));
	return (ray);
}

