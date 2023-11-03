/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 21:11:50 by ksongchu          #+#    #+#             */
/*   Updated: 2023/06/18 01:16:30 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	is_newline(int byte_read, char *str)
{
	int	i;

	i = 0;
	while (i < byte_read && str[i] != '\n')
		i++;
	if (i != byte_read)
		return (1);
	return (0);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)malloc(count * size);
	if (!ptr)
		return (NULL);
	while (i != count)
		ptr[i++] = 0;
	return ((void *)ptr);
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p_mal;
	char	*p_return;
	char	*tmp;

	p_mal = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!p_mal)
		return (NULL);
	p_return = p_mal;
	tmp = (char *)s1;
	while (*tmp)
		*p_mal++ = *tmp++;
	tmp = (char *)s2;
	while (*tmp)
		*p_mal++ = *tmp++;
	*p_mal = 0;
	return (p_return);
}
