/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:08:56 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/26 10:58:08 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	if (arr)
		free(arr);
	arr = NULL;
}

void	free_int_arr(int **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_struct(t_pipex *pipex)
{
	if (pipex)
	{
		if (pipex->envp_paths)
			free_arr(pipex->envp_paths);
		if (pipex->cmd_arr)
			free_arr(pipex->cmd_arr);
		if (pipex->cmd_path)
		{
			free(pipex->cmd_path);
			pipex->cmd_path = NULL;
		}
		if (pipex->pipe_fd)
			free_int_arr(pipex->pipe_fd);
		if (pipex->pid)
		{
			free(pipex->pid);
			pipex->pid = NULL;
		}
	}
	if (pipex)
		free(pipex);
	pipex = NULL;
}

void	parent_free(t_pipex *pipex)
{
	if (pipex->here_doc)
		if (unlink(".here_doc") == -1)
			print_error(ERR_UNLINK, pipex, EXIT_FAILURE);
	if (pipex->envp_paths)
		free_arr(pipex->envp_paths);
	if (pipex->pipe_fd)
		free_int_arr(pipex->pipe_fd);
	if (pipex->pid)
		free(pipex->pid);
	pipex->pid = NULL;
}

void	free_struct_badcmd(t_pipex *pipex)
{
	if (pipex)
	{
		if (pipex->envp_paths)
			free_arr(pipex->envp_paths);
		if (pipex->cmd_arr)
			free_arr(pipex->cmd_arr);
		if (pipex->cmd_path)
		{
			free(pipex->cmd_path);
			pipex->cmd_path = NULL;
		}
		if (pipex->pipe_fd)
			free_int_arr(pipex->pipe_fd);
		if (pipex->pid)
		{
			free(pipex->pid);
			pipex->pid = NULL;
		}
	}
	pipex = NULL;
}
