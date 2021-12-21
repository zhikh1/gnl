/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomargen <nomargen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:21:15 by nomargen          #+#    #+#             */
/*   Updated: 2021/12/21 21:18:30 by nomargen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

typedef struct line_s{
	int				fd;
	char			*str;
	size_t			buf_size;
	size_t			read_size;
	size_t			fact_size;
	int				eol_flag;
	struct line_s	**head;
	struct line_s	*next_line;
}	t_line;

void	ft_update_fact_size(t_line *line_struct, int only_tail);
char	*get_next_line(int fd);
char	*ft_realloc(char *src, size_t src_size, size_t dst_size, int del);
t_line	*get_line_struct(int fd);
char	*change_size(t_line *line_struct, int split_en);
void	free_str(t_line *line_struct);

#endif