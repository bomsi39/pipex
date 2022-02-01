/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:08:20 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:11:10 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	if (argc < 5)
		wrong_input(argc);
	else
	{
		env = parse_args(argc, argv, envp);
		if (!env->error)
			pipex(env);
		free_env(env);
	}
	return (0);
}

/*
Pipex takes arguments from the terminal and sees them depending on their position
as input/ouput files or as commands. The minimum input is 5 arguments
First argument is the execution of pipex (./pipex)
The second is the input file or here_doc for bonus.
The third to nth arguments are execution commands.
The last argument is the output file.
In my approach an environment struct is created.
If this parsing into the struct happens without problems, pipex is executed.
The struct is freed afterwards.

In my appri
*/