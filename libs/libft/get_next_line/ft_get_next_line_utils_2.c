/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalcaide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:08:43 by aalcaide          #+#    #+#             */
/*   Updated: 2025/12/18 16:47:51 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_cut_new_line(char **all_line)
{
	char	*cut_line;
	char	*temp_all_line;
	size_t	cut_len;

	cut_line = ft_strchr(*all_line, '\n');
	if (!cut_line)
	{
		cut_line = ft_strdup(*all_line);
		free(*all_line);
		*all_line = NULL;
		return (cut_line);
	}
	cut_len = cut_line - *all_line + 1;
	cut_line = ft_substr(*all_line, 0, cut_len);
	temp_all_line = ft_strdup(*all_line + cut_len);
	free(*all_line);
	*all_line = temp_all_line;
	return (cut_line);
}

char	*ft_read_buffer(char **temp_buffer,	t_get_next_line_var	*vars, int fd)
{
	size_t	bytes_read;

	bytes_read = ft_read_loop(fd, vars->buffer, &vars->temp_str);
	if (bytes_read <= 0)
	{
		free(*temp_buffer);
		*temp_buffer = NULL;
		return (vars->temp_str);
	}
	vars->str = ft_extract_line(vars->buffer, &vars->temp_str,
			temp_buffer, bytes_read);
	return (vars->str);
}
