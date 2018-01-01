import re

with open('forwards.txt') as f:
    for line in f:
        if len(line.split()) != 0:
            funcName, ordinal = line.split(':')[1].split('=')[0], line.split('@')[3].split('\"')[0]

            print('FORWARDED_EXPORT_WITH_ORDINAL('+funcName+','+ordinal+',binkw32_.'+funcName+')')
