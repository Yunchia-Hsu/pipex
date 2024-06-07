/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:45:06 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/26 09:45:07 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char *str, int i, char *charset)
{
	int	j;
	int	k;
	int	flags;

	j = -1;
	while (charset[++j])
	{
		if (str[i] == charset[j])
		{
			k = -1;
			flags = 0;
			while (str[++k] && k <= i)
			{
				if (str[k] == '\\')
					flags ^= (1 << 2);
				if (str[k] == '"')
					flags ^= (1 << 1);
				if (str[k] == '\'')
					flags ^= 1;
			}
			return (flags == 0);
		}
	}
	return (str[i] == '\0');
}

static int	count_word(char *str, char *charset)
{
	int	word;
	int	i;

	word = 0;
	i = 0;
	while (str[i])
	{
		if (!is_sep(str, i, charset)
			&& is_sep(str, i + 1, charset))
			word++;
		i++;
	}
	return (word);
}

static char	*write_word(char *dest, char *str, char *charset)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (is_sep(str, i, charset))
			i++;
		while (!is_sep(str, i, charset))
		{
			if (str[i] == '\\')
				i++;
			dest[j++] = str[i++];
		}
		break ;
	}
	dest[j] = '\0';
	return (dest);
}

static char	**write_arr(char **arr, char *str, char *charset)
{
	int	len;
	int	i;
	int	word;

	len = 0;
	i = 0;
	word = 0;
	while (str[i])
	{
		if (is_sep(str, i, charset))
			i++;
		else
		{
			while (!is_sep(str, i + len, charset))
				len++;
			arr[word] = malloc(sizeof(char) * (len + 1));
			if (arr[word] == NULL)
				return (NULL);
			arr[word] = write_word(arr[word], str + i, charset);
			i += len;
			len = 0;
			word++;
		}
	}
	return (arr);
}

/* 
	A customized ft_split() function, which splits the string with valid 
	separators. A separator is deemed as valid if it's outside any pair 
	of quotes.
*/

char	**ft_split_pipex(char *str, char *charset)
{
	char	**arr;
	int		word;

	word = count_word(str, charset);
	arr = (char **)malloc((word + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	arr = write_arr(arr, str, charset);
	arr[word] = NULL;
	return (arr);
}
