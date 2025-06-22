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

""" subcommand calculate statistics by source code """

import os
import sys
import logging

from .pm_config import PmConfig
from .utils_files import UtilsFiles

logging.basicConfig()


class CommandCodeStats:
    """ CommandCodeStats """
    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandCodeStats")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        self.__subcomamnd_name = "code-stats"

    def get_name(self):
        """ return subcommand name """
        return self.__subcomamnd_name

    def do_registry(self, subparsers):
        """ registring sub command """
        _parser_code_stats = subparsers.add_parser(
            name=self.__subcomamnd_name,
            description='Calculate statistics by source code'
        )
        _parser_code_stats.set_defaults(subparser=self.__subcomamnd_name)

    def execute(self, _):
        """ executing """
        self.__log.info("Start...")
        root_dir = self.__config.get_root_dir()
        src_dir = os.path.join(root_dir, "src")
        src_wsjcpp_dir = os.path.join(root_dir, "src.wsjcpp")
        src_resources_dir = os.path.join(root_dir, "src-resources.wsjcpp")

        all_code_lines = 0
        fhq_code_lines = 0
        thirdparty_code_lines = 0
        wsjcpp_code_lines = 0

        files = UtilsFiles.get_all_files(src_dir)
        for filepath in files:
            _fp = filepath.lower()
            is_header = _fp.endswith(".h") or _fp.endswith(".hpp")
            is_source_code = _fp.endswith(".cpp") or _fp.endswith(".c") or _fp.endswith(".cc")
            if is_source_code or is_header:
                lines = UtilsFiles.safe_read_file(filepath)
                all_code_lines += len(lines)
                if "third-party" not in _fp:
                    fhq_code_lines += len(lines)
                else:
                    thirdparty_code_lines += len(lines)

        files = UtilsFiles.get_all_files(src_resources_dir)
        for filepath in files:
            lines = UtilsFiles.safe_read_file(filepath)
            all_code_lines += len(lines)
            fhq_code_lines += len(lines)

        files = UtilsFiles.get_all_files(src_wsjcpp_dir)
        for filepath in files:
            lines = UtilsFiles.safe_read_file(filepath)
            is_header = _fp.endswith(".h") or _fp.endswith(".hpp")
            is_source_code = _fp.endswith(".cpp") or _fp.endswith(".c") or _fp.endswith(".cc")
            if is_source_code or is_header:
                lines = UtilsFiles.safe_read_file(filepath)
                all_code_lines += len(lines)
                wsjcpp_code_lines += len(lines)

        self.__log.info(
            "\n\n"
            "fhq-server code lines: %s\n"
            "wsjcpp code lines: %s\n"
            "third-party code lines: %s\n"
            "all code lines: %s\n",
            str(fhq_code_lines),
            str(wsjcpp_code_lines),
            str(thirdparty_code_lines),
            str(all_code_lines),
        )

        sys.exit(0)
