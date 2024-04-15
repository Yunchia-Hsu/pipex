/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 20:47:42 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/15 17:30:08 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void free_arr(char **arr)
{
	int i;

	i = 0;
	if (arr == NULL)
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void free_int_arr(int **arr)
{
	int i;

	i = 0;
	if (arr == NULL)
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void free_struct(t_pipex *pipex)
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
	free(pipex);
	pipex = NULL;	
}




void print_error(char *name, t_pipex pipex, int err_cmd)
{
	if (ft_putstr_fd(name, 2) == -1)
	{
		perror("write error");
		free_struct(&pipex);
		exit(1);
	}
	if (err_cmd == EXIT_CMD_NOT_FOUND)
	{
		if (ft_putstr_fd(name, 2) == -1)
		{
			perror("write error");
			free_struct(&pipex);
			exit(1);
		}
		if (ft_putendl_fd(": command not found", 2)== -1)
		{
			perror("write error");
			free_struct(&pipex);
			exit(1);
		}
	}
	else
		perror(name);
	free_struct(&pipex);
	exit(1);
}
void close_fds(t_pipex *pipex)
{
	int i;

	i = 0;
	if (pipex->infile)
		close(pipex->infile);
	if (pipex->outfile)
		close(pipex->outfile);
	while (i < pipex->pipe_nbr)
	{
		if (close(pipex->pipe_fd[i][0]) == -1)
			print_error("close error", *pipex, EXIT_FAILURE);
		if (close(pipex->pipe_fd[i][1]) == -1)
			print_error("close error", *pipex, EXIT_FAILURE);
		i++;
	}
}




void only_print_error(char *name)
{
	if (ft_putstr_fd("pipex: ", 2) == -1)
	{
		perror("pipex: write error");
		exit(1);
	}
	perror(name);
	exit(1);
}

void error_exit(int argc, char **argv)
{
	if (ft_putendl_fd(ERR_INPUT, 2) == -1)
	{
		perror("pipex:write error");
		exit(1);
	}
	
	if (argc > 1 && ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (ft_putendl_fd(EX_HEREDOC_ARGS, 2) == -1)
		{
			perror("pipex:write error");
			exit(1);
		}
	}
	else
	{
		if (ft_putendl_fd(EX_ARGS, 2) == -1)
		{
			perror("pipex:write error");
			exit(1);
		}
	}
	exit(1);
}