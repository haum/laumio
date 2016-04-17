#!/usr/bin/env python3

import sys

sys.path.append('../')
from laumio import *

def print_usage():
    print(
"Usage: rgb.py R G B\n\
\n\
    example:\n\
        rgb.py 255 255 255\n\
")
    sys.exit(1)

if len(sys.argv) != 4:
    print(len(sys.argv))
    print_usage()

r = None
g = None
b = None
try: 
    r = int(sys.argv[1])
    g = int(sys.argv[2])
    b = int(sys.argv[3])
except ValueError:
    print_usage()

l = Laumio("192.168.37.141")
l.fill(r, g, b)
