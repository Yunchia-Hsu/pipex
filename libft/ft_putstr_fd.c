/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:43:04 by yhsu              #+#    #+#             */
/*   Updated: 2024/05/03 16:38:22 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (! s)
	{
		if (ft_putstr_fd("(null)", fd) == -1)
			return (-1);
		return (0);
	}
	if (write (fd, s, ft_strlen(s)) == -1)
		return (-1);
	return (0);
}
