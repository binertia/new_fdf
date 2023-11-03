/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 14:17:02 by ksongchu          #+#    #+#             */
/*   Updated: 2023/04/10 18:34:28 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
static size_t	num_genr(char const *s, char c, int indc, int line)
{
	size_t	count;
	size_t	i;
	size_t	target;

	target = 0;
	count = 0;
	i = -1;
	while (s[++i])
	{
		if (i != 0 && s[i - 1] != c && s[i] == c)
			count++;
		else if (s[i] != c && s[i + 1] == 0)
			count++;
		if ((int)count == line && indc == 0)
			return (target);
		else if (indc == 1 && s[i] != c && s[i + 1] == 0)
			return (i + 1);
		else if ((int)count == line && indc == 1)
			return (i);
		if (s[i] == c)
			target = i + 1;
	}
	return (count);
}

static char	*cup_noodle_malloc(const char *src, char c, size_t line)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*dst;

	i = 0;
	start = num_genr(src, c, 0, line);
	end = num_genr(src, c, 1, line);
	dst = (char *)malloc((end - start) + 1);
	if (!dst)
		return (0);
	while (start != end)
		dst[i++] = *(src + (start++));
	dst[i] = 0;
	return (dst);
}

static int	free_time(char **s, char const *root, char c, size_t line )
{
	size_t	end_line;

	end_line = num_genr(root, c, -1, -1);
	while (line <= end_line && line++ != 0)
		free(s[line - 2]);
	free(s);
	return (1);
}

char	**ft_split(char const *s, char c)
{
	size_t	line;
	char	**ptr_result;

	if (!s)
		return (0);
	line = num_genr(s, c, -1, -1);
	ptr_result = malloc(sizeof(char *) * (line + 1));
	if (!ptr_result)
		return (0);
	if (!line)
	{
		ptr_result[0] = 0;
		return (ptr_result);
	}
	ptr_result[line] = 0;
	while (line-- != 0)
	{
		ptr_result[line] = cup_noodle_malloc(s, c, (line + 1));
		if (!ptr_result[line] && free_time(ptr_result, s, c, line))
			return (0);
	}
	return (ptr_result);
}
// int main() {
//   char *s = "asdf jkl   ";
//   char  *j = cup_noodle_malloc(s,' ',1);
//   printf("--%s--\n", j);
//
//   char **real = ft_split(s, ' ');
//   printf("-%s-", real[0]);
//   printf("%p\n", real[1]);
//   printf("%p\n", real[2]);
//   free_time(real, s, ' ', 1);
//   printf("%lu", num_genr(s, ' ', -1, -1));
//   free(j);
// }
