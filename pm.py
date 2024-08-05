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

# new storage upgrade
CREATE_STORAGE_UPDATE = 'create-storage-update'
clang_format_parser = subparsers.add_parser(
    name=CREATE_STORAGE_UPDATE,
    description='Create storage update (for change struct of database)'
)
clang_format_parser.set_defaults(subparser=CREATE_STORAGE_UPDATE)

# code-check
CODE_CHECK = 'code-check'
code_check_parser = subparsers.add_parser(
    name=CODE_CHECK,
    description='Run code check'
)
code_check_parser.set_defaults(subparser=CODE_CHECK)

# py-check
PY_CHECK = 'py-check'
py_check_parser = subparsers.add_parser(
    name=PY_CHECK,
    description='Run py check'
)
py_check_parser.set_defaults(subparser=PY_CHECK)

# rebuild-environment-images
REBUILD_ENVIRONMENT_IMAGES = 'rebuild-environment-images'
rebuild_environment_images_parser = subparsers.add_parser(
    name=REBUILD_ENVIRONMENT_IMAGES,
    description='rebuild-environment-images (Dockerfile.build-environment && Dockerfile.release-environment)'
)
rebuild_environment_images_parser.set_defaults(subparser=REBUILD_ENVIRONMENT_IMAGES)

subcommands = libfhqpm.Subcommands()
subcommands.do_registry(subparsers)

# main

arguments = main_parser.parse_args()
if 'subparser' not in arguments:
    main_parser.print_help(sys.stderr)
    sys.exit(1)

subcommand = arguments.subparser

if subcommands.has_subcommand(arguments.subparser):
    subcommands.execute(arguments.subparser)
elif arguments.subparser == CLANG_FORMAT:
    cmd = libfhqpm.CommandsHelper()
    files = []
    files.extend(glob2.glob(os.path.join("src", "**", "*.cpp")))
    files.extend(glob2.glob(os.path.join("src", "**", "*.h")))

    for _file in files:
        if 'third-party' in _file:
            continue
        command = ["clang-format", '-style=file', '-i', _file]
        cmd.run_command(command)
elif arguments.subparser == CREATE_STORAGE_UPDATE:
    upgarde = libfhqpm.CreateStorageUpdate()
    upgarde.create()
elif arguments.subparser == CODE_CHECK:
    code_check = libfhqpm.CodeCheck()
    if code_check.run() > 0:
        sys.exit(1)
elif arguments.subparser == PY_CHECK:
    ret_pep8 = os.system("python3 -m pycodestyle libfhqpm --max-line-length=100")
    ret_pylint = os.system("python3 -m pylint libfhqpm --max-line-length=100")
    if ret_pep8 != 0 or ret_pylint != 0:
        sys.exit(1)
    sys.exit(0)
elif arguments.subparser == REBUILD_ENVIRONMENT_IMAGES:
    task = libfhqpm.RebuildEnvironmentImages()
    if task.run() > 0:
        sys.exit(1)
else:
    main_parser.print_help(sys.stderr)
    sys.exit(1)


# TODO
# https://clang.llvm.org/extra/clang-tidy/
# clang-tidy src/main.cpp -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*

