#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re

print("simple_code_check 0.0.1")

folders_for_check = []

folders_for_check.append({
    "path": "../fhq-server/src/",
    "exclude": [
        "../fhq-server/src/core/3rdParty",
        "../fhq-server/src/3rdParty/quazip-0.7.3"
    ]
})

folders_for_check.append({
    "path": "../fhq-server/unit-tests/",
    "exclude": []
})

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def log_warn(msg):
    global bcolors
    print(bcolors.WARNING + "WARNING: " + msg + bcolors.ENDC)

def log_err(msg):
    global bcolors
    print(bcolors.FAIL + "ERROR: " + msg + bcolors.ENDC)

def isExclude(p, path):
    ret = False
    for ex in p['exclude']:
        if path.find(ex) == 0:
            ret = True
    return ret

def getListOfFiles(p):
    f = []
    for (dirpath, dirnames, filenames) in os.walk(p['path']):
        for filepath in filenames:
            path = dirpath + "/" + filepath
            if not isExclude(p, path):
                f.append(path)
            else:
                print("Skipped: " + path)

    return f

warnings = {
    'check_line_tabs': 0
}

# TODO: check int nNumber 
# TODO: search TODO in line
# TODO: check if() - must contains space
# TODO check the if () {} - figure brakets
# TODO: auto - is great evil !!!

def check_line_tabs(filepath, line_number, l):
    global warnings
    if re.match(r'\t+', l):
        warnings['check_line_tabs'] = warnings['check_line_tabs'] + 1
        log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
            + 'Line: "' + l + '" \n'
            + 'What: found tabs on starts line\n')

_files = getListOfFiles(folders_for_check[0])
for filepath in _files:
    print(filepath)
    with open(filepath) as f:
        lines = f.readlines()
        line_number = 0
        for l in lines:
            l = l.rstrip('\n')
            check_line_tabs(filepath, line_number, l)
            line_number = line_number + 1

print(" ============= ")
print(" Warnings: " + str(warnings))
