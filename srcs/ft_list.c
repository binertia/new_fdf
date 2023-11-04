#include "../includes/fdf.h"

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
