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

""" init module for libfhqpm (Project Manager) helper lib for a handling processes in project """

import argparse
from .pm_config import PmConfig
from .command_check import CommandCheck
from .command_front import CommandFront
from .command_rebuild_environment_images import CommandRebuildEnvironmentImages
from .command_clang_format import CommandClangFormat
from .command_clean import CommandClean
from .command_code_check import CommandCodeCheck
from .command_code_stats import CommandCodeStats
from .command_create_storage_update import CommandCreateStorageUpdate
from .command_generate_models import CommandGenerateModels
from .command_py_check import CommandPyCheck
from .utils_copyrights import UtilsCopyrights
from .utils_log import UtilsLog
from .utils_files import UtilsFiles
from .utils_shell import UtilsShell
from .utils_strings import UtilsStrings


def print_custom_help(parser):
    """ print more comfortable help with subcommands"""
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


def run_main(root_dir):
    """ run_main """
    print("Root Directory: ", root_dir)
    _config = PmConfig(root_dir)
