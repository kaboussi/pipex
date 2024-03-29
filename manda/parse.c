/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relkabou <relkabou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 15:45:18 by relkabou          #+#    #+#             */
/*   Updated: 2022/12/03 15:45:19 by relkabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_env_path(t_params *p)
{
	while (p->main_params.env)
	{
		if (!*p->main_params.env || !p->main_params.env)
			break ;
		if (!(ft_strncmp("PATH=", *p->main_params.env, 5)))
			return (*p->main_params.env);
		p->main_params.env++;
	}
	return (NULL);
}

static char	**get_valid_paths(t_params *p)
{
	char	**paths;

	if (!p->env_paths)
		return (NULL);
	paths = ft_split(p->env_paths + 5, ':');
	return (paths);
}

static char	*get_path(t_params *p, t_cmd *cmd)
{
	char	**paths;
	char	*path;
	int		i;

	i = -1;
	if (ft_strchr(cmd->cmd, '/') && !access(cmd->cmd, F_OK | X_OK))
		return (ft_strdup(cmd->cmd));
	paths = get_valid_paths(p);
	while (paths && paths[++i])
	{
		path = join_path(paths[i], '/', cmd->cmd);
		if ((access(path, F_OK | X_OK)) == -1)
			free(path);
		else
			return (free_tab(paths), path);
	}
	free_tab(paths);
	return (NULL);
}

int	parsing(t_params *p)
{
	if (p->main_params.ac != 5)
	{
		ft_fprintf(2, "Invalid arguments. Quiting...\n");
		exit(EXIT_FAILURE);
	}
	p->cmd1.args = ft_split(p->main_params.av[2], ' ');
	p->cmd2.args = ft_split(p->main_params.av[3], ' ');
	p->cmd1.file = p->main_params.av[1];
	p->cmd1.cmd = p->cmd1.args[0];
	p->cmd2.cmd = p->cmd2.args[0];
	p->cmd2.file = p->main_params.av[4];
	p->env_paths = get_env_path(p);
	p->cmd1.path = get_path(p, &p->cmd1);
	p->cmd2.path = get_path(p, &p->cmd2);
	return (1);
}
