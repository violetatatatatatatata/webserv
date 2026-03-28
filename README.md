[repo de referencia](https://github.com/cclaude42/webserv.git)
[http y sockets](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
[artículo sobre webserver](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7i)

├── Makefile <br>
├── srcs <br>
│   ├── core<br>
│   │   ├── Client.cpp      # estado de la conexion <br>
│   │   ├── Cluster.cpp     # gestion de multiples servidores <br>
│   │   └── Server.cpp      # sockets de escucha <br>
│   ├── cgi<br>
│   │   └── CGI.cpp<br>
│   ├── utils<br>
│   │   └── Utils.cpp<br>
│   ├── http<br>
│   │   ├── Request.cpp     # parser de peticiones <br>
│   │   ├── Response.cpp    # generar respuestas <br>
│   │   └── Router.cpp      # resolucion de rutas <br>
│   ├── parse<br>
│   │   └── Config.cpp      # parser <br>
│   └── Webserv.cpp         # main <br>
├── inc<br>
│   ├── Webserv.hpp         # cositas generales, macros, colores <br>
│   ├── core<br>
│   │   ├── Client.hpp <br>      
│   │   ├── Cluster.hpp <br>
│   │   └── Server.hpp <br>
│   └── http<br>
│       ├── AutoIndex.hpp<br>
│       ├── CGI.hpp        <br>
│       ├── Request.hpp    <br>
│       └── Response.hpp   <br>
├── www                     # sitios de pruebas <br>
│   └── index.html
