/* ************************************************************************** */

/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:15:33 by avelandr          #+#    #+#             */
/*   Updated: 2026/03/31 10:16:14 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <Webserv.hpp>
#include <ServerConfig.hpp>

class Config {
    private:
        std::vector<ServerConfig> _servers;

    public:
        Config();
        Config(Config const &src);
        
		virtual ~Config(void);
        Config &operator=(Config const &src);

        int parseFile(const char *f);

        std::vector<ServerConfig> getServers() const { return _servers; }
};

#endif
