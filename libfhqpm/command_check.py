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

""" subcommand calculate statistics by source code """

import os
import re
import sys
from datetime import datetime
import logging

from .pm_config import PmConfig
from .utils_files import UtilsFiles

logging.basicConfig()


class CommandCheck:
    """ CommandCheck """
    def __init__(self, config: PmConfig):
        self.__log = logging.getLogger("CommandCheck")
        self.__log.setLevel(logging.DEBUG)
        self.__config = config
        self.__subcommand_name = "check"
        source_license_lines = self.__get_source_copyright(self.__config.get_root_dir())
        _bash_copyright = self.__bash_format_license(source_license_lines)
        self.__opt = {
            "bash_copyright": ["#!/bin/bash"] + _bash_copyright,
            "cpp_copyright": self.__cpp_format_license(source_license_lines),
            "py_copyright": ["#!/usr/bin/env python3"] + _bash_copyright,
            "cmake_copyright": _bash_copyright,
        }
        self.__errors = []
        # print("\n".join(self.__source_license_lines))
        self.__ignore_dirs = [
            "third-party",
            "src.wsjcpp",
            "node_modules",
            "tmp",
            "data",
            "freehackquest_libclient_py",  # TODO
            ".wsjcpp",
            "src-resources.wsjcpp",
            "unit-tests.wsjcpp",
        ]

    def get_name(self):
        """ return subcommand name """
        return self.__subcommand_name

    def do_registry(self, subparsers):
        """ registering sub command """
        _parser_check = subparsers.add_parser(
            name=self.__subcommand_name,
            description='Calculate statistics by source code'
        )
        _parser_check.set_defaults(subparser=self.__subcommand_name)

    def __get_source_copyright(self, root_dir):
        _expected_years = "2011-" + str(datetime.now().year)
        _current_license_lines = UtilsFiles.safe_read_file(
            os.path.join(root_dir, "LICENSE")
        )
        _re_years = re.compile(r'Copyright .* (\d{4}[ ]*-[ ]*\d{4}) ')
        source_license_lines = [
            "                     Project",
            "   __ _",
            "  / _| |__   __ _       ___  ___ _ ____   _____ _ __",
            " | |_| '_ \\ / _` | ___ / __|/ _ \\ '__\\ \\ / / _ \\ '__|",
            " |  _| | | | (_| ||___|\\__ \\  __/ |   \\ V /  __/ |",
            " |_| |_| |_|\\__, |     |___/\\___|_|    \\_/ \\___|_|",
            "               |_|",
            "",
        ]
        for _line in _current_license_lines:
            _line = _line.strip()
            source_license_lines.append(_line)
            _m_years = re.match(_re_years, _line)
            if _m_years:
                _years = _m_years.group(1)
                if _years != _expected_years:
                    self.__log.error(
                        "\n\nExpected '%s', but got '%s' in LICENSE file\n\n",
                        _expected_years,
                        _years
                    )
                    sys.exit(1)
        source_license_lines.extend([
            "",
            "Original repository: https://github.com/freehackquest/fhq-server",
            "",
        ])
        return source_license_lines

    def __bash_format_license(self, source_license_lines):
        ret = [
            "##################################################################################",
        ]
        for ln in source_license_lines:
            if len(ln) == 0:
                ret.append("#")
            else:
                ret.append("# " + ln)
        ret.extend([
            "##################################################################################",
            "",
        ])
        return ret

    def __cpp_format_license(self, source_license_lines):
        ret = [
            "/**********************************************************************************",
        ]
        for ln in source_license_lines:
            if len(ln) == 0:
                ret.append(" *")
            else:
                ret.append(" * " + ln)
        ret.extend([
            " ***********************************************************************************/",
            "",
        ])
        return ret

    def __check_copyrights_cpp(self):
        root_dir = self.__config.get_root_dir()
        src_files = UtilsFiles.get_all_files(
            os.path.join(root_dir, "src"),
            ignore_dirs=self.__ignore_dirs
        )
        # src_wsjcpp_dir = os.path.join(root_dir, "src.wsjcpp")
        # src_resources_dir = os.path.join(root_dir, "src-resources.wsjcpp")
        _cpp_copyright = self.__opt["cpp_copyright"]
        for _filepath in src_files:
            if _filepath.endswith(".png") or _filepath.endswith(".json"):
                continue  # skip
            _source_lines = UtilsFiles.safe_read_file(_filepath)
            if not UtilsFiles.compare_first_lines(_source_lines, _cpp_copyright):
                self.__log.error(
                    "\nExpected copyright \n%s\n in file %s\n",
                    "\n".join(_cpp_copyright),
                    _filepath
                )
                self.__errors.append(_filepath)

    @staticmethod
    def __cpp_filter(_filepath):
        if _filepath.endswith(".cpp") or _filepath.endswith(".h"):
            return True
        if _filepath.endswith(".cc") or _filepath.endswith(".hpp"):
            return True
        return False

    @staticmethod
    def __cmake_filter(_filepath):
        return _filepath.endswith("CMakeLists.txt")

    @staticmethod
    def __shell_filter(_filepath):
        return _filepath.endswith(".sh")

    @staticmethod
    def __py_filter(_filepath):
        return _filepath.endswith(".py")

    def __check_copyrights(self, _copyright, _filter):
        root_dir = self.__config.get_root_dir()
        _files = UtilsFiles.get_all_files(
            os.path.join(root_dir),
            ignore_dirs=self.__ignore_dirs
        )
        for _filepath in _files:
            if not _filter(_filepath):
                continue  # skip
            _source_lines = UtilsFiles.safe_read_file(_filepath)
            if not UtilsFiles.compare_first_lines(_source_lines, _copyright):
                self.__log.error(
                    "\nExpected copyright \n%s\n in file %s\n",
                    "\n".join(_copyright),
                    _filepath
                )
                self.__errors.append(_filepath)

    def execute(self, _):
        """ executing """
        self.__log.info("Start...")

        self.__check_copyrights(
            self.__opt["cpp_copyright"],
            CommandCheck.__cpp_filter,
        )
        self.__check_copyrights(
            self.__opt["bash_copyright"],
            CommandCheck.__shell_filter,
        )
        self.__check_copyrights(
            self.__opt["cmake_copyright"],
            CommandCheck.__cmake_filter,
        )
        self.__check_copyrights(
            self.__opt["py_copyright"],
            CommandCheck.__py_filter,
        )

        if len(self.__errors) > 0:
            self.__log.info("Errors %s", len(self.__errors))
            sys.exit(1)
        self.__log.info("Everything is fine")
        sys.exit(0)
