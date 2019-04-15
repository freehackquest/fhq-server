#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
import sys

print("simple_code_checker 0.0.1")

only_errors = False

for arg in sys.argv:
    if arg == './simple_code_checker.py':
        pass
    elif arg == '--only-errors':
        only_errors = True
    else:
        print("uknown argument " + str(arg))
        exit(-1)

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
                if not only_errors:
                    print("Skipped: " + path)
    return f

warnings = {
    'TODO': 0,
    'if-format': 0,
    'while-format': 0,
    'for-format': 0,
    'end-brackets': 0,
}

evil_pieces = {
    'auto': 0,
    'tabs': 0,
}

# TODO for(){
# TODO: check int nNumber 

def check_line_tabs(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'\t+', l):
        evil_pieces['tabs'] = evil_pieces['tabs'] + 1
        log_err('File: ' + filepath + ' (' + str(line_number) + ') \n'
            + 'Line: "' + l + '" \n'
            + 'What: found tabs on starts line\n')

def check_TODO(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*TODO.*', l):
        warnings['TODO'] = warnings['TODO'] + 1
        if not only_errors:
            log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
                + 'Line: "' + l + '" \n'
                + 'What: found TODO in line \n')

def check_if_format(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*if\(.*', l):
        warnings['if-format'] = warnings['if-format'] + 1
        if not only_errors:
            log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
                + 'Line: "' + l + '" \n'
                + 'What: found wrong if-format (expected space between "if" and "(") in line \n')

def check_for_format(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*for\(.*', l):
        warnings['for-format'] = warnings['for-format'] + 1
        if not only_errors:
            log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
                + 'Line: "' + l + '" \n'
                + 'What: found wrong if-format (expected space between "for" and "(") in line \n')

def check_while_format(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*while\(.*', l):
        warnings['while-format'] = warnings['while-format'] + 1
        if not only_errors:
            log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
                + 'Line: "' + l + '" \n'
                + 'What: found wrong while-format (expected space between "while" and "(") in line \n')

def check_end_brackets_format(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*\){.*', l):
        warnings['end-brackets'] = warnings['end-brackets'] + 1
        if not only_errors:
            log_warn('File: ' + filepath + ' (' + str(line_number) + ') \n'
                + 'Line: "' + l + '" \n'
                + 'What: found wrong end-brackets (expected space between ")" and "{") in line \n')

def check_auto_evil(filepath, line_number, l):
    global warnings
    global evil_pieces
    if re.match(r'.*[^\w]+auto[^\w]+.*', l):
        evil_pieces['auto'] = evil_pieces['auto'] + 1
        log_err('File: ' + filepath + ' (' + str(line_number) + ') \n'
            + 'Line: "' + l + '" \n'
            + 'What: found "auto" in line - c++ is a specific language\n')

_files = getListOfFiles(folders_for_check[0])
for filepath in _files:
    if not only_errors:
        print(filepath)
    with open(filepath) as f:
        lines = f.readlines()
        line_number = 0
        for l in lines:
            line_number = line_number + 1
            l = l.rstrip('\n')
            check_line_tabs(filepath, line_number, l)
            check_TODO(filepath, line_number, l)
            check_auto_evil(filepath, line_number, l)
            check_if_format(filepath, line_number, l)
            check_end_brackets_format(filepath, line_number, l)
            check_for_format(filepath, line_number, l)
            check_while_format(filepath, line_number, l)
            

print(" ============= \n")
for i in warnings:
    log_warn(" found '" + i + "' " + str(warnings[i]) + " times in code")
for i in evil_pieces:
    log_err(" found '" + i + "' " + str(evil_pieces[i]) + " times in code")

print("")