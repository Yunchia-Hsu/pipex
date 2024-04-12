/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:43:04 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/09 20:39:59 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (! s)
	{
		if (ft_putstr_fd(NULL, fd) == -1)
			return (-1);
		return (0);
	}	
	while (s[i])
	{
		if( write (fd, &s[i], 1) == -1);
			return (-1);
		i++;
	}

	return (0);
}
