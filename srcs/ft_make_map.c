#include "../includes/fdf.h"

void	ft_init_calc_map(t_calc_map *c, t_node **ref)
{
	c->i = 0;
	c->j = 0;
	(*ref)->min = 322;
	(*ref)->max = -322;
}

void	ft_add_max_dent(t_calc_map *c, t_node **ref, char **arr)
{
		c->num_arr[c->j] = (int *)malloc(sizeof(int) * 2);
		c->num_arr[c->j][0] = ft_atoi(arr[c->j]);
		if (c->num_arr[c->j][0] - c->j> (*ref)->max)
		{
			(*ref)->max = c->num_arr[c->j][0] - c->j;
			(*ref)->max_col = c->j;
		}
		if (c->num_arr[c->j][0] < (*ref)->min)
		{
			(*ref)->min = c->num_arr[c->j][0] - c->j;
			(*ref)->min_col = c->j;
		}
		c->k = 0;
}

int	ft_color_input(t_calc_map *c, char **arr)
{
	if (!valid_color(&arr[c->j][c->k + 1]))
		c->num_arr[c->j][1] = 0xFF0000;
	else
		c->num_arr[c->j][1] = ft_atoi_hex(&arr[c->j][c->k + 1]);
	return (1);
}

void	ft_free_char_set(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
}

int	ft_make_map(char **arr, t_node **node, t_node **ref)
{
	t_calc_map	c;

	ft_init_calc_map(&c, ref);
	while (arr[c.i])
		c.i++;
	c.num_arr = (int **)malloc(sizeof(int *) * (c.i + 1));
	while (c.i > c.j)
	{
		ft_add_max_dent(&c, ref, arr);
		while(arr[c.j][c.k])
		{
			if (arr[c.j][c.k] == ',' && ft_color_input(&c, arr))
				break;
			c.k++;
		}
		if (!arr[c.j][c.k])
			c.num_arr[c.j][1] = 0x0;
		c.j++;
	}
	c.num_arr[c.j] = NULL;
	ft_new_list_add_back(c.num_arr, node);
	ft_free_char_set(arr);
	return (c.j);
}
