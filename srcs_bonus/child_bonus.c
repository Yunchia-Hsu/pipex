/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:27:18 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/18 17:49:03 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void close_fds(t_pipex *pipex)
{
	int i;

	i = 0;
	if (pipex->infile > 0) 
		close(pipex->infile);
    if (pipex->outfile > 0) 
		close(pipex->outfile);
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
	//dprintf(2,"close fd7\n");
}

char *get_cmd_path( t_pipex *pipex, char **cmd_arr)
{
	int i;
	char *str;
	char *command_path;

	i = 0;
	while (*pipex->envp_paths)
	{
		str = ft_strjoin( pipex->envp_paths[i] , "/"); //handle error
		command_path = ft_strjoin(str, *cmd_arr);
		if (str != NULL)
			free(str);
		if (access(command_path, F_OK) == 0)
		{
			return (command_path);
		}
		if (command_path)	
			free(command_path);
		pipex->envp_paths++;
	}
	return (NULL);
}

void	sub_dup2(int input, int output, t_pipex *pipex)
{
	if (dup2(input, 0) == -1)
		print_error("dup2 error", pipex, EXIT_FAILURE);
	if (dup2(output, 1) == -1)
		print_error("dup2 error", pipex, EXIT_FAILURE);
}

void child(int i, char **argv, char **envp, t_pipex *pipex)
{
	if (i == 0)//1 cmd
	{
		sub_dup2(pipex->infile, pipex->pipe_fd[0][1], pipex);
		pipex->cmd_arr = ft_split(argv[2 + pipex->here_doc], ' ');
	}
	else if (i == (pipex->cmd_nbr - 1))//last cmd 3
	{
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->outfile, pipex);
		pipex->cmd_arr = ft_split(argv[i + 2 + pipex->here_doc], ' ');
	}
	else //middle cmd
	{
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->pipe_fd[i][1], pipex);
		pipex->cmd_arr = ft_split(argv[i + 2 + pipex->here_doc], ' ');
	}
	if (pipex->cmd_arr == NULL)
		print_error("error malloc", pipex, EXIT_FAILURE);
	close_fds(pipex);
	pipex->cmd_path = get_cmd_path( pipex, &pipex->cmd_arr[0]);// may need go back to free command_path
	execve(pipex->cmd_path,pipex->cmd_arr,envp);
	print_error(argv[i + 2 + pipex->here_doc], pipex, EXIT_CMD_NOT_FOUND);//if execve fails this line will run
} 


