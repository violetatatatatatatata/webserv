/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:01:18 by avelandr          #+#    #+#             */
/*   Updated: 2026/01/28 18:32:04 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

# define TRUE	1
# define FALSE	0

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdarg.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_get_next_line_var
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read;
	char	*temp_str;
	char	*str;
}	t_get_next_line_var;

int		ft_atoi(const char *n);
char	*ft_itoa(int n);

void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nelem, size_t elsize);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t c);
void	*ft_memmove(void *dest, const void *src, size_t c);
void	*ft_memset(void *dest, int c, size_t count);

int		ft_is_buitlin(char *s);
int		ft_is_pipe(char *s);
int		ft_is_redirection(char *s);
int		is_controlop(char *s);
int		is_filename(const char *s);
int		is_redop(char *s);
int		is_token(char *s);
int		is_word(char *s);

int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_ismeta(int c);
int		ft_isspace(int c);

void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr_fd(int num, int fd);
void	ft_putstr_fd(char const *s, int fd);

char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(char *string);
size_t	ft_strlcat(char *dest, char *src, size_t size);
size_t	ft_strlcpy(char *dest, char *src, size_t size);
size_t	ft_strlen(const char *str);
int		ft_strncmp(char *s1, char *s2, unsigned int n);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strnstr(const char *str, const char *to_find, size_t len);
int		ft_tolower(int str);
int		ft_toupper(int str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);

int		ft_lstsize(t_list *lst);
t_list	*ft_lstnew(void *content);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new_lst);
void	ft_lstclear(t_list **lst, void (*del)(void *));

char	*get_next_line(int fd);
int		ft_read_loop(int fd, char *buffer, char **temp_str);
char	*ft_extract_line(char *buffer, char **temp_str, char **temp_buffer,
			int bytes_read);
char	*ft_init_and_copy_line(char **temp_str, char *buffer, size_t temp_size,
			size_t line_size);
void	ft_handle_remaining_bytes(char *buffer, char **temp_buffer,
			int bytes_read, size_t line_size);
size_t	ft_isnewline(const char *buffer, size_t bytes_read);
char	*ft_strcombine(const char *dest, const char *src);
size_t	ft_linesize(const char *buffer);
char	*ft_cut_new_line(char **all_line);
char	*ft_read_buffer(char **temp_buffer, t_get_next_line_var *vars, int fd);

#endif
