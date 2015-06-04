#!/usr/bin/python

import os
import sys

def process_includepaths(filename):
    if not os.path.isfile(filename):
        print "The path '%s' is not a valid file" % filename;
        return;
    print open(filename).read();
    

for s in sys.argv[1:]:
    process_includepaths(s);
