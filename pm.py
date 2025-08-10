#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) 2020-2023 FreeHackQuest Team <freehackquest@gmail.com>

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

CONFIG = libfhqpm.PmConfig(ROOT_DIR)

COMMANDS = [
    libfhqpm.CommandClean(CONFIG),
    libfhqpm.CommandCodeStats(CONFIG),
    libfhqpm.CommandPyCheck(CONFIG),
    libfhqpm.CommandClangFormat(CONFIG),
    libfhqpm.CommandRebuildEnvironmentImages(CONFIG),
    libfhqpm.CommandCodeCheck(CONFIG),
    libfhqpm.CommandCreateStorageUpdate(CONFIG),
    libfhqpm.CommandGenerateModels(CONFIG),
]

def print_custom_help(parser):
    """ print more comfortabl help with subcomamnds"""
    parser.print_help()
    # retrieve subparsers from parser
    subparsers_actions = [
        action for action in parser._actions  # pylint: disable=protected-access
        if isinstance(action, argparse._SubParsersAction)  # pylint: disable=protected-access
    ]
    print("")
    subcommand_name_maxlen = 0
    for subparsers_action in subparsers_actions:
        for choice, subparser in subparsers_action.choices.items():
            subcommand_name_maxlen = max(subcommand_name_maxlen, len(choice))
    subcommand_name_maxlen += 3
    for subparsers_action in subparsers_actions:
        for choice, subparser in subparsers_action.choices.items():
            subcommand_short_help = "  " + choice.ljust(subcommand_name_maxlen, " ")
            subcommand_short_help += subparser.description
            print(subcommand_short_help)
            # print(subparser.format_help())
    print("")
    parser.exit()


class CustomActionHelp(argparse._HelpAction):  # pylint: disable=protected-access
    """ custom help action """
    def __call__(self, parser, namespace, values, option_string=None):
        print_custom_help(parser)


if __name__ == "__main__":
    MAIN_PARSER = argparse.ArgumentParser(
        prog='stxpm',
        description='Project manager for c++ projects',
        epilog='Helper tools for work with c++ projects',
        add_help=False
    )
    MAIN_PARSER.add_argument('--help', '-h', action=CustomActionHelp, help='help')

    SUBCOMMANDS = MAIN_PARSER.add_subparsers(title='subcommands')
    for _comamnd in COMMANDS:
        _comamnd.do_registry(SUBCOMMANDS)
    ARGS = MAIN_PARSER.parse_args()
    if 'subparser' not in ARGS:
        print_custom_help(MAIN_PARSER)
        sys.exit(1)
    SUBCOMMAND = ARGS.subparser
    for _comamnd in COMMANDS:
        if _comamnd.get_name() == SUBCOMMAND:
            _comamnd.execute(ARGS)

# # new storage upgrade
# CREATE_STORAGE_UPDATE = 'create-storage-update'
# clang_format_parser = subparsers.add_parser(
#     name=CREATE_STORAGE_UPDATE,
#     description=''
# )
# clang_format_parser.set_defaults(subparser=CREATE_STORAGE_UPDATE)
# 
# elif arguments.subparser == CREATE_STORAGE_UPDATE:
#     upgarde = libfhqpm.CreateStorageUpdate()
#     upgarde.create()
# else:
#     main_parser.print_help(sys.stderr)
#     sys.exit(1)

# # TODO
# # https://clang.llvm.org/extra/clang-tidy/
# # clang-tidy src/main.cpp -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
