[repo de referencia](https://github.com/cclaude42/webserv.git)
[http y sockets](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
[artículo sobre webserver](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7i)

├── Makefile
├── srcs
│   ├── core
│   │   ├── Client.cpp      # estado de la conexion
│   │   ├── Cluster.cpp     # gestion de multiples servidores
│   │   └── Server.cpp      # sockets de escucha
│   ├── cgi
│   │   └── CGI.cpp
│   ├── utils
│   │   └── Utils.cpp
│   ├── http
│   │   ├── Request.cpp     # parser de peticiones
│   │   ├── Response.cpp    # generar respuestas
│   │   └── Router.cpp      # resolucion de rutas
│   ├── parse
│   │   └── Config.cpp      # parser
│   └── Webserv.cpp         # main
├── inc
│   ├── Webserv.hpp         # cositas generales, macros, colores
│   ├── core
│   │   ├── Client.hpp      
│   │   ├── Cluster.hpp
│   │   └── Server.hpp
│   └── http
│       ├── AutoIndex.hpp
│       ├── CGI.hpp        
│       ├── Request.hpp    
│       └── Response.hpp   
├── www                     # sitios de pruebas
│   └── index.html

