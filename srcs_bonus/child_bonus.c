/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:27:18 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/22 19:37:14 by yhsu             ###   ########.fr       */
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
}

char *verify_path(char *cmd, t_pipex *pipex)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) != 0)
			print_error_badcmd(cmd, pipex, EXIT_CMD_NOT_FOUND);
		if (access(cmd, X_OK) != 0)
			print_error(cmd, pipex, EXIT_CMD_PERMMIT_ERR);
		return(ft_strdup(cmd));
	}
	return (NULL);
}

char *get_cmd_path( t_pipex *pipex, char **cmd_arr)
{
	int i;
	char *str;
	char *command_path;

	command_path = verify_path(pipex->cmd_arr[0], pipex);
	if (command_path != NULL)
		return (command_path);
	i = -1;
	while (pipex->envp_paths != NULL && pipex->envp_paths[++i] != NULL)
	{
		str = ft_strjoin( pipex->envp_paths[i] , "/"); //handle error
		command_path = ft_strjoin(str, *cmd_arr);
		if (str)
			free(str);
		if (access(command_path, F_OK) == 0)
			break;	
		if (command_path)
			free(command_path);
		command_path = NULL;
	}
	return (command_path);
}

void	sub_dup2(int input, int output, t_pipex *pipex)
{
	if (dup2(input, 0) == -1)
		print_error("dup2 error", pipex, EXIT_FAILURE);
	if (dup2(output, 1) == -1)
		print_error("dup2 error", pipex, EXIT_FAILURE);
}

char *no_quote(char *cmd, t_pipex *pipex) //'{print}'
{
	char *result;
	int new_len;
	int i;

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
	return(result);
}  

char **get_cmd_arr(char *argv, t_pipex *pipex) //awk '{print}' -> cmd_arr0: awk / cmd_arr1: {print}
{
		char **cmd_arr;
		int i;

		cmd_arr = ft_split_pipex(argv, " ");
		if (cmd_arr == NULL)
			print_error("maloc error", pipex, EXIT_FAILURE);
		i = 0;
		while(cmd_arr[i] != NULL)
		{
			if (ft_strlen(cmd_arr[i]) > 1)
			{
				if ((cmd_arr[i][0] == '\'' && cmd_arr[i][ft_strlen(cmd_arr[i]) - 1] == '\'') ||  //字符常量中包含一個單引號本身，就必須使用轉義序列 \'
					(cmd_arr[i][0] == '"' && cmd_arr[i][ft_strlen(cmd_arr[i]) - 1 ] == '"')  )
					cmd_arr[i] = no_quote(cmd_arr[i], pipex);
			}
			i++;
		}
	return (cmd_arr);	
}

void get_infile(char **argv, t_pipex *pipex)
{
	if (pipex->here_doc == 1)//init infile
			get_input(pipex, argv);
		else
		{
			pipex->infile = open(argv[1], O_RDONLY);
			if (pipex->infile < 0)
				only_print_error("infile opened failed");
		}
}


void child(int i, char **argv, char **envp, t_pipex *pipex)
{
	if (i == 0)//1 cmd
	{
		get_infile(argv, pipex);
		sub_dup2(pipex->infile, pipex->pipe_fd[0][1], pipex);
		pipex->cmd_arr = get_cmd_arr(argv[2 + pipex->here_doc], pipex);
	}
	else if (i == (pipex->cmd_nbr - 1))//last cmd 3
	{
		get_outfile(argv, pipex);// init outfile  check if need to free sth if fails
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->outfile, pipex);
		pipex->cmd_arr = get_cmd_arr(argv[i + 2 + pipex->here_doc], pipex);
	}
	else //middle cmds
	{
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->pipe_fd[i][1], pipex);
		pipex->cmd_arr = get_cmd_arr(argv[i + 2 + pipex->here_doc], pipex);
	}
	if (pipex->cmd_arr == NULL)
		print_error("error malloc", pipex, EXIT_FAILURE);
	close_fds(pipex);
	pipex->cmd_path = get_cmd_path( pipex, &pipex->cmd_arr[0]);// may need go back to free command_path
	//dprintf(2, "pipex->cmd_path: %s\n", pipex->cmd_path);
	if (pipex->cmd_path != NULL)
		execve(pipex->cmd_path,pipex->cmd_arr,envp);
	print_error_badcmd(argv[i + 2 + pipex->here_doc], pipex, EXIT_CMD_NOT_FOUND);//if execve fails this line will run
}
