/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:54:07 by paulguign         #+#    #+#             */
/*   Updated: 2021/12/03 17:39:47 by mkralik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_cmd(char *arg)
{
	if (!arg)
	{
		arg = (char *)malloc(sizeof(char));
		if (error_catch(!arg, NULL, "Malloc failed"))
			exit (1);
		*arg = 0;
	}
	error_catch(1, arg, "command not found");
	if (!arg[0])
		free(arg);
}

static int	ft_pipe_exec(t_data *data, t_lst *lst, int *fd)
{
	char **ch_env;

	ch_env = NULL;
	if (error_catch(dup2(fd[0], STDIN_FILENO) < 0, NULL, strerror(errno)))
	{
		g_exit_status = 1;
		exit (g_exit_status);
		//exit (ft_free_data(data, 1));
	}
	if (error_catch(dup2(fd[1], STDOUT_FILENO) < 0, NULL, strerror(errno)))
	{
		g_exit_status = 1;
		exit (g_exit_status);
		//exit (ft_free_data(data, 1));
	}
	close(fd[0]);
	close(fd[1]);
	ch_env = get_env_to_char(data->env);
	ft_execute(data, 0, lst, ch_env);
	free_dble_str(ch_env);
	exit (0);
}

int	ft_pipe(t_data *data, t_lst *lst, int fd_in, int step)
{
	int	fd[2];
	int	pid;
	int	status;
	int	ret;

	status = 0;
	if (lst && lst->builtin && !lst->next && (!ft_strcmp(lst->cmd, "exit") || !ft_strcmp(lst->cmd, "export")))
	{
		ret = exec_builtin(lst, data);
		return (ret);
	}
	if (error_catch(pipe(fd) < 0, NULL, strerror(errno)))
	{
		//Tout free et sortir
		return (1);
	}
	pid = fork();
	if (error_catch(pid < 0, NULL, strerror(errno)))
		exit (1);
		//exit (ft_free_data(data, 1));
	if (pid == 0)
	{
		close(fd[0]);
		if (fd_in == -1 && lst->input == 0)
			fd[0] = open("/dev/stdin", O_RDONLY);
		else if (lst->input == 0)
			fd[0] = fd_in;
		else
			fd[0] = lst->input;
		if (lst->output == 0 && !lst->next)
		{
			close(fd[1]);
			fd[1] = open("/dev/stdout", O_WRONLY);
		}
		else if (lst->output != 0)// && lst->output != 0)
		{
			close(fd[1]);
			fd[1] = lst->output;
		}
		ft_pipe_exec(data, lst, fd);
	}
	close(fd[1]);
	if (lst->next)
		ret = ft_pipe(data, lst->next, fd[0], step + 1);
	close(fd[0]);
	waitpid(pid, &status, 0);
	if (!lst->next)
		ret = WEXITSTATUS(status);
	return (ret);
}