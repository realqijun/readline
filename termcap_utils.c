/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 22:39:48 by qang              #+#    #+#             */
/*   Updated: 2024/07/16 20:21:56 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>

void	clear_scr(void);
void	ft_init_term(void);

void	ft_init_term(void)
{
	char	*env;
	int success;

	env = getenv("TERM");
	if (env == NULL)
	{
		dprintf(2, "TERM is not set\n");
		exit(1);
	}
	success = tgetent(NULL, env);
	if (success < 0)
	{
		dprintf(2, "Could not access the termcap database\n");
		exit(1);
	}
	if (success == 0)
	{
		dprintf(2, "Terminal type %s is not defined\n", env);
		exit(1);
	}
}

void	clear_scr(void)
{
	if (tputs(tgetstr("cl", NULL), 2, ft_putchar) == ERR)
	{
		dprintf(2, "Error while clearing the screen\n");
		exit(1);
	}
}

void	put_cap(char *cap, int n)
{
	char	*str;

	str = tgetstr(cap, NULL);
	while (n-- > 0)
		tputs(str, 1, ft_putchar);
}

void	alert_sound(void)
{
	if (tputs(tgetstr("bl", NULL), 1, ft_putchar) == ERR)
	{
		dprintf(2, "Error while alerting\n");
		exit(1);
	}
}
