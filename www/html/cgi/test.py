import os
import sys

print("Hello world!")

path = os.environ["PATH_INFO"]
	
if path == "/one":
	print("ONE")
elif path == "/two":
	print("TWO")
else:
	print("UNKOWN")


data = sys.stdin.read()
print("Reçu :", data)
