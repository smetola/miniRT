#include "miniRT.h"

t_color	color_add(t_color c1, t_color c2)
{
	c1.b += c2.b;
	c1.r += c2.r;
	c1.g += c2.g;
	if (c1.b > 255)
		c1.b = 255;
	if (c1.r > 255)
		c1.r = 255;
	if (c1.g > 255)
		c1.g = 255;
	return (c1);
}

t_color	color_prod(t_color c1, t_color c2)
{
	c1.b *= c2.b;
	c1.r *= c2.r;
	c1.g *= c2.g;
	if (c1.b > 255)
		c1.b = 255;
	if (c1.r > 255)
		c1.r = 255;
	if (c1.g > 255)
		c1.g = 255;
	return (c1);
}

t_color	color_scale(t_color c, double s)
{
	c.b *= s;
	c.r *= s;
	c.g *= s;
	if (c.b > 255)
		c.b = 255;
	else if (c.b < 0)
		c.b = 0;
	if (c.r > 255)
		c.r = 255;
	else if (c.r < 0)
		c.r = 0;
	if (c.g > 255)
		c.g = 255;
	else if (c.g < 0)
		c.g = 0;
	return (c);
}
