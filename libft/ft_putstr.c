/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:12:42 by yhsu              #+#    #+#             */
/*   Updated: 2024/05/03 16:41:52 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_putstr(const char *s)
{
	size_t	i;

	i = 0;
	if (! s)
	{
		if (write (1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	if (write (1, s, ft_strlen(s)) == -1)
		return (-1);
	return (i);
}
