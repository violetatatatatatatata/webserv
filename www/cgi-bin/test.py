#!/usr/bin/env python3
import sys
import os

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("<h1>Hello from CGI</h1>\n")
sys.stdout.write("<p>Method: " + os.environ.get("REQUEST_METHOD", "unknown") + "</p>\n")
sys.stdout.flush()
