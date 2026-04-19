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

""" Utils Copyrights """

import datetime
import sys
import re
import logging
from .utils_files import UtilsFiles


class UtilsCopyrights:
    """ UtilsCopyrights """

    def __init__(self, license_filepath):
        self.__source_copyrights = self.__get_source_copyright(license_filepath)

    def __get_source_copyright(self, license_filepath):
        _expected_years = "2011-" + str(datetime.datetime.now().year)
        _current_license_lines = UtilsFiles.safe_read_file(license_filepath)
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
                    logging.error(
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

    def get_bash_copyrights(self):
        """ return copyright for a .sh script """
        return ["#!/bin/bash"] + self.__bash_format_license(self.__source_copyrights)

    def get_cpp_copyrights(self):
        """ return copyright for a source code files """
        return self.__cpp_format_license(self.__source_copyrights)

    def get_python_copyrights(self):
        """ return copyright for a .py script """
        return ["#!/usr/bin/env python3"] + self.__bash_format_license(self.__source_copyrights)

    def get_cmake_copyrights(self):
        """ return copyright for a CMakeLists.txt files """
        return self.__bash_format_license(self.__source_copyrights)
