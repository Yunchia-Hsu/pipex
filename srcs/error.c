/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:13 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/19 14:56:52 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void print_error(char *name, t_pipex *pipex, int err_cmd)
{
	if (ft_putstr_fd(name, 2) == -1)
	{
		perror("write error");
		free_struct(pipex);
		exit(1);
	}
	if (err_cmd == EXIT_CMD_NOT_FOUND)
	{
		if (ft_putstr_fd(name, 2) == -1)
		{
			perror("write error");
			free_struct(pipex);
			exit(1);
		}
		if (ft_putendl_fd(": command not found", 2)== -1)
		{
			perror("write error");
			free_struct(pipex);
			exit(1);
		}
	}
	else
		perror(name);
	free_struct(pipex);
	exit(1);
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
