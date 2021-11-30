/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkralik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:54:07 by paulguign         #+#    #+#             */
/*   Updated: 2021/11/30 14:28:25 by lcavallu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_cmd(char *arg)
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
	if (!lst->path)
	{
		error_cmd(lst->cmd);
		//ft_free_split(split);
		//exit (ft_free_data(data, 127));
		g_exit_status = 127;
		return(g_exit_status);
	}
	ch_env = get_env_to_char(data->env);
	ft_execute(data, 0, lst, ch_env);
	free_dble_str(ch_env);
	// execve(lst->path, lst->arg, ch_env);
	exit (0);
}

int	ft_pipe(t_data *data, t_lst *lst, int fd_in, int step)
{
	int	fd[2];
	int	pid;
	int	status;
	int	ret;

	status = 0;
	if (lst && lst->builtin && !lst->next)
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
		if (lst->input == -2)
			fd[0] = fd_in;
		else
			fd[0] = lst->input;
		if (lst->output != -1)
		{
			close(fd[1]);
			fd[1] = lst->output;
		}
		ft_pipe_exec(data, lst, fd);
		//ft_redirection(data, fd, fd_in, step);
	}
	close(fd[1]);
	if (lst->next)
		ret = ft_pipe(data, lst->next, fd[0], step + 1);
	close(fd[0]);
	waitpid(pid, &status, 0);
	if (!lst->next)
		ret = WEXITSTATUS(status);
		//ret = ft_free_data(data, WEXITSTATUS(status));
	return (ret);
}
