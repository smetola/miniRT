#include "miniRT.h"

int	matrix_compare(t_matrix a, t_matrix b)
{
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4) {
			if (fabs(a.m[i][j] - b.m[i][j]) > EPSILON)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

t_matrix	matrix_scale(t_matrix a, double s)
{
	t_matrix	result;
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4) {
			result.m[i][j] = a.m[i][j] * s;
			j++;
		}
		i++;
	}
	return (result);
}

t_matrix	matrix_product(t_matrix a, t_matrix b)
{
	t_matrix	result;
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
			j++;
		}
		i++;
	}
	return (result);
}

t_matrix	matrix_transpose(t_matrix a)
{
	t_matrix	result;
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4) {
			result.m[i][j] = a.m[j][i];
			j++;
		}
		i++;
	}
	return (result);
}

double	matrix_cofactor(t_matrix a, int i, int j)
{
	return (0);
}

double	matrix_determinant(t_matrix a)
{
	double	result;
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		if (i % 2)
			result -= matrix_cofactor(a, i, 0) * a.m[i][0];
		else
			result += matrix_cofactor(a, i, 0) * a.m[i][0];
		i++;
	}
	return (result);
}

t_matrix	matrix_inverse(t_matrix a)
{
	t_matrix	result;
	double	m_determinant;
	int	i;
	int j;

	m_determinant = matrix_determinant(a);
	if (m_determinant == 0)
	{
		//no solution
	}
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.m[i][j] = matrix_cofactor(a, i, j);
			j++;
		}
		i++;
	}
	result = matrix_transpose(result);
	return (matrix_scale(result, m_determinant));
}
