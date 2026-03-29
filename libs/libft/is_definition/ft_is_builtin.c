/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalcaide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:35:45 by aalcaide          #+#    #+#             */
/*   Updated: 2026/01/27 17:30:55 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_is_buitlin(char *s)
{
	if (!s)
		return (FALSE);
	return (!ft_strncmp(s, "cd", sizeof("cd\0"))
		|| !ft_strncmp(s, "echo", sizeof("echo\0"))
		|| !ft_strncmp(s, "env", sizeof("env\0"))
		|| !ft_strncmp(s, "exit", sizeof("exit"))
		|| !ft_strncmp(s, "export", sizeof("export\0"))
		|| !ft_strncmp(s, "pwd", sizeof("pwd\0"))
		|| !ft_strncmp(s, "unset", sizeof("unset\0")));
}
