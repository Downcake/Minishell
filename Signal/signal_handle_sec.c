/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle_sec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymabsout <ymabsout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 07:22:24 by ymabsout          #+#    #+#             */
/*   Updated: 2024/03/15 07:29:16 by ymabsout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

void	return_def(int c)
{
	(void)c;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	heredoc_signal(int c)
{
	(void)c;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	g_received_signal = -1;
}

void	no_nl(int c)
{
	(void)c;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_received_signal = 2;
}
