/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:37:56 by qang              #+#    #+#             */
/*   Updated: 2024/07/27 23:22:36 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

void	set_rl_term(struct termios *orig_termios)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, orig_termios);
	term = *orig_termios;
	term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
								| INLCR | IGNCR | ICRNL | IXON);
	term.c_oflag = term.c_oflag;
  // term.c_oflag &= ~OPOST;
	term.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN);
  // term.c_lflag &= ~ISIG;
	term.c_cflag &= ~(CSIZE | PARENB);
	term.c_cflag |= CS8;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void	reset_terminal_mode(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

void	get_term_size(int *row, int *col)
{
	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	*row = w.ws_row;
	*col = w.ws_col;
}
