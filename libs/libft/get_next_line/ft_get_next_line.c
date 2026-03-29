/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalcaide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:59:00 by aalcaide          #+#    #+#             */
/*   Updated: 2025/12/18 16:47:14 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_read_loop(int fd, char *buffer, char **temp_str)
{
	char	*new_comb;
	int		bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		if (ft_isnewline(buffer, bytes_read))
			break ;
		new_comb = ft_strcombine(*temp_str, buffer);
		free(*temp_str);
		*temp_str = new_comb;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	return (bytes_read);
}

char	*ft_extract_line(char *buffer, char **temp_str,
			char **temp_buffer, int bytes_read)
{
	char	*line;
	size_t	temp_size;
	size_t	line_size;

	line_size = ft_linesize(buffer);
	temp_size = 0;
	if (*temp_str)
	{
		temp_size = 0;
		while ((*temp_str)[temp_size])
			temp_size++;
	}
	line = ft_init_and_copy_line(temp_str, buffer, temp_size, line_size);
	if (!line)
		return (NULL);
	ft_handle_remaining_bytes(buffer, temp_buffer, bytes_read, line_size);
	return (line);
}

char	*ft_init_and_copy_line(char **temp_str, char *buffer,
			size_t temp_size, size_t line_size)
{
	size_t	i;
	char	*line;

	line = malloc(temp_size + line_size + 1);
	if (!line)
		return (NULL);
	if (*temp_str)
	{
		ft_memmove(line, *temp_str, temp_size);
		free(*temp_str);
		*temp_str = NULL;
	}
	i = 0;
	while (i < line_size)
	{
		line[temp_size + i] = buffer[i];
		i++;
	}
	line[temp_size + line_size] = '\0';
	return (line);
}

void	ft_handle_remaining_bytes(char *buffer, char **temp_buffer,
			int bytes_read, size_t line_size)
{
	size_t	i;

	if (bytes_read - (int)line_size <= 0)
		return ;
	*temp_buffer = malloc(bytes_read - line_size + 1);
	if (!*temp_buffer)
		return ;
	i = line_size;
	while ((int)i < bytes_read)
	{
		(*temp_buffer)[i - line_size] = buffer[i];
		i++;
	}
	(*temp_buffer)[bytes_read - line_size] = '\0';
}

char	*get_next_line(int fd)
{
	t_get_next_line_var	vars;
	static char			*temp_buffer = NULL;

	ft_bzero(vars.buffer, BUFFER_SIZE + 1);
	vars.temp_str = NULL;
	vars.str = NULL;
	if (temp_buffer)
	{
		if (ft_isnewline(temp_buffer, ft_linesize(temp_buffer)))
			return (ft_cut_new_line(&temp_buffer));
		vars.str = ft_strdup(temp_buffer);
		free(temp_buffer);
		temp_buffer = NULL;
	}
	return (ft_read_buffer(&temp_buffer, &vars, fd));
}
