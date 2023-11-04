#include "../includes/fdf.h"

int	add_init(t_img **img, int row, int col, int fd)
{
	(*img)->row = row;
	(*img)->col = col;
	close(fd);
	return (1);
}

int	ft_exit_invalid_input(char** temp, t_node **head, int fd)
{
	write(2, "input not valid : clean and exit\n", 33);
	if (*head)
		ft_free_node(head);
	if (*temp)
		free(*temp);
	close(fd);
	exit(2);
}

t_node	*get_input(char *s, t_img *img, int fd, t_node *ref)
{
	t_node	*head;
	int		checker;
	int		num;
	int		count;
	char	*temp;

	head = NULL;
	if (fd == -1)
		return (0);
	checker = 0;
	count = 0;
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp || *temp == '\n' || !*temp)
			break;
		if (!check_valid_input(temp, 0))
			ft_exit_invalid_input(&temp, &head, fd);
		num = ft_make_map(ft_split(temp, ' '), &head, &ref);
		free(temp);
		count++;
	}
	free(temp);
	add_init(&img, num, count, fd);
	if (!head)
		ft_exit_invalid_input(&temp, &head, fd);
	return  (head);
}
