/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomargen <nomargen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:19:11 by nomargen          #+#    #+#             */
/*   Updated: 2021/12/22 21:43:41 by nomargen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

t_line	*get_new_line(int fd)
{
	t_line	*new_line;

	new_line = (t_line *)malloc(sizeof(t_line));
	if (new_line)
	{
		new_line->fd = fd;
		new_line->str = malloc(sizeof(char) * (unsigned int)BUFFER_SIZE);
		new_line->buf_size = BUFFER_SIZE;
		new_line->read_size = 0;
		new_line->fact_size = 0;
		new_line->eol_flag = 0;
		new_line->next_line = NULL;
		if (new_line->str == NULL)
		{
			free(new_line);
			new_line = NULL;
		}
	}
	return (new_line);
}

t_line	*get_line_struct(int fd)
{
	static t_line	*lst_head = NULL;
	t_line			*crnt;

	if (lst_head)
	{
		crnt = lst_head;
		while (crnt->next_line && crnt->fd != fd)
			crnt = crnt->next_line;
		if (crnt->next_line == NULL && crnt->fd != fd)
		{
			crnt->next_line = get_new_line(fd);
			crnt = crnt->next_line;
			crnt->head = &lst_head;
		}
		else
			ft_update_fact_size(crnt, 0);
	}
	else
	{
		crnt = get_new_line(fd);
		lst_head = crnt;
		crnt->head = &lst_head;
	}
	return (crnt);
}

char	*ft_realloc(char *src, size_t src_size, size_t dst_size, int del)
{
	char	*new;
	size_t	i;

	i = 0;
	new = (char *) malloc(sizeof(char) * dst_size);
	while ((i < src_size) && (i < dst_size) && new && src)
	{
		new[i] = src[i];
		i++;
	}
	if (src && new && del)
		free(src);
	return (new);
}

char	*remap_ptrs(char *tail_pt, int split_en,
	t_line *line_struct, size_t	tail_size)
{
	char	*line_pt;

	if (line_struct->fact_size)
		line_pt = ft_realloc(line_struct->str,
				line_struct->buf_size, line_struct->fact_size
				+ (BUFFER_SIZE * !split_en) + (split_en == 1), 1);
	else
	{
		free(line_struct->str);
		line_pt = NULL;
	}
	line_struct->str = line_pt;
	line_struct->buf_size = line_struct->fact_size + (BUFFER_SIZE * !split_en);
	if (split_en)
	{
		line_struct->str = tail_pt;
		line_struct->buf_size = tail_size;
		line_struct->read_size = tail_size - BUFFER_SIZE;
		if (line_pt)
			line_pt[line_struct->fact_size] = 0;
	}	
	if (!line_pt && tail_pt)
		free(tail_pt);
	return (line_pt);
}

char	*change_size(t_line *line_struct, int split_en)
{
	char	*line_pt;
	char	*tail_pt;
	size_t	tail_size;

	tail_pt = NULL;
	tail_size = line_struct->buf_size
		+ line_struct->read_size - line_struct->fact_size;
	if (split_en)
	{
		tail_pt = ft_realloc(&line_struct->str
			[line_struct->fact_size], tail_size - BUFFER_SIZE, tail_size, 0);
	}
	line_pt = remap_ptrs(tail_pt, split_en, line_struct, tail_size);
	if (line_struct->fact_size == 0)
		free_str(line_struct);
	return (line_pt);
}
