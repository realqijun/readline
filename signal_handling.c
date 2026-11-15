/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:48:18 by qang              #+#    #+#             */
/*   Updated: 2024/07/15 15:54:07 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

#include <signal.h>
#include <unistd.h>

static struct termios	g_orig_termios;

static void	handle_sigint(int sig)
{
	reset_terminal_mode(&g_orig_termios);
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

void	set_signals(struct termios *orig_termios)
{
	g_orig_termios = *orig_termios;
	signal(SIGINT, handle_sigint);
}
