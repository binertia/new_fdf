#include "../includes/fdf.h"

void ft_free_node(t_node **map)
{
	t_node	*temp;

	temp = *map;
	while (temp)
	{
		t_node *free_time = temp;
		temp = temp->next;
		int i = 0;
		while (free_time->num_arr[i])
		{
			free(free_time->num_arr[i]);
			i++;
		}
		free(free_time->num_arr[i]);
		free(free_time->num_arr);
		free(free_time);
		free_time = NULL;
	}
}

int ft_is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int check_valid_hex(char *s)
{
	int	i;

	i = 0;
	if (*s == '\n')
		return (0);
	while (s[i] && (ft_is_number(s[i]) || (s[i] >= 'a' && s[i] <= 'f') ||
					(s[i] >= 'A' && s[i] <= 'F') || s[i] == '\n'))
		i++;
	if (!s[i] || s[i] == ' ')
		return (i);
	return (0);
}

int valid_color(char *s)
{
	if (*s && s[1] && *s == '0' && s[1] == 'x')
		s += 2;
	else
		return (0);
	while (*s)
	{
		if (ft_is_number(*s))
			s++;
		else if (*s >= 'a' && *s <= 'f')
			s++;
		else if (*s >= 'A' && *s <= 'F')
			s++;
		else
			return (0);
	}
	if (*s == 'x')
		return (0);
	return (1);
}

int check_valid_input(char *s, int i)
{
	while (*s)
	{
		if (ft_is_number(*s))
			i++;
		if (*s == ' ' || ft_is_number(*s) || *s == '\n')
			s++;
		else if (*s == '-' && s[1] && ft_is_number(s[1]))
			s++;
		else if (*s == ',' && s[1] && s[1] == '0' && s[2] && s[2] == 'x' && s[3])
		{
			if (check_valid_hex(s + 3))
				s = s + 3 + (check_valid_hex(s + 3));
			else
				return (0);
		}
		else if (*s == '0' && s[1] && s[1] == 'x' && s[2])
			s += 2;
		else
			break;
	}
	if (*s)
		return (0);
	return (i);
}
