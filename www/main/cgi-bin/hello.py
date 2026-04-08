#!/usr/bin/env python3
import os
import sys

print("Content-type: text/html\r\n\r\n")
print("<html><head><title>CGI Python</title></head><body>")
print("<h1>hello world</h1>")

# Demostrar que el CGI recibe el Body para peticiones POST
if os.environ.get("REQUEST_METHOD") == "POST":
    body = sys.stdin.read()
    print("<h2>Body recibido por POST:</h2>")
    print(f"<p>{body}</p>")

# Demostrar que el CGI tiene las variables de entorno
print("<h2>Variables de Entorno:</h2><ul>")
for key, value in os.environ.items():
    print(f"<li><strong>{key}</strong>: {value}</li>")
print("</ul></body></html>")
