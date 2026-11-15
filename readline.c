/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qang <qang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:37:15 by qang              #+#    #+#             */
/*   Updated: 2024/07/30 23:50:51 by qang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "readline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

void  start(t_readline_info *rl_info)
{
	ft_init_term();
	get_term_size(&rl_info->row, &rl_info->col);
	set_rl_term(&rl_info->orig_termios);
	set_signals(&rl_info->orig_termios);
	rl_info->cur_x = 0;
	rl_info->cur_y = 0;
	rl_info->pad = 0;
}

static char	*prompt_line(char *prompt)
{
	char	*nl;

	nl = ft_strrchr(prompt, '\n');
	if (nl)
		return (nl + 1);
	return (prompt);
}

static int	prompt_width(char *prompt)
{
	unsigned char	*s;
	int				width;

	s = (unsigned char *)prompt_line(prompt);
	width = 0;
	while (*s)
	{
		if (*s == 27 && s[1] == '[')
		{
			s += 2;
			while (*s && (*s < '@' || *s > '~'))
				s++;
		}
		else if (*s >= ' ' && *s != 127 && (*s & 0xC0) != 0x80)
			width++;
		if (*s)
			s++;
	}
	return (width);
}

static void	move_to(t_readline_info *rl_info, int pos)
{
	int	row;

	row = pos / rl_info->col;
	put_cap("up", rl_info->cur_y - row);
	put_cap("do", row - rl_info->cur_y);
	write(1, "\r", 1);
	put_cap("nd", pos % rl_info->col);
	rl_info->cur_y = row;
}

static void	redraw_line(char *line, t_readline_info *rl_info)
{
	char	*last;
	int		end;

	last = prompt_line(rl_info->prompt);
	move_to(rl_info, 0);
	put_cap("cd", 1);
	write(1, last, strlen(last));
	write(1, line, strlen(line));
	end = rl_info->pad + strlen(line);
	if (end > 0 && end % rl_info->col == 0)
		write(1, "\r\n", 2);
	rl_info->cur_y = end / rl_info->col;
	move_to(rl_info, rl_info->cur_x);
}

void	print_prompt(t_readline_info* rl_info)
{
	write(1, rl_info->prompt, prompt_line(rl_info->prompt) - rl_info->prompt);
	rl_info->cur_x = rl_info->pad;
	rl_info->cur_y = 0;
	redraw_line("", rl_info);
}

void	backspace(char **line, t_readline_info* rl_info)
{
	char	*pos;

	if (rl_info->cur_x > rl_info->pad)
	{
		rl_info->cur_x--;
		pos = *line + rl_info->cur_x - rl_info->pad;
		memmove(pos, pos + 1, strlen(pos));
		redraw_line(*line, rl_info);
	}
}

static void	insert_char(char c, char **line, t_readline_info *rl_info)
{
	*line = insert(*line, c, rl_info->cur_x - rl_info->pad);
	rl_info->cur_x++;
	redraw_line(*line, rl_info);
}

static void	complete(char **line, t_readline_info *rl_info)
{
	char	*suffix;
	int		i;

	suffix = complete_line(*line, rl_info->cur_x - rl_info->pad);
	if (suffix == NULL)
	{
		alert_sound();
		return ;
	}
	i = 0;
	while (suffix[i])
	{
		*line = insert(*line, suffix[i++], rl_info->cur_x - rl_info->pad);
		rl_info->cur_x++;
	}
	free(suffix);
	redraw_line(*line, rl_info);
}

void  add_history(t_readline_info *rl_info, t_history **up_history, t_history **down_history, char **line)
{
	t_history	*node;

	move_to(rl_info, rl_info->pad + strlen(*line));
	write(1, "\r\n", 2);
	if (*down_history != NULL)
	{
		while (*down_history)
			push(down_history, up_history);
		node = *up_history;
		while (node)
		{
			free(node->tmp);
			node->tmp = NULL;
			node = node->next;
		}
		push(up_history, down_history);
		free((*down_history)->line);
		free(*down_history);
		*down_history = NULL;
	}
	append_history(*line, up_history);
	*line = strdup("");
	print_prompt(rl_info);
}

static void	load_history(t_history *entry, char **line, t_readline_info *rl_info)
{
	if (entry->tmp != NULL)
		*line = strdup(entry->tmp);
	else
		*line = strdup(entry->line);
	rl_info->cur_x = strlen(*line) + rl_info->pad;
	redraw_line(*line, rl_info);
}

void  handle_special_char(int c, char **line, t_readline_info* rl_info)
{
	static t_history	*up_history = NULL;
	static t_history	*down_history = NULL;

	if (c == 4)
	{
		reset_terminal_mode(&rl_info->orig_termios);
		exit(0);
	}
	else if (c == 9)
		complete(line, rl_info);
	else if (c == 127)
		backspace(line, rl_info);
	else if (c == 10 || c == 13)
		add_history(rl_info, &up_history, &down_history, line);
	else if (c == 27)
	{
		if (read(STDIN_FILENO, &c, 1) > 0 && c == 91)
		{
			read(STDIN_FILENO, &c, 1);
			if (c == 'A')
			{
				if (down_history == NULL)
				{
					append_history(*line, &down_history);
					down_history->final = true;
				}
				else
				{
					free(down_history->tmp);
					down_history->tmp = *line;
				}
				push(&up_history, &down_history);
				load_history(down_history, line, rl_info);
			}
			else if (c == 'B' && down_history && !down_history->final)
			{
				free(down_history->tmp);
				down_history->tmp = *line;
				push(&down_history, &up_history);
				load_history(down_history, line, rl_info);
			}
			else if (c == 'C')
			{
				if (rl_info->cur_x < (int)strlen(*line) + rl_info->pad)
					rl_info->cur_x++;
			}
			else if (c == 'D')
			{
				if (rl_info->cur_x > rl_info->pad)
					rl_info->cur_x--;
			}
		}
		move_to(rl_info, rl_info->cur_x);
	}
}

char  *start_read(t_readline_info *rl_info)
{
	char  c;
	char *line;

	line = ft_strdup("");
	while (read(STDIN_FILENO, &c, 1) > 0 && c != 'q')
	{
		/*if (c == 10 || c == 13)*/
		/*	return (line);*/
		if (ft_iscntrl(c))
			handle_special_char(c, &line, rl_info);
		else
			insert_char(c, &line, rl_info);
	}
	return (NULL);
}

char	*ft_readline(char *prompt)
{
	t_readline_info rl_info;
	char  *line;

	start(&rl_info);
	rl_info.prompt = prompt;
	rl_info.pad = prompt_width(prompt);
	clear_scr();
	print_prompt(&rl_info);
	line = start_read(&rl_info);
	reset_terminal_mode(&rl_info.orig_termios);
	return (line);
}

