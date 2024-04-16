/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:39:43 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/16 17:44:02 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "pipex_bonus.h"

void free_arr(char **arr)
{
	int i;

	i = 0;
	if (arr == NULL)
		return;
	while (arr[i] != NULL)
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
	dprintf(2,"close fd7\n");
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


void init_envp(char **envp, t_pipex *pipex)
{
	char **arr;
	int i;
	
	i = 0;
	if (envp == NULL)
	{	
		arr = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':');
		if (arr == NULL)
			print_error("error malloc", pipex, EXIT_FAILURE);
		pipex->envp_paths = arr;
	}
	else
	{
		while(ft_strncmp(envp[i],"PATH=", 5) != 0)
			i++;
		arr = ft_split(envp[i], ':');
		if (arr == NULL)
			print_error("error malloc", pipex, EXIT_FAILURE);
		pipex->envp_paths = arr;
	}
}

void input_f_stdin(t_pipex  *pipex, char **argv, int heredoc_fd)//read words from stdinput and save it in .here_doc
{
	char *line;
	char *limiter;

	line = get_next_line(STDIN_FILENO); 
	limiter = ft_strjoin(argv[2], "\n");
	while(ft_strncmp(line, limiter, ft_strlen(limiter)) != 0)
	{
		if (ft_putstr_fd(line, heredoc_fd) == -1)
		{
			free(line);
			free(limiter);
			close_fds(pipex);
			print_error(".here_doc", pipex, EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(limiter);
}


void get_input(t_pipex  *pipex, char **argv)
{
	int heredoc_fd;

	heredoc_fd = open(".here_doc", O_RDONLY | O_CREAT | O_TRUNC, 0777);
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
	pipex->infile = open(".here_doc", O_RDONLY, 0777);
	if (pipex->infile == -1)
	{
		close_fds(pipex);
		print_error(".here_doc", pipex, EXIT_FAILURE);
	}
}

void	get_outfile(char **argv,t_pipex  *pipex)
{
	if (pipex->here_doc == 1)
	{
		//文件不存在則創建它，並以只寫方式打開，並將新的數據追加到文件的末尾。
		pipex->outfile = open(argv[pipex->cmd_nbr + 3], O_CREAT| O_WRONLY | O_APPEND, 0777);
		if (pipex->here_doc == -1)
		{
			close_fds(pipex);
			print_error("close error", pipex, EXIT_FAILURE);
		}
	}
	else
	{
		//文件不存在則創建它，並以只寫方式打開，如果文件存在則清空文件的內容
		pipex->outfile = open(argv[pipex->cmd_nbr + 2], O_CREAT| O_WRONLY | O_TRUNC, 0777);
		if (pipex->outfile == -1)
		{
			close_fds(pipex);
			print_error("close error", pipex, EXIT_FAILURE);
		}
	}
}


void	init_pipex_data(int argc, char **argv,char **envp, t_pipex *pipex)
{
	
	pipex->cmd_nbr = argc - 3 - pipex->here_doc;
	pipex->cmd_arr = NULL;
	pipex->cmd_path = NULL;
	//pipe_nbr = pipex->cmd_nbr - 1;
	init_envp(envp, pipex);
	pipex->pipe_fd = (int **)malloc(sizeof(int *) * (pipex->cmd_nbr - 1));
	if (!pipex->pipe_fd)
		print_error("malloc error", pipex, EXIT_FAILURE);
	pipex->pid = (int *)malloc (sizeof(int) * pipex->cmd_nbr);
	if (!pipex->pid)
		print_error("malloc error", pipex, EXIT_FAILURE);
	if (pipex->here_doc == 1)//init infile
	{
		get_input(pipex, argv);
	}
	else
	{
		
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
		{
			//close_fds(&pipex);
			
			only_print_error("infile opened failed");
			//print_error(argv[1], pipex, EXIT_FAILURE);
		}
	}
	
	get_outfile(argv, pipex);// init outfile  check if need to free sth if fails
	
	dprintf(2,"pipex->cmd_nbr: %d\n", pipex->cmd_nbr);
	//dprintf(2,"pipex->cmd_arr: %s\n", pipex->cmd_arr);
	dprintf(2,"pipex->cmd_path: %s\n", pipex->cmd_path);
	dprintf(2,"pipex->here_doc: %d\n", pipex->here_doc);
	dprintf(2,"pipex->infile: %d\n", pipex->infile);
	dprintf(2,"pipex->outfile: %d\n", pipex->outfile);
}


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

char *get_cmd_path( t_pipex *pipex, char **cmd_arr)
{
	int i;
	char *str;
	char *command_path;

	i = 0;
	while (pipex->envp_paths[i])
	{
		str = ft_strjoin( pipex->envp_paths[i] , "/"); //handle error
		
		command_path = ft_strjoin(str, *cmd_arr);
		/*dprintf(2, "i: %d\n", i);
		dprintf(2, "pipex->envp_paths[0]: %s\n", pipex->envp_paths[0]);
		dprintf(2, "pipex->envp_paths[1]: %s\n", pipex->envp_paths[1]);
		dprintf(2, "pipex->envp_paths[2]: %s\n", pipex->envp_paths[2]);
		dprintf(2, "pipex->cmd_arr[0] %s\n", pipex->cmd_arr[0]);
		dprintf(2, "command_path1: %s\n", command_path);
		dprintf(2, "str: %s\n", str);*/
		//free(str);
		if (access(command_path, F_OK) == 0)
		{
			dprintf(2,"command_path2 %s\n", command_path);
			return (command_path);
		}
			
		free(command_path);
		i++;
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
		dprintf(2, "child first\n");
		sub_dup2(pipex->infile, pipex->pipe_fd[0][1], pipex);
		pipex->cmd_arr = ft_split(argv[2 + pipex->here_doc], ' ');
		dprintf(2, "pipex->cmd_arr child1 %s\n", pipex->cmd_arr[0]);
		
	}
	else if (i == (pipex->cmd_nbr - 1))//last cmd 3
	{
		dprintf(2, "child last\n");
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->outfile, pipex);
		dprintf(2, "argv[3] %s\n", argv[3]);
		pipex->cmd_arr = ft_split(argv[i + 2 + pipex->here_doc], ' ');
		dprintf(2, "pipex->cmd_arr %s\n", pipex->cmd_arr[0]);
	}
	else //middle cmd
	{
		dprintf(2, "child middle\n");
		sub_dup2(pipex->pipe_fd[i - 1][0], pipex->pipe_fd[i][1], pipex);
		pipex->cmd_arr = ft_split(argv[i + 2 + pipex->here_doc], ' ');
	}
	if (pipex->cmd_arr == NULL)
		print_error("error malloc", pipex, EXIT_FAILURE);
	close_fds(pipex);
	pipex->cmd_path = get_cmd_path( pipex, &pipex->cmd_arr[0]);// may need go back to free command_path
	dprintf(2, "pipex->cmd_path %s\n", pipex->cmd_path);
	execve(pipex->cmd_path,pipex->cmd_arr,envp);
	print_error(argv[i + 2 + pipex->here_doc], pipex, EXIT_CMD_NOT_FOUND);//if execve fails this line will run
}  

int ft_pipex(char **argv, char **envp, t_pipex *pipex)
{
	int status;
	int i;
	
	create_pipe(pipex);
	i = -1;
	while(++i < pipex->cmd_nbr)
	{
		
		pipex->pid[i] = fork();
		
		if (pipex->pid[i] == 0)
		{
			dprintf(2, "i in ft_pipex: %d\n", i);
			child(i, argv, envp, pipex);
		}
			
		if (pipex->pid[i] == -1)
			print_error("error fork", pipex, EXIT_FAILURE);
	}

	close_fds(pipex);//clsoe fds for parent process
	
	//waitpid(-1, &status, 0);//for test
	//if (waitpid(-1, &status, 0) == -1)// pid= -1, waitpid() waits for any child process to end.
		//print_error("error wairpid", pipex, EXIT_FAILURE);
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
	
	init_pipex_data(argc, argv, envp, &pipex);
	
	status = ft_pipex(argv, envp, &pipex);
	
 	//if (WIFEXTED(status))
	if (status == 0)
	{
		if (pipex.here_doc)
		{
			
			if (unlink(".here_doc") == -1)
				print_error("error unlink", &pipex, EXIT_FAILURE);
			
			free_struct(&pipex);
			//exit(WIFEXTED(status));
			exit(1);
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