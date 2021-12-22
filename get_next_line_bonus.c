/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomargen <nomargen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:19:11 by nomargen          #+#    #+#             */
/*   Updated: 2021/12/22 21:33:34 by nomargen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

void	free_str(t_line *line_struct)
{
	t_line	*prev;

	if (line_struct && line_struct->head)
	{
		prev = *(line_struct->head);
		while (prev && prev->next_line != line_struct)
			prev = prev->next_line;
		if (line_struct == *(line_struct->head))
			*(line_struct->head) = line_struct->next_line;
		else if (prev)
			prev->next_line = line_struct->next_line;
		if (line_struct->str)
			free(line_struct->str);
		free(line_struct);
	}
}

void	small(size_t i, size_t end, char *str, t_line *line_struct)
{
	while (i < end && str[i] && str[i] != '\n' && str[i] != '\r')
		i++;
	if (i < end)
	{
		line_struct->fact_size = i + (str[i] == '\n' || str[i] == '\r');
		line_struct->eol_flag = ((str[i] == '\n')
				|| (str[i] == '\r') || (!line_struct->read_size));
	}
	else if (i != 0)
	{
		line_struct->fact_size = i + (str[i - 1] == '\n'
				|| str[i - 1] - 1 == '\r');
		line_struct->eol_flag = ((str[i - 1] == '\n')
				|| (str[i - 1] == '\r') || (!line_struct->read_size));
	}
	else
	{
		line_struct->fact_size = 0;
		line_struct->eol_flag = 0;
	}
}

void	ft_update_fact_size(t_line *line_struct, int only_tail)
{
	size_t	i;
	size_t	end;
	char	*str;

	str = line_struct->str;
	if (((int)line_struct->buf_size >= (int)BUFFER_SIZE) && str)
	{
		if (only_tail)
		{
			i = (line_struct->buf_size - BUFFER_SIZE);
			end = line_struct->buf_size - BUFFER_SIZE + line_struct->read_size;
		}
		else
		{
			i = 0;
			end = line_struct->buf_size - BUFFER_SIZE;
		}				
		small(i, end, str, line_struct);
	}
	else
	{		
		line_struct->fact_size = 0;
		line_struct->eol_flag = 1;
	}
}

char	*check_preveous_data(t_line *line_struct)
{
	char	*new_line;

	new_line = NULL;
	if (line_struct)
	{
		if (line_struct->eol_flag && line_struct->read_size)
		{
			line_struct->buf_size = BUFFER_SIZE;
			new_line = change_size(line_struct, 1);
		}
		else
			line_struct->eol_flag = 0;
	}
	return (new_line);
}

char	*get_next_line(int fd)
{
	t_line	*line_struct;
	char	*new_line;

	if (BUFFER_SIZE <= 0 && fd < 0)
		return (NULL);
	line_struct = get_line_struct(fd);
	new_line = check_preveous_data(line_struct);
	while (line_struct && !line_struct->eol_flag && line_struct->str)
	{
		line_struct->read_size = read(fd, &line_struct->str
			[line_struct->buf_size - BUFFER_SIZE], BUFFER_SIZE);
		if ((long int)line_struct->read_size >= 0)
		{	
			ft_update_fact_size(line_struct, 1);
			new_line = change_size(line_struct, line_struct->eol_flag);
		}
		else
		{
			free_str(line_struct);
			return (NULL);
		}
	}
	return (new_line);
}
