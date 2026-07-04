#!/usr/bin/env python3
import sys

body = sys.stdin.read()
sys.stdout.write("Content-Type: text/plain\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("Received: " + body + "\n")
sys.stdout.flush()
