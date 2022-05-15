import os
import sys

if len(sys.argv) > 1:
    string = sys.argv[1]
else:
    string = input("str : ")

def search_string(text, string):
    in_line = []
    counter = 0
    for line in text.splitlines():
        counter += 1
        if string in line:
            in_line.append(counter)
    return in_line

def get_file_text(file):
    text = None
    if os.path.isfile(file):
        with open(file, "r") as f:
            try:
                text = f.read()
            except:
                pass
    return text

for root, folders, files in os.walk("."):
    for file in files:
        current_file = "%s/%s" % (root, file)
        file_text = get_file_text(current_file)
        if file_text != None:
            result = search_string(file_text, string)
            
            if result != []:
                print("%s: in line %s" % (current_file, result))