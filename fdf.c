#include "includes/fdf.h"
#include "includes/mlx.h"
#include <math.h>
#include <sys/fcntl.h>

// setup
typedef struct s_img
{
	// default
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	// custom
	int		row;
	int		col;
	int		box_size;
	int		init_x;
	int		init_y;
}	t_img;

// first part == input
typedef struct s_var
{
	void	*win;
	void	*mlx;
}	t_var;

typedef struct s_node
{
	int **num_arr;
	
	int	max;
	int	min;
	int	max_col;
	int	min_col;
	struct s_node	*next;
}	t_node;

int	check_valid_hex(char *s)
{
	int	i;

	i = 0;
	if (*s == '\n')
		return (0);
	while (s[i] && ((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f')
		|| (s[i] >= 'A' && s[i] <= 'F') || s[i] == '\n'))
		i++;
	if (!s[i] || s[i] == ' ')
		return (i);
	return (0);
}

int	check_valid_input(char *s)
{
	while (*s)
	{
		if (*s == ' ' || (*s >= '0' && *s <= '9') || *s == '\n')
			s++;
		else if (*s == '-' && s[1] && (s[1] >= '0' && s[1] <= '9'))
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
	return (1);
}

void	*ft_new_list(int **content)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	new->num_arr = content;
	new->next = NULL;
	return (new);
}


void	ft_new_list_add_back(int **content, t_node **node)
{
	t_node	*temp;

	if (!content)
		return ;
	printf("finish add\n");
	if (*node == NULL)
	{
		*node = ft_new_list(content);
		return ;
	}
	temp = *node;
	while (temp && temp -> next)
		temp = temp -> next;
	temp->next = ft_new_list(content);
}

int	valid_color(char *s)
{
	if (*s && s[1] && *s == '0' && s[1] == 'x')
		s += 2;
	else
		return (0);
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
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

int	ft_make_map(char **arr, t_node **node, t_node **ref)
{
	int	i;
	int	j;
	int	k;
	int	max;
	int	**num_arr;

	printf("ft_make_map\n");
	i = 0;
	j = 0;
	(*ref)->min = 322;
	(*ref)->max = -322;
	while (arr[i])
		i++;
	num_arr = (int **)malloc(sizeof(int *) * (i + 1));
	while (i > j)
	{
		// if (arr[i][0] == 0)
		// {
		// 	printf("hello bith\n");
		// 	return (0);
		// }
		num_arr[j] = (int *)malloc(sizeof(int) * 2);
		num_arr[j][0] = ft_atoi(arr[j]);
		if (num_arr[j][0] - j> (*ref)->max)
		{
			(*ref)->max = num_arr[j][0] - j;
			(*ref)->max_col = j;
		}
		if (num_arr[j][0] < (*ref)->min)
		{
			(*ref)->min = num_arr[j][0] - j;
			(*ref)->min_col = j;
		}
		k = 0;
		while(arr[j][k])
		{
			if (arr[j][k] == ',')
			{
				if (!valid_color(&arr[j][k + 1]))
					num_arr[j][1] = 0xFF0000;
				else
					num_arr[j][1] = ft_atoi_hex(&arr[j][k + 1]);
				printf("%i\n", num_arr[j][1]);
				break;
			}
			k++;
		}
		if (!arr[j][k])
			num_arr[j][1] = 0x0;
		printf("==%i==\n", num_arr[j][0]);
		j++;
		printf("make map\n");
	}
	printf("end map\n");
	num_arr[j] = NULL;
	printf("j == %i\n", j);
	ft_new_list_add_back(num_arr, node);

	//:HACK:free logic
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
	//
	return (j);
}

void	add_init(t_img **img, int row, int col)
{
	(*img)->row = row;
	(*img)->col = col;
}

int	get_input(char *s, t_node **head, t_img *img, int fd, t_node *ref)
{
	int		checker;
	int		num;
	int		count;
	char	*temp;

	if (fd == -1)
		return (0);
	checker = 0;
	count = 0;
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp || *temp == '\n' || !*temp)
			break;
		if (!check_valid_input(temp))
		{
			printf("input_not_valid");
			return (0); // need to free :TODO:
		}	// 
		num = ft_make_map(ft_split(temp, ' '), head, &ref);
		free(temp);
		count++;
	}
	close(fd);
	add_init(&img, num, count);
	printf("hello\n");
	return (1);
}

// calc init_point :BUG:

void	calc_init_draw(t_img *img, t_node *ref)
{
	int	box_size;
	int	temp;
	int	range_max;
	int	range_min;

	range_max = (((ref->max + ref->max_col) ) - ref->max_col) ; //remove /3
	range_min = ((abs)(ref->min + ref->min_col) ) - (img->col - ref->min_col); // remove /3
	if (range_max < 0)
		range_max = 0;
	if (range_min < 0)
		range_min = 0;
	box_size = 1;
	while (1000 > (img->col + img->row + range_max + range_min - 2 ) * box_size && box_size < 13)
		box_size++;
	if (box_size > 1)
		box_size--;
	range_max = (range_min - range_max) * box_size;
	img->box_size = box_size;
	img->init_x = 1280 + (int)((img->col * img->box_size * sqrt(3) - (img->row * img->box_size * sqrt(3))) / 2);
	img->init_y = 895 - (((img->col + img->row - 2) * box_size + range_max) / 2);
	printf("x = %i\n" ,img->init_x);
	printf("y = %i\n" ,img->init_y);
	printf("row = %i\n" ,img->row);
	printf("col = %i\n" ,img->col);
	printf("box_size = %i\n" ,img->box_size);
}

// // for putr
 void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

// for drawing
typedef	struct s_arg
{
	int	row;
	int	col;
}	t_arg;

typedef	struct	s_data
{
	int	row;
	int	col;
	int	start_x;
	int	start_y;
	int	start_color;
	int	end_x;
	int	end_y;
	int	end_color;
}				t_data;

t_data	calc_pos(t_arg curr, t_img *img, int *prev_dent, int *dent) //:TODO:
{
	t_data	res;
	double	diff_x;
	int	diff_y;

	if (prev_dent[1] != 0)
		res.start_color = prev_dent[1];
	else if (prev_dent[0] >= 0)
	{
		res.start_color = 0xFFFF + (prev_dent[0] * 0x220000);
		if (prev_dent[0] >= 8)
			res.start_color = 0xFFFFFF;
	}
	else
	{
		res.start_color = 0xFFFF - (abs(prev_dent[0]) * 0x2200);
		if (prev_dent[0] <= -8)
			res.start_color = 0xFF;
	}
	if (dent[1] != 0)
		res.end_color = dent[1];
	else if (dent[0] >= 0)
	{
		if (dent[0] >= 8)
			res.end_color = 0xFFFFFF;
		else
			res.end_color = 0xFFFF + (dent[0] * 0x220000);
	}
	else
	{
		if (dent[0] <= -8)
			res.end_color = 0xFF;
		else
			res.end_color = 0xFFFF - (abs(dent[0]) * 0x2200);
	}
	printf("\nrow == %i \ncol == %i",curr.row, curr.col);
	diff_x = img->box_size * sqrt(3);
	diff_y = img->box_size;
	res.row = curr.row;
	res.col = curr.col;
	res.end_x = img->init_x + (curr.row * diff_x) - (curr.col * diff_x);
	res.start_x = img->init_x + (curr.row - 1) * diff_x - curr.col * diff_x;
	res.end_y = img->init_y + (curr.row + curr.col - (double)dent[0] ) * diff_y;// remove /3 form dent[0]
	res.start_y = img->init_y + ((curr.row - 1) + curr.col - (double)prev_dent[0] ) * diff_y; // same at top comment;
	printf("\nstart_x == %i \n start_y == %i\n",res.start_x, res.start_y );
	printf("end_x == %i \n end_y == %i\n",res.end_x, res.end_y );
	// if (res.end_y >= 1020 || res.end_y < 0) //:TODO: Error handler
	// 	return ((t_data){0});
	return (res);
}

#define FIXED_SCALE 16

typedef	struct	s_calc
{
	int	dif_x;
	int	dif_y;
	int	steps;
	int	x_inc;
	int	y_inc;
}				t_calc;

void	draw_point_to_point(t_img **img, t_data curr)
{
	int		i;
	t_calc	c;
	int		x;
	int		y;
	int		color;

	// if (curr.start_y < 0 || curr.end_y < 0)
	// 	exit(2);
	c.dif_x = (curr.end_x - curr.start_x) << FIXED_SCALE;
	c.dif_y = (curr.end_y - curr.start_y) << FIXED_SCALE;
    if (abs(c.dif_x) > abs(c.dif_y))
        c.steps = abs(c.dif_x >> FIXED_SCALE);
    else
        c.steps = abs(c.dif_y >> FIXED_SCALE);
    c.x_inc = c.dif_x / c.steps;
    c.y_inc = c.dif_y / c.steps;
    x = curr.start_x << FIXED_SCALE;
    y = curr.start_y << FIXED_SCALE;
	if ((y >> FIXED_SCALE) > 0 && (y >> FIXED_SCALE) < 1390 && (x >> FIXED_SCALE) > 0 && (x >> FIXED_SCALE) < 2560)
		my_mlx_pixel_put(*img,  x >> FIXED_SCALE, y >> FIXED_SCALE, curr.start_color);
	i = 0;
	while(i < c.steps)
	{
		if (curr.start_color < curr.end_color)
		{
			if (curr.start_color < 0xFFFF && curr.start_color >= 0xFF)
				curr.start_color += 0x100;
			else if (curr.start_color <= 0xFEFFFF && curr.start_color >= 0xFFFF)
				curr.start_color += 0x10000 ;
			// if (curr.start_color > (0xFFFF - 0x100 * color) && curr.start_color < 0xFFFF)
			// 	curr.start_color = 0xFFFF;
		}
		else if (curr.start_color > curr.end_color)
		{
			if (curr.start_color <= 0xFFFF && curr.start_color >= 0x1FF)
				curr.start_color -= 0x100;
			else if (curr.start_color <= 0xFFFFFF  && curr.start_color >= 0x1FFFF)
				curr.start_color -= 0x10000;
		}
        x += c.x_inc;
        y += c.y_inc;
		if ((y >> FIXED_SCALE) > 0 && (y >> FIXED_SCALE) < 1390 && (x >> FIXED_SCALE) > 0 && (x >> FIXED_SCALE) < 2560)
			my_mlx_pixel_put(*img, x >> FIXED_SCALE, y >> FIXED_SCALE, curr.start_color);
		i++;
    }
}

t_data	calc_pos_col(t_arg curr, t_img *img, int *prev_dent, int *dent)
{
	t_data	res;
	double	diff_x;
	int	diff_y;


	if (dent[1] != 0)
		res.start_color = dent[1];
	else if (dent[0] >= 0)
	{
		res.start_color = 0xFFFF + (dent[0] * 0x220000);
		if (dent[0] >= 8)
			res.start_color = 0xFFFFFF;
	}
	else
	{
		res.start_color = 0xFFFF - (abs(dent[0]) * 0x2200);
		if (dent[0] <= -8)
			res.start_color = 0xFF;
	}
	if (prev_dent[1] != 0)
		res.end_color = prev_dent[1];
	else if (prev_dent[0] >= 0)
	{
		if (prev_dent[0] >= 8)
			res.end_color = 0xFFFFFF;
		else
			res.end_color = 0xFFFF + (prev_dent[0] * 0x220000);
	}
	else
	{
		if (prev_dent[0] <= -8)
			res.end_color = 0xFF;
		else
			res.end_color = 0xFFFF - (abs(prev_dent[0]) * 0x2200);
	}
//
		//
	printf("\nrow == %i \ncol == %i",curr.row, curr.col);
	diff_x = img->box_size * sqrt(3);
	diff_y = img->box_size;
	res.row = curr.row;
	res.col = curr.col;
	res.start_x = img->init_x + (curr.row) * diff_x - curr.col * diff_x;
	res.end_x = img->init_x + ((curr.row + 1) * diff_x) - (curr.col * diff_x);
	res.start_y = img->init_y + ((curr.row) + curr.col ) * diff_y - ((double)dent[0]) * diff_y;
	res.end_y = img->init_y + (curr.row + (curr.col - 1) - (double)prev_dent[0]) * diff_y;
	//res.start_color = dent[1];
	//res.end_color = prev_dent[1];
	printf("\nstart_x == %i \n start_y == %i\n",res.start_x, res.start_y );
	printf("end_x == %i \n end_y == %i\n",res.end_x, res.end_y );
	//:TODO: need to handle exit when number is negative;
	return (res);
}

void	draw_from_prev_row(t_img **img, int **curr_set,int **prev_set ,int current_col)
{
	int	i = 0;
	while (prev_set[i] && curr_set[i])
	{
		draw_point_to_point(img, calc_pos_col((t_arg){i, current_col},*img, prev_set[i], curr_set[i]));
		i++;
	}
}

void	draw_map_to_img(t_node *head,t_img *img)
{
	int	i;
	int	j;
	int	*prev;
	int	**previous_set;

	previous_set = NULL;
	j = 0;
	while (head)
	{
		i = 0;
		while (head->num_arr[i])
		{
			printf("\ndrawto_x:\n");
			if (i != 0)
				draw_point_to_point(&img, calc_pos((t_arg){i, j}, img, prev, head->num_arr[i]));
			prev = head->num_arr[i];
			i++;
		}
		if (previous_set != NULL)
			draw_from_prev_row(&img, head->num_arr, previous_set, j);
		previous_set = head->num_arr;
		head = head -> next;
		j++;
	}
}
//  ======================================


// debug drawing :BUG:

void	draw_background(t_img *img, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i != y)
	{
		j = 0;
		while (j != x)
		{
			my_mlx_pixel_put(img, j, i, color);
			j++;
		}
		i++;
		if (i % 8 == 0)
		{
		if (color % 0x100 > 0x11)
			color--;
		if (color % 0x10000 > 0x1111)
			color -= 0x100;
		if (color <= 0xff0000 && color > 0x111111)
			color-= 0x10000;
		}
		// 0x49557a
	}

}

//================================================
//hook
int win_close(int keycode, t_var *vars)
{
	if (keycode == 53)
		mlx_destroy_window(vars->mlx, vars->win);
	return (0);
}

int	main(int ac, char *av[])
{
	t_img img;
	t_var var;
	t_node *map;
	t_node ref;

	map = NULL;
	if (ac != 2)
		return (0);
	//if (!get_input(av[1]))
	//	return (0);
	if (!get_input(av[1], &map, &img, open(av[1], O_RDONLY ), &ref))
		return (2);
	printf("pass_get_input");

	// === start mlx and img.img
	// default setup ::
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 2560, 1390, "fdf"); // max 1680, 1020
	img.img = mlx_new_image(var.mlx, 2560, 1390);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);

	// === calculate init x and init y;
	calc_init_draw(&img, &ref);
	// === draw background
	draw_background(&img, 2560, 1390, 0x7C81AD);
	// === drawing to img.img
	draw_map_to_img(map, &img);
	// === put drawing to win
	mlx_put_image_to_window(var.mlx, var.win, img.img, 0, 0);

	// === set hook :TODO:
	 mlx_hook(var.win, 2, 0, win_close, &var);
	// === set loop;
	mlx_loop(var.mlx);
	// usaged
	// teest :HACK:
	printf("\n%d", map->num_arr[0][0]);
	printf("\n______\n");
	t_node *temp = map;
	int	i;
	while (map && map->num_arr)
	{
		i = 0;
		while(map->num_arr[i])
		{
			printf("\n%d", map->num_arr[i][0]);
			i++;
		}
		map = map -> next;
		printf("\n3333333333");
	}

	//free :HACK:
	while (temp)
	{
		t_node *free_time = temp;
		temp = temp->next;
		int i = 0;
		while(free_time->num_arr[i])
		{
			free(free_time->num_arr[i]);
			i++;
		}
		free(free_time->num_arr[i]);
		free(free_time->num_arr);
		free(free_time);
		free_time = NULL;
	}
	printf("-end-");
	return (0);
}
