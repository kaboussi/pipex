#include "pipex_bonus.h"

void	display(void *node)
{
	t_cmd *tmp = node;
	ft_fprintf(1, "cmd: %s\n", tmp->cmd);
	ft_fprintf(1, "args: {%s, %s}\n", tmp->args[0], tmp->args[1]);
	ft_fprintf(1, "path: %s\n", tmp->path);
	ft_putstr_fd("=====================\n", 1);
}

t_list	*parse_commands(t_params *p)
{
	t_list	*cmds;
	t_cmd	*node;
	int		i;

	i = 1;
	cmds = NULL;
	node = malloc(sizeof(node));
	if (!node)
		return (NULL);
	while (++i < p->main.ac - 1)
	{
		node->cmd = ft_strdup(p->main.av[i]);
		node->args = ft_split(p->main.av[i], ' ');
		node->path = get_path(node->args[0], p->main.envp);
		ft_lstadd_back(&cmds, ft_lstnew(ft_strdup_cmd(node)));
		free_cmd_node(node);
	}
	return (cmds);
}

static void	parse_pipeline(int argc, char **argv, char **envp, t_params *p)
{
	int		i;

	i = 0;
	p->main.ac = argc;
	p->main.av = argv;
	p->main.envp = envp;
	p->ncmd = argc - 3;
	p->file.infile = argv[1];
	p->file.outfile = argv[argc - 1];
	p->cmds = parse_commands(p);
	p->pipes_ports = (p->ncmd - 1) * 2;
	p->pipelines = malloc(sizeof(int) * p->pipes_ports);
	if (!p->pipelines)
		return ;
	while (i < p->ncmd - 1)
	{
		if (pipe(p->pipelines + i * 2) == -1)
			exit_failure(errno, 1);
		i += 2;
	}	
}

void	pipeline(int argc, char **argv, char **envp)
{
	t_params	p;
	t_cmd		*last_cmd;

	if (argc < 5)
		return ;
	parse_pipeline(argc, argv, envp, &p);
	exec_first_cmd(&p);
	if (p.ncmd > 2)
		exec_middle_cmds(&p);
	last_cmd = ft_lstlast(p.cmds)->content;
	exec_last_cmd(last_cmd, &p);
	at_exit_pipeline(&p);
}







	// ft_lstiter(p.cmds, &display);