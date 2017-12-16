import re

with open('forwards.txt') as f:
    for line in f:
        if len(line.split()) != 0:
            funcName, ordinal = line.split(':')[1].split('@')[0], line.split(':')[1].split('@')[1]


            print(funcName + ' lol ' + ordinal)
