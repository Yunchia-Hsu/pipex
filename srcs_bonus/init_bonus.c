/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:25:15 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/22 21:49:40 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void init_envp(char **envp, t_pipex *pipex, char **argv)
//void init_envp(char **envp, t_pipex *pipex)
{
	char **arr;
	int i;
	
	i = 0;
	if (envp == NULL || envp[i] == NULL)
	{	
		arr = ft_split_pipex("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ":");
		if (arr == NULL)
			print_error("error malloc", pipex, EXIT_FAILURE);
		pipex->envp_paths = arr;
	}
	else
	{
		while(envp[i] && (ft_strncmp(envp[i],"PATH=", 5) != 0))
			i++;
		if (!envp[i])
			print_error_badcmd(argv[2 + pipex->here_doc], pipex, EXIT_CMD_NOT_FOUND);
		arr = ft_split(envp[i], ':');
		if (arr == NULL)
			print_error("error malloc", pipex, EXIT_FAILURE);
		pipex->envp_paths = arr;
	}
}

void	get_outfile(char **argv,t_pipex  *pipex)
{
	if (pipex->here_doc == 1)
	{
		//文件不存在則創建它，並以只寫方式打開，並將新的數據追加到文件的末尾。
		pipex->outfile = open(argv[pipex->cmd_nbr + 3], O_CREAT| O_WRONLY | O_APPEND, 0644);
		if (pipex->here_doc == -1)
		{
			close_fds(pipex);
			print_error("close error", pipex, EXIT_FAILURE);
		}
	}
	else
	{
		//文件不存在則創建它，並以只寫方式打開，如果文件存在則清空文件的內容
		pipex->outfile = open(argv[pipex->cmd_nbr + 2], O_CREAT| O_WRONLY | O_TRUNC, 0644);
		if (pipex->outfile == -1)
		{
			close_fds(pipex);
			print_error("close error", pipex, EXIT_FAILURE);
		}
	}
}

void get_input(t_pipex  *pipex, char **argv)
{
	int heredoc_fd;

	heredoc_fd = open(".here_doc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (heredoc_fd == -1)
	{
		close_fds(pipex);
		print_error(".here_doc", pipex, EXIT_FAILURE);
	}
	input_f_stdin(pipex, argv,heredoc_fd);//read words from stdinput and save it in .here_doc
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

void input_f_stdin(t_pipex  *pipex, char **argv, int heredoc_fd)//read words from stdinput and save it in .here_doc
{
	char *line;
	char *limiter;

	line = get_next_line(STDIN_FILENO);// protect line and limiter
	limiter = ft_strjoin(argv[2], "\n");
	if (!limiter) 
        print_error("Memory allocation failed", pipex, EXIT_FAILURE);
	while(line && ft_strncmp(line, limiter, ft_strlen(limiter)) != 0)
	{
		if (ft_putstr_fd(line, heredoc_fd) == -1)
		{
			free(line);
			line = NULL;
			free(limiter);
			limiter = NULL;
			if (close(heredoc_fd) == -1)
				print_error(".here_doc", pipex, EXIT_FAILURE);
			print_error("write error", pipex, EXIT_FAILURE);
		}
		free(line);
		//free(limiter);
		line = get_next_line(STDIN_FILENO);
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
	if (limiter)
	{	
		free(limiter);
		limiter = NULL;	
	}
}

void	init_pipex_data(int argc, char **argv,char **envp, t_pipex *pipex)
{
	
	pipex->cmd_nbr = argc - 3 - pipex->here_doc;
	pipex->cmd_arr = NULL;
	pipex->cmd_path = NULL;
	init_envp(envp, pipex, argv);
	pipex->pipe_fd = (int **)malloc(sizeof(int *) * (pipex->cmd_nbr));//add one more for null
	if (!pipex->pipe_fd)
		print_error("malloc error", pipex, EXIT_FAILURE);
	pipex->pid = (int *)malloc (sizeof(int) * (pipex->cmd_nbr));
	if (pipex->pid == NULL)
		print_error("malloc error", pipex, EXIT_FAILURE);
	
	/*if (pipex->here_doc == 1)//init infile
	{
		get_input(pipex, argv);
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
		{
			only_print_error("infile opened failed");
		}
	}
	get_outfile(argv, pipex);// init outfile  check if need to free sth if fails */
}

