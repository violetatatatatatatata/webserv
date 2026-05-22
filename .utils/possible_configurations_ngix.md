# Posibles configuraciones en un archivo .config

## server {
	# estos definen la identidad global de un servidor virtual.

	- **listen**: El puerto en el que el servidor esperará conexiones (ej. 8080). Es el dato más crítico para el Ingeniero.

	- **host**: La dirección IP (ej. 127.0.0.1). Junto con el puerto, define el socket único de escucha.

	- **server_name**: Una lista de nombres (dominios) para este servidor (ej. example.com, www.example.com). Se usa para distinguir entre varios servidores que comparten el mismo puerto.
	
	- **error_page**: Define rutas personalizadas para códigos de error HTTP específicos (404, 500, etc.).
	
	- **client_max_body_size**: El límite máximo de bytes permitidos en el cuerpo de una petición (vital para controlar subidas de archivos masivas).

	- **root**: Directorio base global para buscar archivos si no se especifica uno más concreto en una location.

	- **index**: Archivo por defecto a servir (ej. index.html) si se solicita un directorio a nivel global.
}

## location {
	# estos definen cómo se comporta el servidor ante una URL específica (ej. location /images { ... }).

	- **path**: El prefijo de la URL que activa estas reglas.

	- **methods**: Lista de métodos HTTP permitidos (GET, POST, DELETE). El servidor debe dar un error 405 si el método usado no está en esta lista.

	- **root**: El directorio real en el disco donde se encuentran los archivos para este prefijo de URL.

	- **index**: El archivo que se debe buscar si la petición es un directorio.

	- **autoindex**: Booleano (on/off) que activa o desactiva el listado automático de archivos cuando no hay un index.html.

	- **upload_store**: La ruta de la carpeta donde el servidor guardará los archivos subidos mediante peticiones POST.

	- **cgi_ext**: Las extensiones de archivo que deben procesarse como CGI (ej. .php, .py).

	- **cgi_path**: La ruta al ejecutable que procesará los scripts CGI (ej. /usr/bin/python3).

	- **return** (Redirección): Permite enviar al cliente a otra URL (habitualmente con un código 301 o 302).

## generales
	timeout: Tiempo máximo de espera antes de cerrar una conexión inactiva (muy útil para el Ingeniero).
