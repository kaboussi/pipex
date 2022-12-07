/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relkabou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:07:16 by relkabou          #+#    #+#             */
/*   Updated: 2022/10/19 12:42:28 by relkabou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char *haystack, char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack && !len)
		return (0);
	if (!*needle || needle == haystack)
		return ((char *)haystack);
	while (*(haystack + i) && i < len)
	{
		j = 0;
		while (*(haystack + j + i) && *(needle + j)
			&& *(haystack + j + i) == *(needle + j) && i + j < len)
			j++;
		if (!*(needle + j))
			return ((char *)haystack + i);
		i++;
	}
	return (0);
}
