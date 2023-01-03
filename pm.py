#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) 2020-2023 FreeHackQuest Team <freehackquest@gmail.com>

import sys
import os
import argparse
import libfhqpm
import glob2

main_parser = argparse.ArgumentParser(
    prog='pm.py',
    description='What the program does',
    epilog='Text at the bottom of help'
)

subparsers = main_parser.add_subparsers(
    title='subcommands',
)

# clang-format
CLANG_FORMAT = 'clang-format'
clang_format_parser = subparsers.add_parser(
    name=CLANG_FORMAT,
    description='Run clang-format'
)
clang_format_parser.set_defaults(subparser=CLANG_FORMAT)

# main

arguments = main_parser.parse_args()
if 'subparser' not in arguments:
    main_parser.print_help(sys.stderr)
    sys.exit(1)

if arguments.subparser == CLANG_FORMAT:
    cmd = libfhqpm.CommandsHelper()
    files = []
    files.extend(glob2.glob(os.path.join("src", "**", "*.cpp")))
    files.extend(glob2.glob(os.path.join("src", "**", "*.h")))

    for _file in files:
        if '3rdParty' in _file:
            continue
        command = ["clang-format", '-style=file', '-i', _file]
        cmd.run_command(command)