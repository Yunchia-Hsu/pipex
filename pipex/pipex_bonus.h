/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 13:39:39 by yhsu              #+#    #+#             */
/*   Updated: 2024/04/12 19:04:15 by yhsu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEN_BONUS_H

#include "./libft/libft.h"
#include <stdlib.h>//free malloc exit
#include <unistd.h>//write close dup2 fork pipe access execve unlink STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO 
#include <fcntl.h>//open
#include <sys/types.h>//fork
#include <sys/wait.h>//waipid
#include <stdio.h>//perror

# define EX_ARGS "e.g.: ./pipex infile cmd1 cmd2 ... cmdn outfile"
# define EX_HEREDOC_ARGS "e.g.: ./pipex here_doc LIMITER cmd cmd1 file"
# define ERR_INPUT "Error: invalid number of arguments"
# define ERR_CMD ": command not found"
# define EXIT_CMD_NOT_FOUND = 127;
# define EXIT_FAIL = 1;


typedef struct s_pipex
{
	int here_doc;
	int cmd_nbr;
	char **cmd_arr;//ls wc
	char *cmd_path;//    /bin/cat
	int pipe_nbr;
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

#endif