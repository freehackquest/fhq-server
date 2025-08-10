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

""" subcommand clean removed temporary files """

import sys
import os
from .utils_files import UtilsFiles
from .utils_log import UtilsLog
from .pm_config import PmConfig


class CommandClean:
    """ CommandClean """
    def __init__(self, config: PmConfig):
        self.__log = UtilsLog("CommandClean").get_logger()
        self.__config = config
        self.__subcomamnd_name = "clean"

    def get_name(self):
        """ return subcommand name """
        return self.__subcomamnd_name

    def do_registry(self, subparsers):
        """ registring sub command """
        _parser_clean = subparsers.add_parser(
            name=self.__subcomamnd_name,
            description='Clean temporary build files'
        )
        _parser_clean.set_defaults(subparser=self.__subcomamnd_name)

    def execute(self, _):
        """ executing """
        self.__log.info("Cleaning...")
        dir_or_files_to_remove = [
            "Makefile",
            "build",
            "Makefile",
            "build",
            "Dockerfile.build-environment",
            "Dockerfile.release-environment",
            "tmp",
            "fhq-server",
            ".qmake.stash",
            "fhq-server.pro.user",
            "CMakeLists.txt.user",
            "debian/files",
        ]
        root_dir = self.__config.get_root_dir()
        for _path in dir_or_files_to_remove:
            _fullpath = os.path.join(root_dir, _path)
            self.__log.info("Removing %s", _fullpath)
            if os.path.isfile(_fullpath):
                self.__log.info("Removing file %s", _fullpath)
                os.remove(_fullpath)
            elif os.path.isdir(_fullpath):
                self.__log.info("Removing dir %s", _fullpath)
                UtilsFiles.recoursive_remove_files(_fullpath)
            else:
                self.__log.info("Not found")
        sys.exit(0)
