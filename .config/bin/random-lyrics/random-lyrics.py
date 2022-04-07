import time
import argparse
import os
import random
from threading import Timer

def file_type(path):
    if os.path.isfile(path):
        return 1
    elif os.path.isdir(path):
        return 2
    else:
        return 0

def list_file(path):
    return 0

def slow_print(text, seed):
    for line in text.splitlines():
        print(line)
        time.sleep(speed)
    return 0

all_args = argparse.ArgumentParser()
all_args.add_argument("-s", "--speed", help="load version")
all_args.add_argument("-f", "--filename", help="load version")
args = vars(all_args.parse_args())
text_file = None

if args["speed"]:
    speed = float(args["speed"])
else:
    speed = 0.1

if args["filename"]:
        filename = args["filename"]
        

if file_type(filename) == 1:
    with open(filename) as fl:
        text_file = fl.read()
elif file_type(filename) == 2:
    files = os.listdir(filename)
    filename = "%s/%s" % (filename, files[random.randint(0,len(files)-1)])
    with open(filename) as fl:
        text_file = fl.read()

if text_file:
    slow_print(text_file, speed)
