#!/usr/bin/env python3

import sys
import os
import time

sys.path.append(os.path.dirname(sys.argv[0])+'/../')
from laumio import *

def print_usage():
    print(
"Usage: "+sys.argv[0]+"\n\
\n\
    example:\n\
        "+sys.argv[0]+"\n\
")
    sys.exit(1)

if len(sys.argv) != 1:
    print(len(sys.argv))
    print_usage()

l = Laumio("172.28.172.3")
print(str(l.status().data))

