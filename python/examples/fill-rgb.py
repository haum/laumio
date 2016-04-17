#!/usr/bin/env python3

import sys
import os

sys.path.append(os.path.dirname(sys.argv[0])+'/../')
from laumio import *

def print_usage():
    print(
"Usage: "+sys.argv[0]+" R G B\n\
\n\
    example:\n\
        "+sys.argv[0]+" 255 255 255\n\
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
