/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:13 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/22 20:56:18 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void print_error(char *name, t_pipex *pipex, int exit_code)
{
	if (ft_putstr_fd(name, 2) == -1)
	{
		perror("pipex: write error");
		free_struct(pipex);
		exit(EXIT_FAILURE);
	}
	if (exit_code == EXIT_CMD_NOT_FOUND)
	{
		if (ft_putstr_fd(name, 2) == -1)
		{
			perror("write error");
			free_struct(pipex);
			exit(EXIT_FAILURE);
		}
		if (ft_putendl_fd("command not found", 2)== -1)
		{
			perror("write error");
			free_struct(pipex);
			exit(EXIT_FAILURE);
		}
	}
	else
		perror(name);
	free_struct(pipex);
	exit(exit_code);
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
		exit(EXIT_FAILURE);
	}
	
	if (argc > 1 && ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (ft_putendl_fd(EX_HEREDOC_ARGS, 2) == -1)
		{
			perror("pipex:write error");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (ft_putendl_fd(EX_ARGS, 2) == -1)
		{
			perror("pipex:write error");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_FAILURE);
}

void free_struct_badcmd(t_pipex *pipex)
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

void print_error_badcmd(char *name, t_pipex *pipex, int exit_code)
{
	if (ft_putstr_fd("pipex: ", 2) == -1)
	{
		perror("pipex: write error");
		exit(EXIT_FAILURE);
	}
	if (exit_code == EXIT_CMD_NOT_FOUND)
	{
		if (ft_putstr_fd("command not found: ", 2)== -1)
		{
			perror("write error");
			free_struct_badcmd(pipex);
			exit(EXIT_CMD_NOT_FOUND);
		}
		if (ft_putendl_fd(name, 2) == -1)
		{  
			perror("write error");
			free_struct_badcmd(pipex);
			exit(EXIT_CMD_NOT_FOUND);
		}
	}
	else
		perror(name);
	free_struct_badcmd(pipex);
	exit(exit_code);
}
