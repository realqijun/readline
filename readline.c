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

#include "readline.h"
#include <ctype.h>
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
  rl_info->cur_x = 0;
  rl_info->cur_y = 0;
  rl_info->pad = 0;
}

void	print_prompt(t_readline_info* rl_info)
{
	rl_info->cur_x = write(1, rl_info->prompt, strlen(rl_info->prompt));
}

void	backspace(char **line, t_readline_info* rl_info)
{
	if (rl_info->cur_x > rl_info->pad)
	{
		rl_info->cur_x--;
		(*line)[strlen(*line) - 1] = '\0';
		clear_line(rl_info->cur_y, rl_info->pad);
		write(1, *line, strlen(*line));
	}
}

void  add_history(t_readline_info *rl_info, t_history **up_history, t_history **down_history, char **line)
{
  char  *temp;

  write(1, "\r\n", 2);
  rl_info->cur_y++;
  rl_info->cur_x = 0;
  if (*down_history == NULL)
    append_history(*line, up_history);
  else
  {
    temp = strdup((*down_history)->tmp);
    free((*down_history)->tmp);
    (*down_history)->tmp = NULL;
    while (*down_history)
      push(down_history, up_history);
    if ((*up_history)->final)
    {
      push(up_history, down_history);
      free((*down_history)->line);
      free(*down_history);
      *down_history = NULL;
    }
    append_history(temp, up_history);
  }
  *line = strdup("");
  print_prompt(rl_info);
}

void  handle_special_char(int c, char **line, t_readline_info* rl_info)
{
	static t_history	*up_history = NULL;
	static t_history  *down_history = NULL;

	if (c == 4)
	{
		reset_terminal_mode(&rl_info->orig_termios);
		exit(0);
	}
	else if (c == 9)
		complete_line(*line);
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
          down_history->tmp = down_history->line;
          down_history->final = true;
        }
        push(&up_history, &down_history);
        if (down_history != NULL)
        {
          clear_line(rl_info->cur_y, rl_info->pad);
          if (down_history->tmp != NULL)
          {
            write(1, down_history->tmp, strlen(down_history->tmp));
            *line = down_history->tmp;
          }
          else
          {
            write(1, down_history->line, strlen(down_history->line));
            *line = strdup(down_history->line);
            down_history->tmp = *line;
          }
          rl_info->cur_x = strlen(*line) + rl_info->pad;
        }
			}
      else if (c == 'B')
      {
        push(&down_history, &up_history);
        if (down_history != NULL)
        {
          clear_line(rl_info->cur_y, rl_info->pad);
          if (down_history->tmp != NULL)
          {
            write(1, down_history->tmp, strlen(down_history->tmp));
            *line = down_history->tmp;
          }
          else
          {
            write(1, down_history->line, strlen(down_history->line));
            *line = strdup(down_history->line);
            down_history->tmp = *line;
          }
          rl_info->cur_x = strlen(*line) + rl_info->pad;
        }
      }
			else if (c == 'C')
      {
        if (rl_info->cur_x < 100 - rl_info->pad && rl_info->cur_x < (int)strlen(*line) + rl_info->pad)
          rl_info->cur_x++;
      }
			else if (c == 'D')
      {
        if (rl_info->cur_x > rl_info->pad)
          rl_info->cur_x--;
      }
    }
		move_cursor(rl_info->cur_y, rl_info->cur_x);
	}
}

char  *start_read(t_readline_info *rl_info)
{
	char  c;
	char *line;
	
	line = strdup("");
	while (read(STDIN_FILENO, &c, 1) > 0 && c != 'q')
	{
    if (c == 10 || c == 13)
      return (line);
		if (ft_iscntrl(c))
			handle_special_char(c, &line, rl_info);
		else
		{
			write(1, &c, 1);
			line = append(line, c);
			rl_info->cur_x++;
		}
	}
	return (NULL);
}

char	*readline(char *prompt)
{
	t_readline_info rl_info;
  char  *line;

	start(&rl_info);
	rl_info.prompt = prompt;
	rl_info.pad = strlen(prompt);
	clear_scr();
	print_prompt(&rl_info);
	line = start_read(&rl_info);
	reset_terminal_mode(&rl_info.orig_termios);
	return (line);
}

int	main(int ac, char **av, char **envp)
{
  char  *line;

  (void)ac;
  (void)av;
  line = readline(SHELL);
  execve(line, (char *[]) {line, NULL}, envp); 
	return (0);
}
