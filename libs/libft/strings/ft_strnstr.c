/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:54:46 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:55:40 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	n;

	i = 0;
	if (*to_find == '\0')
		return ((char *)str);
	while (str[i])
	{
		n = 0;
		while ((str[i + n] == to_find[n]) && ((i + n) < len))
		{
			if (str[i + n] == '\0')
				return ((char *)str + i);
			n++;
		}
		if (to_find[n] == '\0')
			return ((char *)str + i);
		i++;
	}
	return (0);
}
