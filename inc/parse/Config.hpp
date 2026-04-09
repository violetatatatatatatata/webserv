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

/*	
 *	_servers: mapa para organizar configuraciones de servidor por puerto
 *
 *	Config: constructor por defecto
 *	~Config: destructor de la clase
 *	operator=: sobrecarga del operador de asignación
 *
 *	parseFile: lee y procesa el archivo de configuración completo para llenar el vector de servidores
 *	getServers: devuelve la lista de todos los servidores configurados
 * */
class Config {
    private:
			std::map<int, std::vector<ServerConfig> >	_servers;

    public:
        Config();
        Config(Config const &src);
		virtual ~Config(void);
        Config &operator=(Config const &src);

        int parseFile(const char *f);

		std::map<int, std::vector<ServerConfig> > getServers() const { return _servers; };
};

#endif
