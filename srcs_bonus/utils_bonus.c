/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 10:35:03 by yhsu              #+#    #+#             */
/*   Updated: 2024/05/03 16:54:59 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_fds(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_nbr - 1)
	{
		if (pipex->pipe_fd[i][0])
		{
			if (close(pipex->pipe_fd[i][0]) == -1)
				print_error("close error", pipex, EXIT_FAILURE);
		}
		if (pipex->pipe_fd[i][1])
		{
			if (close(pipex->pipe_fd[i][1]) == -1)
				print_error("close error", pipex, EXIT_FAILURE);
		}
		i++;
	}
}

char	*no_quote(char *cmd, t_pipex *pipex)
{
	char	*result;
	int		new_len;
	int		i;

	new_len = ft_strlen(cmd) - 1;
	result = malloc(sizeof(char) * new_len);
	if (result == NULL)
		print_error("maloc error", pipex, EXIT_FAILURE);
	i = 0;
	while (i < new_len - 1)
	{
		result[i] = cmd[i + 1];
		i++;
	}
	result[i] = '\0';
	free(cmd);
	return (result);
}

char	*verify_path(char *cmd, t_pipex *pipex)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) != 0)
			print_error_badcmd(cmd, pipex, EXIT_CMD_PERMMIT_ERR);
		if (access(cmd, F_OK) != 0)
			print_error_badcmd(cmd, pipex, EXIT_CMD_NOT_FOUND);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

char	**get_env_paths(char **envp, t_pipex *pipex)
{
	char	*path_line;
	char	**env_paths;
	int		i;

	path_line = NULL;
	while (*envp != NULL)
	{
		if (ft_strnstr(*envp, "PATH=", 5) != NULL)
		{
			path_line = *envp;
			break ;
		}
		envp++;
	}
	if (path_line == NULL)
		return (NULL);
	i = -1;
	while (++i < 5)
		path_line++;
	env_paths = ft_split(path_line, ':');
	if (env_paths == NULL)
		print_error_partial_free("malloc error", pipex);
	return (env_paths);
}
