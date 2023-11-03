/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksongchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 18:21:03 by ksongchu          #+#    #+#             */
/*   Updated: 2023/04/10 15:56:09 by ksongchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_space(const char *s)
{
	if ((*s >= '\t' && *s <= '\r') || *s == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	n_or_p;
	int	result;

	n_or_p = 1;
	result = 0;
	while (is_space(str))
		str++;
	if (*str == '-')
		n_or_p = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		result = (result * 10) + *str++ - '0';
	return (result * n_or_p);
}
// lose 5 hours because star check from 1 not 0
//int	is_flow(const char *str, int num, int n_or_p)
//{
//	if (num > 214748364 && (*str == '8' && *str == '9'))
//	{
//		if (n_or_p == -1)
//			return (-2147483648);
//		return (2147483647);
//	}
//	return (0);
//
//}
