/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 13:39:39 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/22 21:13:08 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <stdlib.h>//free malloc exit
# include <unistd.h>//write close dup2 fork pipe access execve unlink STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO 
# include <fcntl.h>//open
# include <sys/types.h>//fork
# include <stdio.h>//perror
# include <sys/wait.h> //waipid


# define EX_ARGS "e.g.: ./pipex infile cmd1 cmd2 ... cmdn outfile"
# define EX_HEREDOC_ARGS "e.g.: ./pipex here_doc LIMITER cmd cmd1 file"
# define ERR_INPUT "Error: invalid number of arguments"
# define ERR_CMD ": command not found"
# define EXIT_CMD_NOT_FOUND 127



typedef struct s_pipex
{
	int here_doc;
	int cmd_nbr;
	char **cmd_arr;//ls wc
	char *cmd_path;//    /bin/cat
	
	char	**envp_paths;
	/* 
	 PATH=/Users/yhsu/.brew/bin
	 /usr/local/bin:/usr/bin
	 /bin
	 /usr/sbin
	 /sbin
	 /usr/local/munki% 
	 */
	
	int **pipe_fd;
	
	int *pid;
	int infile;
	int outfile;
	

}t_pipex;

//error_bonus.c
void print_error(char *name, t_pipex *pipex, int err_cmd);
void only_print_error(char *name);
void error_exit(int argc, char **argv);
void print_error_badcmd(char *name, t_pipex *pipex, int err_cmd);


//  free_bonus.c 
void free_arr(char **arr);
void free_struct(t_pipex *pipex);
void free_int_arr(int **arr);


//child
void close_fds(t_pipex *pipex);
void	sub_dup2(int input, int output, t_pipex *pipex);
void child(int i, char **argv, char **envp, t_pipex *pipex);
char *get_cmd_path( t_pipex *pipex, char **cmd_arr);
char **get_cmd_arr(char *argv, t_pipex *pipex);
char	**ft_split_pipex(char *str, char *charset);

//init data

void	init_pipex_data(int argc, char **argv,char **envp, t_pipex *pipex);
void init_envp(char **envp, t_pipex *pipex, char **argv);
//void init_envp(char **envp, t_pipex *pipex);
void get_input(t_pipex  *pipex, char **argv);
void input_f_stdin(t_pipex  *pipex, char **argv, int heredoc_fd);

//main
int ft_pipex(char **argv, char **envp, t_pipex *pipex);
void create_pipe(t_pipex *pipex);
int main(int argc, char **argv, char **envp);
#endif