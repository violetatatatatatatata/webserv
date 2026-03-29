/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalcaide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:08:43 by aalcaide          #+#    #+#             */
/*   Updated: 2026/01/27 18:43:18 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

size_t	ft_linesize(const char *buffer)
{
	size_t	size;

	if (!buffer)
		return (0);
	size = 0;
	while (buffer[size])
	{
		if (buffer[size] == '\n')
		{
			size++;
			break ;
		}
		size++;
	}
	return (size);
}

size_t	ft_isnewline(const char *buffer, size_t bytes_read)
{
	size_t	i;

	i = 0;
	while (i < bytes_read)
	{
		if (buffer[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strcombine(const char *dest, const char *src)
{
	size_t	dest_size;
	size_t	src_size;
	char	*result;

	dest_size = 0;
	src_size = 0;
	if (!dest)
		dest = "";
	if (!src)
		src = "";
	while (dest[dest_size])
		dest_size++;
	while (src[src_size])
		src_size++;
	result = (char *)malloc(sizeof(char) * (src_size + dest_size + 1));
	if (!result)
		return (NULL);
	ft_memmove(result, dest, dest_size);
	ft_memmove(result + dest_size, src, src_size);
	result[src_size + dest_size] = '\0';
	return (result);
}
