/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relkabou <relkabou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 18:37:51 by relkabou          #+#    #+#             */
/*   Updated: 2022/12/02 19:22:38 by relkabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exec_first(t_params *p)
{
	int	fd;

	fd = open(p->cmd1.file, O_RDONLY);
	if (fd == -1)
	{
		print_error("bash", p->cmd1.file, "No such file or directory");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 0) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (dup2(p->fd[1], 1) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(p->fd[0]) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(p->fd[1]) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(fd) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (execve(p->cmd1.path, p->cmd1.args, p->main_params.env) == -1)
	{
		print_error("bash", p->cmd1.cmd, "command not found");
		exit(127);
	}
}

static void	exec_second(t_params *p)
{
	int		fd;

	fd = open(p->cmd2.file, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		perror(p->cmd2.file);
	if (dup2(fd, 1) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(fd) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (dup2(p->fd[0], 0) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(p->fd[1]) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (close(p->fd[0]) == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (execve(p->cmd2.path, p->cmd2.args, p->main_params.env) == -1)
	{
		print_error("bash", p->cmd2.cmd, "command not found");
		exit(127);
	}
}

int	main(int ac, char **av, char **env)
{
	t_params	p;
	pid_t		pid1;
	pid_t		pid2;

	p.main_params.ac = ac;
	p.main_params.av = av;
	p.main_params.env = env;
	parsing(&p);
	if (pipe(p.fd) == -1)
	{
		perror("");
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (!pid1)
		exec_first(&p);
	pid2 = fork();
	if (pid2 == -1)
		ft_exits(errno, 1, EXIT_FAILURE);
	if (!pid2)
		exec_second(&p);
	at_exit(&p, &pid1, &pid2);
	return (1);
}
