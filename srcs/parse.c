/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:27:27 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/30 20:06:14 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_cmds(char **pths, t_env *env)
{
	t_cmd	*node;
	int		i;
	char	*tmp;

	node = env->fir_cmd;
	while (node)
	{
		i = 0;
		while (pths[i])
		{
			tmp = ft_strinsjoin(pths[i++], node->array[0], '/');
			if (!access(tmp, F_OK))
			{
				if (access(tmp, X_OK))
					env->error = terminate(env, ERR_PER, node->array[0]);
				node->pth = ft_strdup(tmp);
			}
			free(tmp);
		}
		if (!node->pth)
			env->error = terminate(env, ERR_CMD, node->array[0]);
		node = node->next;
	}
	free_array(pths);
}

void	lmt_info(t_env *env, char **argv)
{
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		env->lmt->islmt = true;
		env->cmd_no -= 1;
		env->lmt->lmtr = argv[2];
	}
}

void	parse_cmd(t_env *env, char **argv, char **envp)
{
	int		i;
	char	*t_paths;

	t_paths = NULL;
	i = 2 + env->lmt->islmt;
	while (i < env->ac - 1)
	{
		create_cmd(env, ft_split(argv[i], ' '));
		i++;
	}
	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			t_paths = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 1));
		if (!ft_strncmp(envp[i], "SHELL=/bin/", 11))
			env->shell = ft_substr(envp[i], 11, (ft_strlen(envp[i])));
	}
	check_cmds(ft_split(t_paths, ':'), env);
	free(t_paths);
}

void	access_io(t_env *env, char **argv)
{
	if (!env->lmt->islmt)
	{
		if (access(argv[1], F_OK))
			env->error = terminate(env, ERR_PTH, argv[1]);
		if (access(argv[1], R_OK))
			env->error = terminate(env, ERR_PER, argv[1]);
		if (!access(argv[env->ac - 1], F_OK))
		{
			if (access(argv[env->ac - 1], W_OK))
				env->error = terminate(env, ERR_PER, argv[env->ac - 1]);
		}
	}
	else if (!access(argv[env->ac - 1], F_OK))
	{
		if (access(argv[env->ac - 1], W_OK))
			env->error = terminate(env, ERR_PER, argv[env->ac - 1]);
	}
}

t_env	*parse_args(int argc, char **argv, char **envp)
{
	t_env	*env;

	env = init_env(argc, envp);
	lmt_info(env, argv);
	parse_cmd(env, argv, envp);
	access_io(env, argv);
	if (!env->lmt->islmt)
	{
		env->fdin = open(argv[1], O_RDONLY);
		env->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT, 0666);
	}
	else
		env->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	return (env);
}
