/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:39:43 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/12 19:06:31 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
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
		
		if (pipex->envp_paths)
			free_arr(pipex->envp_paths);
		if (pipex->pipe_fd)
			free_arr(pipex->pipe_fd);
		if (pipex->pid)
		{
			free(pipex->pid);
			pipex->pid = NULL;
		}
	}	
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
		
		if (ft_putendl_fd(": command not found", 2 ) == -1)
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
	if (argc > 1 && argv[1] == "here_doc")
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



void init_envp(char **envp, t_pipex pipex)
{
	char **arr;
	int i;
	
	i = 0;
	if (envp == NULL)
	{	
		arr = ft_spit("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
		if (arr == NULL)
			partial_free();
		pipex.envp_paths = arr;
	}
	else
	{
		while(ft_strncmp(envp[i],"PATH=", 5) != 0)
			i++;
		arr = ft_split(envp[i], ':');
		if (arr == NULL)
			partial_free();
		pipex.envp_paths = arr;
	}	
}
char *input_f_stdin(t_pipex  pipex, char **argv, int heredoc_fd)//read words from stdinput and save it in .here_doc
{
	char *line;
	char *limiter;

	line = get_next_line(STDIN_FILENO); 
	limiter = ft_strjoin(argv[2], '\n');
	while(ft_strcmp(line, limiter) != 0)
	{
		if (ft_putstr_fd(line, heredoc_fd) == -1)
		{
			free(line);
			free(limiter);
			free_fds();
			print_error();
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(limiter);
}


void get_input(t_pipex  pipex, char **argv)
{
	int heredoc_fd;
	
	heredoc_fd = open(".here_doc", O_RDONLY | O_CREAT | O_TRUNC, 0777);
	if (heredoc_fd == -1)
	{
		closefds();
		print_error();
	}
	input_f_stdin(pipex, argv,heredoc_fd);//read words from stdinput and save it in .here_doc
	if (close(heredoc_fd) == -1)
	{
		closefds();
		print_error();
	}
	pipex.infile = open(".here_doc", O_RDONLY, 0777);
	if (pipex.infile == -1)
	{
		closefds();
		print_error();
	}	 
}

void	get_outfile(char **argv,t_pipex  pipex)
{
	if (pipex.here_doc == 1)
	{
		//文件不存在則創建它，並以只寫方式打開，並將新的數據追加到文件的末尾。
		pipex.outfile = open(argv[pipex.cmd_nbr + 3], O_CREAT| O_WRONLY | O_APPEND, 0777);
		if (pipex.here_doc == -1)
		{
			close_fds(pipex);
			print_error();
		}
	}
	else
	{
		//文件不存在則創建它，並以只寫方式打開，如果文件存在則清空文件的內容
		pipex.outfile = open(argv[pipex.cmd_nbr + 2], O_CREAT| O_WRONLY | O_TRUNC, 0777);
		if (pipex.outfile == -1)
		{
			close_fds(pipex);
			print_error();
		}
	}
}


void	init_pipex_data(int argc, char **argv,char **envp, t_pipex pipex)
{
	pipex.cmd_nbr = argc - 3 - pipex.here_doc;
	pipex.cmd_arr = NULL;
	pipex.cmd_path = NULL;
	int pipe_nbr = pipex.cmd_nbr - 1;
	init_envp(envp, pipex);
	pipex.pipe_fd = (int **)malloc(sizeof(int *) * (pipex.cmd_nbr - 1));
	if (!pipex.pipe_fd)
		free_struct(&pipex);//need to free more
	pipex.pid = (int *)malloc (sizeof(int) * pipex.cmd_nbr);
	if (!pipex.pid)
		free_struct(&pipex);//need to free more
	if (pipex.here_doc == 1)//init infile
		get_input(pipex, argv);
	else
	{
		if (pipex.infile = open(argv[1], O_RDONLY) < 0)
		{
			free_fds();
			print_error();
		}
	}
	get_outfile(argv, pipex);// init outfile  check if need to free sth if fails
}


create_pipe(t_pipex pipex)
{
	size_t i;

	i = 0;
	while (i < pipex.cmd_nbr - 1)
	{
		pipex.pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		
		if (pipex.pipe_fd[i] == NULL)
		{
			free_struct(&pipex);//pipex.pipe_fd[i] before 
			/*
			free_arr((*data)->env_paths);
			free(*data);
			*data = NULL;
			*/
		}
		i++;
	}
	i = -1;
	while (++i < pipex.cmd_nbr - 1)
	{
		if (pipe(pipex.pipe_fd[i]) == -1)
			//print_error();
	}
}

char *get_cmd_path(char **envp, t_pipex pipex)
{
	int i;
	char *str;
	char *command_path;

	i = 0;
	while (pipex.envp_paths[i])
	{
		str = ft_strjoin( pipex.envp_paths[i] , "/"); //handle error
		command_path = ft_strjoin(str, pipex.cmd_arr[0]);
		free(str);
		if (access(command_path, 0) == 0)
			return (command_path);
		free(command_path);
		i++;
	}
	return (NULL);
}

void	sub_dup2(int zero, int first)
{
	if (dup2(zero, 0) == -1)
		print_error();
	if (dup2(first, 1) == -1)
		print_error();
}

void close_fds(t_pipex pipex)
{
	int i;

	i = 0;
	if (pipex.infile)
		close(pipex.infile);
	if (pipex.outfile)
		close(pipex.outfile);
	while(i < pipex.pipe_nbr)
	{
		if (close(pipex.pipe_fd[i][0]) == -1)
			print_error();
		if (close(pipex.pipe_fd[i][1]) == -1)
			print_error();
		i++;
	}
}

void child(int i, char **argv, char **envp, t_pipex pipex)
{
	if (i == 0)//1 cmd
	{
		sub_dup2(pipex.infile, pipex.pipe_fd[0][1]);
		pipex.cmd_arr = ft_split(argv[2 + pipex.here_doc], ' ');
	}
	else if ((i == pipex.cmd_nbr - 1) )//last cmd
	{
		sub_dup2(pipex.pipe_fd[i][1], pipex.outfile);
		pipex.cmd_arr = ft_split(argv[pipex.cmd_nbr - 2], ' ');
	}
	else //middle cmd
	{
		sub_dup2(pipex.pipe_fd[i][1], pipex.pipe_fd[i][0]);
		pipex.cmd_arr = ft_split(argv[i + 2 + pipex.here_doc], ' ');
	}
	if (pipex.cmd_arr == NULL)
			partial_free();
	close_fds(pipex);
	pipex.cmd_path = get_cmd_path(envp, pipex);// may need go back to free command_path
	execve(pipex.cmd_path,pipex.cmd_arr,envp);
	print_error();//if execve fails this line will run
}

int pipex(char **argv, char **envp, t_pipex pipex)
{
	int status;
	int i;
	
	create_pipe(pipex);
	i = -1;
	while(++i < pipex.cmd_nbr)
	{
		pipex.pid[i] = fork();
		if ( pipex.pid[i] == 0)
			child(i, argv, envp, pipex);
		if (pipex.pid[i] == -1)
			print_error();
	}
	close_fds();//clsoe fds for parent process
	if (waitpid(-1, status, 0) == -1)// pid= -1, waitpid() waits for any child process to end.
		print_error();
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
	if (ft_strcmp(argv[1], "here_doc") == 0)//ft_check_args()
	{
		if (argc < 6)
			error_exit(argc, argv);
		else
			pipex.here_doc = 1;
	}
	else
		pipex.here_doc = 0;
	init_pipex_data(argc, argv, *envp, pipex);
	status = pipex(argv, envp, pipex);
 	if(WIFEXTED(status))
	{
		if (pipex.here_doc)
		{
			if (unlink(".here_doc") == -1)
				print_error("error link", pipex, EXIT_FAIL);
			free_struc(&pipex);
			exit(WIFEXTED(status));
		}
	}
    return 0;
}



/*使用 open 函式打開第一個命令行參數所指定的檔案作為輸入檔案，並將檔案描述符存儲在 pipex.infile 中。
使用 open 函式以指定的模式打開最後一個命令行參數所指定的檔案作為輸出檔案，並將檔案描述符存儲在 pipex.outfile 中。
使用 pipe 函式創建一個管道，並將讀取端和寫入端的檔案描述符分別存儲在 pipex.tube[0] 和 pipex.tube[1] 中。
使用 find_path 函式查找 PATH 變量的值，並將其存儲在 pipex.paths 中。
使用 ft_split 函式將 PATH 變量的值按照冒號分割成一個字符串陣列，並將其存儲在 pipex.cmd_paths 中。
使用 fork 函式創建第一個子進程，如果是子進程，則調用 first_child 函式進行相應的操作。
使用 fork 函式創建第二個子進程，如果是子進程，則調用 second_child 函式進行相應的操作。
調用 close_pipes 函式關閉管道的讀取端和寫入端。
使用 waitpid 函式等待第一個子進程和第二個子進程結束。
最後，調用 parent_free 函式釋放所有父進程的資源，並返回 0 表示程式執行成功。*/