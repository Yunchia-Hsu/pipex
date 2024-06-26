/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:25:15 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/26 11:59:18 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_envp(char **envp, t_pipex *pipex)
{
	char	**arr;

	if (*envp == NULL)
	{
		arr = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
		if (arr == NULL)
			print_error("error malloc", pipex, EXIT_FAILURE);
		pipex->envp_paths = arr;
	}
	else
	{
		pipex->envp_paths = get_env_paths(envp, pipex);
	}
}

void	get_outfile(char **argv, t_pipex *pipex)
{
	if (pipex->here_doc == 1)
	{
		pipex->outfile = open(argv[pipex->cmd_nbr + 3],
				O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (pipex->here_doc == -1)
		{
			close_fds(pipex);
			print_error_badcmd("outfile", pipex, EXIT_FAILURE);
		}
	}
	else
	{
		pipex->outfile = open(argv[pipex->cmd_nbr + 2],
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (pipex->outfile == -1)
		{
			close_fds(pipex);
			print_error_badcmd("outfile", pipex, EXIT_FAILURE);
		}
	}
}

void	get_input(t_pipex *pipex, char **argv)
{
	int	heredoc_fd;

	heredoc_fd = open(".here_doc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (heredoc_fd == -1)
	{
		close_fds(pipex);
		print_error(".here_doc", pipex, EXIT_FAILURE);
	}
	input_f_stdin(pipex, argv, heredoc_fd);
	if (close(heredoc_fd) == -1)
	{
		close_fds(pipex);
		print_error(".here_doc", pipex, EXIT_FAILURE);
	}
	pipex->infile = open(".here_doc", O_RDONLY);
	if (pipex->infile == -1)
	{
		close_fds(pipex);
		print_error(".here_doc", pipex, EXIT_FAILURE);
	}
}

void	input_f_stdin(t_pipex *pipex, char **argv, int heredoc_fd)
{
	char	*line;
	char	*limiter;

	line = get_next_line(STDIN_FILENO);
	limiter = ft_strjoin(argv[2], "\n");
	if (!limiter)
		print_error("Memory allocation failed", pipex, EXIT_FAILURE);
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter)) != 0)
	{
		if (ft_putstr_fd(line, heredoc_fd) == -1)
		{
			free(line);
			free(limiter);
			if (close(heredoc_fd) == -1)
				print_error(".here_doc", pipex, EXIT_FAILURE);
			print_error("write error", pipex, EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	line = NULL;
	free(limiter);
	limiter = NULL;
}

void	init_pipex_data(int argc, char **envp, t_pipex *pipex)
{
	pipex->cmd_nbr = argc - 3 - pipex->here_doc;
	pipex->cmd_arr = NULL;
	pipex->cmd_path = NULL;
	init_envp(envp, pipex);
	pipex->pipe_fd = (int **)malloc(sizeof(int *) * (pipex->cmd_nbr));
	if (!pipex->pipe_fd)
		print_error("malloc error", pipex, EXIT_FAILURE);
	pipex->pid = (int *)malloc (sizeof(int) * (pipex->cmd_nbr));
	if (pipex->pid == NULL)
		print_error("malloc error", pipex, EXIT_FAILURE);
}
