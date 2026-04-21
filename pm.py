#!/usr/bin/env python3
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

import sys
import os
import argparse
import libfhqpm

ROOT_DIR = os.path.dirname(os.path.realpath(__file__))
# find the root dir (dir which contains dir 'libfhqpm')
PM_DIR = os.path.join(ROOT_DIR, 'libfhqpm')
PM_FILE = os.path.join(ROOT_DIR, 'pm.py')
while not os.path.isdir(PM_DIR) and not os.path.isfile(PM_FILE):
    ROOT_DIR = os.path.join(ROOT_DIR, '..')
    ROOT_DIR = os.path.normpath(ROOT_DIR)
    PM_DIR = os.path.join(ROOT_DIR, 'libfhqpm')
    PM_FILE = os.path.join(ROOT_DIR, 'pm.py')
    if ROOT_DIR == "/":
        sys.exit("Could not find rood dir")
# print("ROOT_DIR =", ROOT_DIR)

libfhqpm.run_main(ROOT_DIR)

CONFIG = libfhqpm.PmConfig(ROOT_DIR)

COMMANDS = [
    libfhqpm.CommandClean(CONFIG),
    libfhqpm.CommandFront(CONFIG),
    libfhqpm.CommandCodeStats(CONFIG),
    libfhqpm.CommandPyCheck(CONFIG),
    libfhqpm.CommandCheck(CONFIG),
    libfhqpm.CommandClangFormat(CONFIG),
    libfhqpm.CommandRebuildEnvironmentImages(CONFIG),
    libfhqpm.CommandCodeCheck(CONFIG),
    libfhqpm.CommandCreateStorageUpdate(CONFIG),
    libfhqpm.CommandGenerateModels(CONFIG),
]

class CustomActionHelp(argparse._HelpAction):  # pylint: disable=protected-access
    """ custom help action """
    def __call__(self, parser, namespace, values, option_string=None):
        libfhqpm.print_custom_help(parser)


if __name__ == "__main__":
    MAIN_PARSER = argparse.ArgumentParser(
        prog='pm',
        description='Project manager for current project',
        epilog='Helper tools for work with current project',
        add_help=False
    )
    MAIN_PARSER.add_argument('--help', '-h', action=CustomActionHelp, help='help')

    SUBCOMMANDS = MAIN_PARSER.add_subparsers(title='subcommands')
    for _command in COMMANDS:
        _command.do_registry(SUBCOMMANDS)
    ARGS = MAIN_PARSER.parse_args()
    if 'subparser' not in ARGS:
        libfhqpm.print_custom_help(MAIN_PARSER)
        sys.exit(1)
    SUBCOMMAND = ARGS.subparser
    for _command in COMMANDS:
        if _command.get_name() == SUBCOMMAND:
            _command.execute(ARGS)

# # new storage upgrade
# CREATE_STORAGE_UPDATE = 'create-storage-update'
# clang_format_parser = subparsers.add_parser(
#     name=CREATE_STORAGE_UPDATE,
#     description=''
# )
# clang_format_parser.set_defaults(subparser=CREATE_STORAGE_UPDATE)
#
# elif arguments.subparser == CREATE_STORAGE_UPDATE:
#     upgrade = libfhqpm.CreateStorageUpdate()
#     upgrade.create()
# else:
#     main_parser.print_help(sys.stderr)
#     sys.exit(1)

# # TODO
# # https://clang.llvm.org/extra/clang-tidy/
# # clang-tidy src/main.cpp -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
