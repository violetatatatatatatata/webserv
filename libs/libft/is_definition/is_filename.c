/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_filename.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 02:05:33 by avelandr          #+#    #+#             */
/*   Updated: 2025/11/23 02:05:34 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	is_filename(const char *s)
{
	if (s == NULL || *s == '\0')
		return (1);
	while (*s)
	{
		if (*s == '/' || *s == '\0')
			return (1);
		s++;
	}
	return (0);
}
