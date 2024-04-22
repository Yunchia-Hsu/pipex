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

static int	is_sep(char *str, int i, char *charset)
{
	int	j;
	int	k;
	int	flags;

	j = -1;
	while (charset[++j])
	{
		if (str[i] == charset[j])
		{
			k = -1;
			flags = 0;
			while (str[++k] && k <= i)
			{
				if (str[k] == '\\')
					flags ^= (1 << 2);
				if (str[k] == '"')
					flags ^= (1 << 1);
				if (str[k] == '\'')
					flags ^= 1;
			}
			return (flags == 0);
		}
	}
	return (str[i] == '\0');
}

static int	count_word(char *str, char *charset)
{
	int	word;
	int	i;

	word = 0;
	i = 0;
	while (str[i])
	{
		if (!is_sep(str, i, charset)
			&& is_sep(str, i + 1, charset))
			word++;
		i++;
	}
	return (word);
}

static char	*write_word(char *dest, char *str, char *charset)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (is_sep(str, i, charset))
			i++;
		while (!is_sep(str, i, charset))
		{
			if (str[i] == '\\')
				i++;
			dest[j++] = str[i++];
		}
		break ;
	}
	dest[j] = '\0';
	return (dest);
}

static char	**write_arr(char **arr, char *str, char *charset)
{
	int	len;
	int	i;
	int	word;

	len = 0;
	i = 0;
	word = 0;
	while (str[i])
	{
		if (is_sep(str, i, charset))
			i++;
		else
		{
			while (!is_sep(str, i + len, charset))
				len++;
			arr[word] = malloc(sizeof(char) * (len + 1));
			if (arr[word] == NULL)
				return (NULL);
			arr[word] = write_word(arr[word], str + i, charset);
			i += len;
			len = 0;
			word++;
		}
	}
	return (arr);
}

/* 
	A customized ft_split() function, which splits the string with valid 
	separators. A separator is deemed as valid if it's outside any pair 
	of quotes.
*/

char	**ft_split_pipex(char *str, char *charset)
{
	char	**arr;
	int		word;

	word = count_word(str, charset);
	arr = (char **)malloc((word + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	arr = write_arr(arr, str, charset);
	arr[word] = NULL;
	return (arr);
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
}

char *get_cmd_path( t_pipex *pipex, char **cmd_arr)
{
	int i;
	char *str;
	char *command_path;

	i = 0;
	
	while (pipex->envp_paths != NULL && pipex->envp_paths[i] != NULL)
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
                                            // awk '{print}'
char **get_cmd_arr(char *argv, t_pipex *pipex) //awk '{print}' -> cmd_arr0: awk / cmd_arr1: {print}
{
		char **cmd_arr;
		int i;

		cmd_arr = ft_split_pipex(argv, " ");
		if (ft_strncmp(cmd_arr[0],"/bin", 4) == 0)
			cmd_arr[0] = cmd_arr[0] + 5; 
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




void child(int i, char **argv, char **envp, t_pipex *pipex)
{
	if (i == 0)//1 cmd
	{
		sub_dup2(pipex->infile, pipex->pipe_fd[0][1], pipex);
		pipex->cmd_arr = get_cmd_arr(argv[2 + pipex->here_doc], pipex);
	}
	else if (i == (pipex->cmd_nbr - 1))//last cmd 3
	{
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
	dprintf(2, "child pipex->cmd_path: %s\n", pipex->cmd_path);
	dprintf(2, "child pipex->cmd_arr[0]: %s\n", pipex->cmd_arr[0]);
	if (pipex->cmd_path != NULL)
		execve(pipex->cmd_path,pipex->cmd_arr,envp);
	print_error_badcmd(argv[i + 2 + pipex->here_doc], pipex, EXIT_CMD_NOT_FOUND);//if execve fails this line will run
}
