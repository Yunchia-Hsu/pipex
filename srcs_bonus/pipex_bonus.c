/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:39:43 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/22 19:52:21 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void create_pipe(t_pipex *pipex)
{
	int i;

	i = 0;
	while (i < pipex->cmd_nbr - 1)
	{
		pipex->pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		
		if (pipex->pipe_fd[i] == NULL)
			print_error("malloc error", pipex, EXIT_FAILURE);
		i++;
	}
	pipex->pipe_fd[i] = NULL;
	i = -1;
	while (++i < pipex->cmd_nbr - 1)
	{
		if (pipe(pipex->pipe_fd[i]) == -1)
		{
			close_fds(pipex);
			print_error("pipe error", pipex, EXIT_FAILURE);
		}
	}
}

int ft_pipex(char **argv, char **envp, t_pipex *pipex)
{
	int status;
	int i;
	
	create_pipe(pipex);
	i = 0;
	while(i < pipex->cmd_nbr)
	{
		pipex->pid[i] = fork();
		if (pipex->pid[i] == 0)
		{
			child(i, argv, envp, pipex);
		}
		if (pipex->pid[i] == -1)
			print_error("error fork", pipex, EXIT_FAILURE);
		i++;
	}
	close_fds(pipex);//clsoe fds for parent process
	i = -1;
	while (++i < pipex->cmd_nbr)
	{
		if (waitpid(pipex->pid[i], &status, 0) == -1)
			print_error("waitpid error", pipex, EXIT_FAILURE);
	}
	return (status);
}

/*
	./pipex infile cmd0 cmd1 cmd2 ... cmdn outfile
	should behave like:
	< infile cmd0 | cmd1 | cmd2 ... | cmdn > outfile

	./pipex here_doc LIMITER cmd cmd1 file
	should behave like:
	cmd0 << LIMITER | cmd1 >> file
*/


int main(int argc, char **argv, char **envp)
{
	t_pipex pipex;
	int status;
	

	if (argc < 5)
		error_exit(argc, argv);
	
	if (ft_strncmp(argv[1], "here_doc",9) == 0)//ft_check_args()
	{
		
		if (argc < 6)
			error_exit(argc, argv);
		else
			pipex.here_doc = 1;
		
	}
	else
		pipex.here_doc = 0;
	init_pipex_data(argc, envp, &pipex);
	status = ft_pipex(argv, envp, &pipex);
	parent_free(&pipex);// free  infile, outfile,  unlink ,  cmd_path[i] , cmd psth , pipe
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
    return 0;
}
