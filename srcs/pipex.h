/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 13:39:39 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/26 12:05:24 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
//free malloc exit
# include <stdlib.h>
//write close dup2 fork pipe access 
//execve unlink STDIN_FILENO, 
//STDOUT_FILENO, STDERR_FILENO 
# include <unistd.h>
//open
# include <fcntl.h>
//fork
# include <sys/types.h>
//perror
# include <stdio.h>
//waipid
# include <sys/wait.h>

# define EX_ARGS "e.g.: ./pipex infile cmd1 cmd2 outfile"
# define EX_HEREDOC_ARGS "e.g.: ./pipex here_doc LIMITER cmd cmd1 file"
# define ERR_INPUT "Error: invalid number of arguments"
# define ERR_CMD ": command not found"
# define ERR_UNLINK "unlink error"
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CMD_PERMMIT_ERR 126

typedef struct s_pipex
{
	int		here_doc;
	int		cmd_nbr;
	char	**cmd_arr;
	char	*cmd_path;
	char	**envp_paths;
	int		**pipe_fd;
	int		*pid;
	int		infile;
	int		outfile;
}	t_pipex;

//error_bonus.c
void	print_error(char *name, t_pipex *pipex, int exit_code);
void	only_print_error(char *name);
void	error_exit(int argc, char **argv);
void	print_error_badcmd(char *name, t_pipex *pipex, int exit_code);

//  free_bonus.c 
void	free_arr(char **arr);
void	free_struct(t_pipex *pipex);
void	free_int_arr(int **arr);
void	parent_free(t_pipex *pipex);
void	print_error_partial_free(char *name, t_pipex *data);
void	free_struct_badcmd(t_pipex *pipex);

//child
void	sub_dup2(int input, int output, t_pipex *pipex);
char	*get_cmd_path( t_pipex *pipex, char **cmd_arr);
char	**get_cmd_arr(char *argv, t_pipex *pipex);
void	get_infile(char **argv, t_pipex *pipex);
void	child(int i, char **argv, char **envp, t_pipex *pipex);

//utils 
void	close_fds(t_pipex *pipex);
char	*no_quote(char *cmd, t_pipex *pipex);
char	*verify_path(char *cmd, t_pipex *pipex);
char	**get_env_paths(char **envp, t_pipex *pipex);

//init data
void	init_pipex_data(int argc, char **envp, t_pipex *pipex);
void	init_envp(char **envp, t_pipex *pipex);
void	get_input(t_pipex *pipex, char **argv);
void	input_f_stdin(t_pipex *pipex, char **argv, int heredoc_fd);
void	get_outfile(char **argv, t_pipex *pipex);

//main
int		ft_pipex(char **argv, char **envp, t_pipex *pipex);
void	create_pipe(t_pipex *pipex);
int		main(int argc, char **argv, char **envp);
#endif