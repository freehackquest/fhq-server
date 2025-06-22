#!/usr/bin/env python3
##################################################################################
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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
##################################################################################

""" Command for format c++ code by clang-format """

import os
import sys
import logging
from .utils_files import UtilsFiles
from .pm_config import PmConfig
from .utils_shell import UtilsShell

logging.basicConfig()


class CommandClangFormat:
    """ CommandClangFormat """
    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandClangFormat")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        self.__subcomamnd_name = "clang-format"

    def get_name(self):
        """ return subcommand name """
        return self.__subcomamnd_name

    def do_registry(self, subparsers):
        """ registring sub command """
        _parser_clang_format = subparsers.add_parser(
            name=self.__subcomamnd_name,
            description='Fix cpp and h files use a clang format'
        )
        _parser_clang_format.set_defaults(subparser=self.__subcomamnd_name)

    def execute(self, _):
        """ executing """
        self.__log.info("Clang-format...")
        root_dir = self.__config.get_root_dir()
        src_dir = os.path.join(root_dir, "src")

        files = UtilsFiles.get_all_files(src_dir)
        for _file in files:
            if 'third-party' in _file:
                continue
            command = ["clang-format", '-style=file', '-i', _file]
            UtilsShell.run_command(command)
        sys.exit(0)
